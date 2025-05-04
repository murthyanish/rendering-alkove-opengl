/*!****************************************************************************
 * \file   GBufferPrepass.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   04-18-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "GBufferPrepass.h"
#include "Renderer.h"

GBufferPrepass::GBufferPrepass() : RenderPass() {
  this->addShader("shaders/gbuffer.vert\nshaders/gbuffer.frag");

  renderMask = RenderMask::GBufferPrepass;
}

void GBufferPrepass::initialize() {
  renderGraphBuilder->createScreenSizeTexture("GBuffer_position");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_normal");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_diffuse");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_specular");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_eye_vector");
  gbuffer = renderGraphBuilder->createScreenSizeFBO(
    "GBuffer",
    {
      "GBuffer_position",
      "GBuffer_normal",
      "GBuffer_diffuse",
      "GBuffer_specular",
      "GBuffer_eye_vector"
    });
}

void GBufferPrepass::draw(std::shared_ptr<Camera> camera, SceneGraph* sceneGraph) const {

  Renderer::Viewport viewport = renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  gbuffer->bind();

  gbuffer->clear();

  RenderPass::draw(camera, sceneGraph);

  gbuffer->unbind();
}
