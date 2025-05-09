/*!****************************************************************************
 * \file   FinalOneBitPass.cpp
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-09-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "FinalOneBitPass.h"
#include "Renderer.h"
#include "RendererStateSaver.h"

FinalOneBitPass::FinalOneBitPass() :
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/final_one_bit.vert\nshaders/final_one_bit.frag");

  renderMask = RenderMask::UIPass;
}

void FinalOneBitPass::initialize()
{
  setProperty(
     "edge_texture", renderGraphBuilder->getTexture("edge_texture"));
  setProperty(
    "one_bit_lights", renderGraphBuilder->getTexture("dithered_B_image"));
}

void FinalOneBitPass::draw(
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
