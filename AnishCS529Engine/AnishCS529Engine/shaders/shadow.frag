///////////////////////////////////////////////////////////////////////////////
// Fragment shader for Shadow
//
// Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#version 440

in vec4 position;
out vec4 FragColor;

void main()
{
  FragColor = vec4(
    position.w,
    pow(position.w, 2.0),
    pow(position.w, 3.0),
    pow(position.w, 4.0));
}
