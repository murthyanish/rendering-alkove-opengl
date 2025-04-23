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

  void computeHor(ComputeDispatchSize dispatch);
  void computeVert(ComputeDispatchSize dispatch);

private:
  std::shared_ptr<Shader> shaderVert;
};

#endif // !MOMENT_SHADOW_MAP_PASS_H
