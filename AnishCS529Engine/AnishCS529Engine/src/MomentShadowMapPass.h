/*!****************************************************************************
 * \file   MomentShadowMapPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-22-2025
 * \brief  Moment Shadow Map Compute Pass
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef MOMENT_SHADOW_MAP_PASS_H
#define MOMENT_SHADOW_MAP_PASS_H

#pragma once

#include "RenderPass.h"

class MomentShadowMapPass : public RenderPass {
public:
  MomentShadowMapPass();

  /*virtual void initialize() override;

  virtual void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* sceneGraph) const override;*/

private:
  FBO* gbuffer;

};

#endif // !MOMENT_SHADOW_MAP_PASS_H
