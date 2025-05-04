///////////////////////////////////////////////////////////////////////////////
// Pixel shader for rendering Point Lights
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 440

#define M_PI 3.1415926535897932384626433832795

const float pi = 3.14159265358979323846;

layout (binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    mat4 inverseView;
    float exposure;
};

struct Light {
  vec3 position;
  float intensity;
  vec3 color;
  float radius;

  int castShadow;
  int shadowIndex;

  int padding[2];
};

layout(std430, binding = 2) buffer LightBuffer {
    Light lights[];
};

uniform uint lightIndex;

uniform uint height, width;

uniform sampler2D GBuffer_position;
uniform sampler2D GBuffer_normal;
uniform sampler2D GBuffer_diffuse;
uniform sampler2D GBuffer_specular;
uniform sampler2D GBuffer_eye_vector;

uniform mat4 ShadowMatrix;
uniform sampler2D ShadowMap;

out vec3 FragColor;

vec3 choleskyDecomposition(
  float m_11, float m_12, float m_13,
  float m_22, float m_23, float m_33,
  float z_1, float z_2, float z_3) {
  float a = sqrt(m_11);
  if ( a < 0 ) a = 0.0001;
  const float b = m_12/a;
  const float c = m_13/a;
  float d = sqrt(m_22 - pow(b,2));
  if ( d < 0 ) d = 0.0001;
  const float e = (m_23 - b*c)/d;
  float f_nos = m_33 - pow(c,2) - pow(e,2);
  if (f_nos <= 0) f_nos = 0.01;
  float f = sqrt(f_nos);
  if ( f < 0 ) f = 0.0001;

  const float c_b_1 = z_1/a;
  const float c_b_2 = (z_2 - b*c_b_1)/d;
  const float c_b_3 = (z_3 - c*c_b_1 - e*c_b_2)/f;

  const float c_3 = c_b_3/f;
  const float c_2 = (c_b_2 - e*c_3)/d;
  const float c_1 = (c_b_1 - b*c_2 - c*c_3)/a;


  return vec3(c_1, c_2, c_3);
}

vec2 solveQuadratic(float A, float B, float C) {
  float z1;
  float z2;
    float discriminant = B * B - 4.0 * A * C;

    if (discriminant < 0.0) {
        return vec2(0); // No real roots
    }

    float sqrtDisc = sqrt(discriminant);

    z1 = -B - sqrtDisc;
    z1 /= 2 * A;
    z2 = -B + sqrtDisc;
    z2 /= 2 * A;

//    // Numerically stable version
//    float q = (B > 0.0) ? -0.5 * (B + sqrtDisc)
//                        : -0.5 * (B - sqrtDisc);
//
//    z1 = q / A;
//    z2 = C / q;

    // Ensure z1 <= z2
    if (z1 > z2) {
        float temp = z1;
        z1 = z2;
        z2 = temp;
    }

    return vec2(z1,z2);
}

float hamburger4MSM(vec4 b, float z_f) {
  const float alpha = 0.001;
  const vec4 b_1 = (1-alpha)*b + alpha*vec4(0.5);

  const vec3 c = choleskyDecomposition(
    1, b_1.x, b_1.y,
    b_1.y, b_1.z, b_1.w,
    1, z_f, z_f*z_f);

  const vec2 z = solveQuadratic(c.z, c.y, c.x);
  const float z_2 = z.x;
  const float z_3 = z.y;
  if (z_2 == 0 && z_3 == 0) return 0.0;
  if (z_f <= z_2) return 0.0;
  else if (z_f <= z_3) {
    return (z_f * z_3 - b_1.x * (z_f + z_3) + b_1.y)
    /
    ((z_3 - z_2) * (z_f - z_2));
  }
  else {
    return 1 - ((z_2 * z_3 - b_1.x * (z_2 + z_3) + b_1.y)
    /
    ((z_f - z_2) * (z_f - z_3)));
  }
}


void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    
    vec3 worldPos = texture(GBuffer_position, gBufferPosition).xyz;

    const Light light = lights[lightIndex];

    vec3 L = light.position - worldPos;
    float radius = light.radius;
    float lightDistance = length(L);

    if ( lightDistance > radius ) {
      FragColor = vec3(0);
      discard;
      return;
    }

    float G = 0.0;

    if (light.castShadow > 0) {
      const vec4 shadowCoord = ShadowMatrix * vec4(worldPos, 1.0);
      vec2 shadowIndex = shadowCoord.xy/shadowCoord.w;

      vec4 b = texture(ShadowMap, shadowIndex);
      float z_f = shadowCoord.w;

      G = hamburger4MSM(b, z_f);
    }

    vec3 eyeVec = texture(GBuffer_eye_vector, gBufferPosition).xyz;

    vec3 N = texture(GBuffer_normal, gBufferPosition).xyz;
    vec3 V = normalize(eyeVec);

    vec3 Kd = texture(GBuffer_diffuse, gBufferPosition).xyz;
    vec3 Ks = texture(GBuffer_specular, gBufferPosition).xyz;
    float alpha = texture(GBuffer_specular, gBufferPosition).w;

    vec3 Ii = lights[lightIndex].color * lights[lightIndex].intensity;

    L = normalize(L);

    // The lighting calculation ...
    vec3 H = normalize(L+V);
    float LN = max(dot(L,N),0.0);
    float HN = max(dot(H,N),0.0);
    float VN = max(dot(V,N),0.0);
    float LH = max(dot(L,H),0.0);

    // BRDF
    vec3 F = Ks + (((1,1,1)-Ks)*pow((1-LH),5.0));
    float D = ((alpha+2.0)/(2*M_PI))*(pow(HN,alpha));
    vec3 BRDF = (Kd/M_PI) + ((F*D)/(4*pow(max(LH,0.0000000001),2.0)));
    FragColor = (Ii*LN*BRDF) * (1.0 - G);

    float constant  = 1.0;
    float linear    = 0.7;
    float quadratic = 1.8;


//    float attenuation = 
//      1.0 / (constant + linear * lightDistance + quadratic * (lightDistance * lightDistance));

    float attenuation = 
      (1/pow(lightDistance, 2) - 1/pow(radius, 2));


    FragColor *= attenuation;
}
