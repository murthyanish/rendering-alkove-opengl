/*!****************************************************************************
 * \file   UntexturedLightingPass.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   04-19-2025
 * \brief  Lighting RenderPass class for untextured objects in 3D rendering
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef UNTEXTUED_LIGHTING_PASS_H
#define UNTEXTUED_LIGHTING_PASS_H

#pragma once

#include "RenderPass.h"

class UntexturedLightingPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> screenQuad;
public:
  UntexturedLightingPass();

  void initialize();

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const override;
};

#endif // !UNTEXTUED_LIGHTING_PASS_H
