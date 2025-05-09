/*!****************************************************************************
 * \file   SurfaceColorPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-08-2025
 * \brief  Pass to set up surface colors so edge detection works better
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef SURFACE_COLOR_PASS_H
#define SURFACE_COLOR_PASS_H

#pragma once

#include "RenderPass.h"

class SurfaceColorPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> screenQuad;

public:
  SurfaceColorPass();

  void initialize();

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const override;
};

#endif // !SURFACE_COLOR_PASS_H
