///////////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting
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

uniform bool HDR;

uniform uint height, width;

uniform sampler2D GBuffer_position;
uniform sampler2D GBuffer_normal;
uniform sampler2D GBuffer_diffuse;
uniform sampler2D GBuffer_specular;

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
      ), vec3(1/2.2));
}

void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    
    vec3 worldPos = texture(GBuffer_position, gBufferPosition).xyz;
    vec3 eyeVec = (inverseView*vec4(0,0,0,1)).xyz-worldPos;

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

    // The lighting calculation ...
    vec3 H = normalize(L+V);
    float LN = max(dot(L,N),0.0);
    float HN = max(dot(H,N),0.0);
    float VN = max(dot(V,N),0.0);
    float LH = max(dot(L,H),0.0);

    vec3 ambientDiffuse = Ia*Kd;

    // BRDF
    vec3 F = Ks + (((1,1,1)-Ks)*pow((1-LH),5.0));
    float D = ((alpha+2.0)/(2*M_PI))*(pow(HN,alpha));
    vec3 BRDF = (Kd/M_PI) + ((F*D)/(4*pow(max(LH,0.0000000001),2.0)));
    FragColor = ambientDiffuse + (Ii*LN*BRDF);

    FragColor = toSRGB(FragColor);
}
