///////////////////////////////////////////////////////////////////////////////
// Pixel shader for Skydome
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 430

const float pi = 3.14159265358979323846;

in vec3 eyePos;
uniform sampler2D skydomeTexture;

uniform bool HDR;

layout (binding = 0) uniform camera
{
    mat4 projection;
    mat4 view;
    mat4 inverseView;
    float exposure;
};

layout (location = 0) out vec4 FragData[4];

vec3 toSRGB(vec3 color) {
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
    vec3 V = normalize(eyePos);
    
    // Sky skips light processing so using a shortcut.
    vec2 uv = vec2(fract(0.5+atan(V.z,V.x)/(2.0f*pi)), acos(V.y)/pi);
    vec4 color = texture(skydomeTexture, uv);
    if (HDR) {
      color.xyz = toSRGB(color.xyz);
    }
    FragData[3] = color;
}
