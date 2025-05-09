///////////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 440

#define M_PI 3.1415926535897932384626433832795

const float pi = 3.14159265358979323846;

uniform uint height, width;

uniform sampler2D GBuffer_position;
uniform sampler2D GBuffer_normal;
uniform sampler2D GBuffer_diffuse;
uniform sampler2D GBuffer_specular;
uniform sampler2D GBuffer_eye_vector;

uniform sampler2D edge_texture;
uniform sampler2D one_bit_lights;

uniform sampler2D dither_texture;

out vec3 FragColor;

float to3Shade(float value) {
  if (value > 0.8) return 1.0;
  if (value > 0.3) return 0.6;
  return 0.2;
}

void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    
    float isEdge = texture(edge_texture, gBufferPosition).x;
    float light = texture(one_bit_lights, gBufferPosition).x;
    
    float finalColor = 0.0;

    if (isEdge < 0.5) {
      finalColor = light;
    }
    else {
      if (light > 0.5) {
        finalColor = 0.0;
      }
      else {
        finalColor = 1.0;
      }
    }

    FragColor = vec3(to3Shade(finalColor));
}
