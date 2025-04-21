/*!****************************************************************************
 * \file   ShadowPass.cpp
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-20-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "ShadowPass.h"

#include "Renderer.h"

ShadowPass::ShadowPass() : RenderPass()
{
  this->addShader("shaders/shadow.vert\nshaders/shadow.frag");

  renderMask = RenderMask::ShadowPass;
}
