/*!****************************************************************************
 * \file   Lights.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   04-04-2025
 * \brief  Lights supported by the engine
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef LIGHTS_H
#define LIGHTS_H

#pragma once

#include "Vector3.h"

struct DirectionalLight {
  Vector3 direction;
  float intensity;
  Vector3 color;
  float padding = 0.0f; // Padding to align the data correctly for OpenGL.
};

struct PointLight {
  Vector3 position;
  float intensity;
  Vector3 color;
  float range;
  int castShadow = 0;
  int shadowIndex = -1;
  // Struct size is 40, so we need to pad after to align next struct to 16 byte
  // boundary.
  int padding[2];
};

struct AmbientLight {
  Vector3 color;
  float intensity;
};

struct LightingPassLights {
  DirectionalLight sunLight;
  AmbientLight ambientLight;
};

struct Lights {
  DirectionalLight sunLight;
  AmbientLight ambientLight;
  std::vector<PointLight> pointLights;
};

struct LightStatus {
  bool sunDirty = false;
  bool ambientDirty = false;
  std::vector<unsigned int> pointDirty = {};
};

#endif // !LIGHTS_H
