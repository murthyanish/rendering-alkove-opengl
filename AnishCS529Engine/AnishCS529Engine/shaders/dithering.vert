///////////////////////////////////////////////////////////////////////////////
// Vertex shader for dithering
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 410
in vec4 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

out vec2 vertTexCoord;

void main() {
  vertTexCoord = vec2(vertexTexture.x, 1.0 - vertexTexture.y);
  gl_Position=vertex;
}