/*!****************************************************************************
 * \file   OneBitLightsPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-08-2025
 * \brief  One Bit Lighting Pass that renders to white, black or grey only.
 * 
 * This is part of my Project 5 for Graphics. I am trying to do the rendering
 * from Return of the Obra Dinn.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef ONE_BIT_LIGHTS_PASS_H
#define ONE_BIT_LIGHTS_PASS_H

#pragma once

#include "RenderPass.h"

class OneBitLightsPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> sphereMesh;

public:
  OneBitLightsPass();

  void initialize();

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* sceneGraph) const override;
};

#endif // !ONE_BIT_LIGHTS_PASS_H
