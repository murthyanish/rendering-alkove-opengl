///////////////////////////////////////////////////////////////////////////////
// Pixel shader for surface color pass
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

uniform uint height, width;

uniform sampler2D GBuffer_position;
uniform sampler2D GBuffer_normal;
uniform sampler2D GBuffer_diffuse;
uniform sampler2D GBuffer_specular;
uniform sampler2D GBuffer_eye_vector;

out vec3 FragColor;

void main()
{
    vec2 gBufferPosition = gl_FragCoord.xy/vec2(width,height);
    
    vec3 worldPos = texture(GBuffer_position, gBufferPosition).xyz;
    vec3 eyeVec = texture(GBuffer_eye_vector, gBufferPosition).xyz;
    float depth = texture(GBuffer_eye_vector, gBufferPosition).w;

    vec3 N = texture(GBuffer_normal, gBufferPosition).xyz;
    vec3 V = normalize(eyeVec);

    vec3 Kd = texture(GBuffer_diffuse, gBufferPosition).xyz;
    vec3 Ks = texture(GBuffer_specular, gBufferPosition).xyz;
    float alpha = texture(GBuffer_specular, gBufferPosition).w;
    
    // Lucas Pope's method:
//    vec3 origin = (inverseView*vec4(0.0,0.0,0.0,1.0)).xyz;
//    vec3 area = vec3(100.0);
//    vec3 cameraDir = normalize((inverseView * vec4(0.0,0.0,-1.0,1.0)).xyz);
//    float light = clamp((dot(N, cameraDir) + 1.0) * 0.5, 0.0, 1.0);
//
//    FragColor = ((origin + area) * 0.5) / area;
//    FragColor.x *= light;
//    FragColor.y /= light;
//    //FragColor *= v.color.g; //??
//    FragColor = fract(FragColor * 100.0);

    // Normal + Depth Buffer Edge Detection (Geometry-Based Sobel)
    // Compute gradients
    vec3 normal_dx = dFdx(N);
    vec3 normal_dy = dFdy(N);
    float normalEdge = length(normal_dx) + length(normal_dy);

    float depth_dx = dFdx(depth);
    float depth_dy = dFdy(depth);
    float depthEdge = abs(depth_dx) + abs(depth_dy);

    float edgePoint = 0.2;
    float normalWeight = 1.0;
    float depthWeight = 1.0;

    // Edge strength
    float edge = step(edgePoint, normalEdge * normalWeight + depthEdge * depthWeight);

    // Output black edge on white background
    FragColor = vec3(1.0 - edge);


}
