///////////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting
//
// Copyright ? 2025 DIGIPEN Institute of Technology. All rights reserved.
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

struct DirectionalLight {
  vec3 direction;
  float intensity;
  vec3 color;
  float padding0;
};

struct AmbientLight {
  vec3 color;
  float intensity;
};

layout (binding = 1) uniform lights
{
  DirectionalLight sunLight;
  AmbientLight ambientLight;
};

layout (binding = 4) uniform irradianceSpecularPointsBlock
{
  float irradianceSpecularPointCount;
  float irradianceSpecularPointPadding;
  vec2 irradianceSpecularPoints[100];
};

uniform bool HDR;

uniform uint height, width;

uniform sampler2D GBuffer_position;
uniform sampler2D GBuffer_normal;
uniform sampler2D GBuffer_diffuse;
uniform sampler2D GBuffer_specular;

// Irradiance Map
uniform int enableIrradiance;
uniform sampler2D IrradianceMap;

// Skybox
uniform int enableIBLSpecular;
uniform sampler2D skydome;

out vec3 FragColor;

vec3 toSRGB(vec3 color) {
  const float exposure = 1.0f;
  return pow(
      (
        (exposure * color) / 
        (
          (exposure * color) + 
          vec3(1.0)
        )
      ), vec3(1.0/2.2));
}

vec2 uvFromDirectionVector(vec3 D) {
  return vec2(atan(-D.z,D.x)/(2.0f*pi), acos(D.y)/pi);
}

vec2 uvFromDirectionVectorFlipped(vec3 D) {
  return vec2(0.5 - atan(-D.z,D.x)/(2.0f*pi), acos(D.y)/pi);
}

vec3 directionVectorFromSkewAngle(vec2 uv) {

  //vec3 result = vec3()

  return vec3(
    cos(2.0 * pi * ( 0.5 - uv.x )) * sin(pi * uv.y),
    cos(pi * uv.y),
    -sin(2.0 * pi * ( 0.5 - uv.x )) * sin(pi * uv.y)
    );
}

vec3 directionVectorFromUVFlipped(vec2 uv) {
  return vec3(
    cos(2 * pi * ( 0.5 - uv.y )) * sin(pi * uv.x),
    sin(2 * pi * ( 0.5 - uv.y )) * sin(pi * uv.x),
    cos(pi * uv.y)
    );
}

// L = Light vector
// N = Normal vector
// V = Eye vector
// Ii = Light color at Light vector
// Kd = Diffuse color of surface
// Ks = Specular color of surface
// alpha = Shininess
vec3 specularBRDF(
  const vec3 L, const vec3 N, const vec3 V,
  const vec3 Ii,
  const vec3 Kd, const vec3 Ks, const float alpha) {

  const vec3 H = normalize(L+V);
  const float LN = max(dot(L,N),0.0);
  const float HN = max(dot(H,N),0.0);
  const float VN = max(dot(V,N),0.0);
  const float LH = max(dot(L,H),0.0);

  // BRDF
  const vec3 F = Ks + (((1.0,1.0,1.0)-Ks)*pow((1.0-LH),5.0));
  const float D = ((alpha+2.0)/(2.0*M_PI))*(pow(HN,alpha));
  const vec3 BRDF = (Kd/M_PI) + ((F*D)/(4*pow(max(LH,0.0000000001),2.0)));
  return Ii*LN*BRDF;
}

vec3 irradianceSpecular(
  vec3 N, vec3 V,
  const vec3 Kd, const vec3 Ks, const float alpha) {

  vec3 finalSpecular = vec3(0);

  int n = int(irradianceSpecularPointCount);
  
  const float VN = max(dot(V,N),0.0);

  // Reflection vector
  vec3 R = reflect(V, N);

  vec3 A = normalize(cross(vec3(0.0f,1.0f,0.0f), R));
  vec3 B = normalize(cross(R, A));

  for (int i = 0; i < n; ++i) {
    const float theta = acos(pow(irradianceSpecularPoints[i].y, (1.0/(alpha+1.0))));

    const vec3 D = directionVectorFromSkewAngle(
      vec2(irradianceSpecularPoints[i].x, theta/pi));

    const vec3 omega_k = -normalize(D.x * A + -D.z * B + D.y * R);


    const vec3 H = normalize(omega_k+V);
    const float LN = max(dot(omega_k,N),0.0);
    const float HN = max(dot(H,N),0.0);
    const float LH = max(dot(omega_k,H),0.0);

    // BRDF
    const vec3 F = Ks + (((1.0,1.0,1.0)-Ks)*pow((1.0-LH),5.0));
    const float specD = ((alpha+2.0)/(2.0*M_PI))*(pow(HN,alpha));

    int level = int(0.5*log2(float(width * height) / float(n)) - 0.5*log2(specD));

    const vec3 IiIBL =
      textureLod(skydome, uvFromDirectionVectorFlipped(-omega_k), level).xyz;
    
    //const vec3 BRDF = IiIBL * clamp(N*omega_k, 0.001) * ((F*specD)/(4*pow(max(LH,0.0000000001),2.0)));
    const vec3 BRDF = (Kd/M_PI) + ((F)/(4*pow(max(LH,0.0000000001),2.0)));
    
    finalSpecular += IiIBL*LN*BRDF;
  }

  return finalSpecular / n;
}

void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    
    vec3 worldPos = texture(GBuffer_position, gBufferPosition).xyz;
    vec3 eyeVec = (inverseView*vec4(0.0,0.0,0.0,1.0)).xyz-worldPos;

    vec3 N = texture(GBuffer_normal, gBufferPosition).xyz;
    vec3 V = normalize(eyeVec);

    vec3 Kd = texture(GBuffer_diffuse, gBufferPosition).xyz;
    vec3 Ks = texture(GBuffer_specular, gBufferPosition).xyz;
    float alpha = texture(GBuffer_specular, gBufferPosition).w;

    vec3 Ia = ambientLight.color * ambientLight.intensity;
    
    vec3 Ii = sunLight.color * sunLight.intensity;
    vec3 L = -normalize(sunLight.direction);
    

    if (texture(GBuffer_diffuse, gBufferPosition).w == 0.0f) {
      FragColor = toSRGB(Kd);
      return;
    }

    vec3 ambientDiffuse;

    FragColor = specularBRDF(L, N, V, Ii, Kd, Ks, alpha);

    if (enableIrradiance > 0) {
      vec2 uv = uvFromDirectionVectorFlipped(N);
      ambientDiffuse = Kd * (texture(IrradianceMap, uv).xyz) / M_PI;
      
      if (enableIBLSpecular > 0) {
        FragColor += irradianceSpecular(
          N, V,
          Kd, Ks, alpha);
      }
    }
    else {
      ambientDiffuse = Ia*Kd;
    }
    FragColor += ambientDiffuse;

    FragColor = toSRGB(FragColor);
}
