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

/*!****************************************************************************
 * \struct PointLight
 * \brief Struct to store point light related params for OpenGL.
 *****************************************************************************/
struct PointLight {
  /**
   * \brief The position of the light in world space.
   */
  Vector3 position;
  /**
   * \brief The intensity of the light.
   */
  float intensity;
  /**
   * \brief The color of the light.
   */
  Vector3 color;
  /**
   * \brief The maximum range of the light's influence.
   *
   * Fragments beyond this range are not affected by this light.
   * Note that intensity scales with this value. Increase intensity as you
   * increase range, else the light will drop off too fast.
   */
  float range;
  /**
   * \brief Flag indicating whether this light casts shadows.
   *
   * 0 means no shadows; non-zero means the light casts shadows.
   */
  int castShadow = 0;
  /**
   * \brief Rotation of the shadow.
   *
   * We only need two since we don't care about roll.
   */
  float rotX = 0.0f;
  float rotY = 0.0f;
  /**
   * \brief Index of the shadow map associated with this light.
   *
   * A value of -1 indicates no shadow map is assigned.
   */
  int shadowIndex = -1;
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
