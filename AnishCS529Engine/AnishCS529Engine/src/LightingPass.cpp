/*!****************************************************************************
 * \file   LightingPass.cpp
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
#include "LightingPass.h"
#include "Renderer.h"

LightingPass::LightingPass() : 
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/lighting.vert\nshaders/lighting.frag");

  renderMask = RenderMask::LightingPass;
}

void LightingPass::initialize()
{
  setProperty(
    "GBuffer_position", renderGraphBuilder->getTexture("GBuffer_position"));
  setProperty(
    "GBuffer_normal", renderGraphBuilder->getTexture("GBuffer_normal"));
  setProperty(
    "GBuffer_diffuse", renderGraphBuilder->getTexture("GBuffer_diffuse"));
  setProperty(
    "GBuffer_specular", renderGraphBuilder->getTexture("GBuffer_specular"));
}

void LightingPass::draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const {

  shader->use();

  applyProperties();

  const Renderer::Viewport viewport = 
    renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  shader->setUInt("height", viewport.height);
  shader->setUInt("width", viewport.width);

  screenQuad->draw(GL_TRIANGLES);
}
