/*!****************************************************************************
 * \file   FreeCamera.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   04-01-2025
 * \brief  Free camera that can be controlled independently
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#pragma once

#include <cassert>
#include <memory>

 /* Base class */
#include "Camera.h"

class FreeCamera : public Camera, public std::enable_shared_from_this<FreeCamera> {
public:
  FreeCamera(std::string _name) : Camera(_name) {};
  ~FreeCamera() = default;

  std::shared_ptr<FreeCamera> lookAt(
    const Vector3 & target,
    const Vector3 & upVector = Vector3(0, 1, 0));
  std::shared_ptr<FreeCamera> move(
    const Vector3 & direction,
    float amount);
  std::shared_ptr<FreeCamera> rotate(
    float roll,
    float pitch,
    float yaw
  );
  std::shared_ptr<FreeCamera> rotate(
    Quaternion _rotation
  );

  std::shared_ptr<FreeCamera> setPosition(
    Vector3 _position
  );

  std::shared_ptr<FreeCamera> setRotation(
    float roll,
    float pitch,
    float yaw
  );

  std::shared_ptr<FreeCamera> setRotation(
    Quaternion _rotation
  );
};

#endif // !FREE_CAMERA_H
