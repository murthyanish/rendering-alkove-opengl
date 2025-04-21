///////////////////////////////////////////////////////////////////////////////
// Vertex shader for Shadow
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 440

in vec4 vertex;
in vec3 vertexNormal;
in vec2 vertexTexture;
in vec3 vertexTangent;

uniform mat4 ViewMatrix, ProjectionMatrix;

uniform mat4 ModelMatrix;

out vec4 position;

void main()
{
    // Computes the point’s projection on the screen
    gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vertex;
    position = gl_Position;
}
