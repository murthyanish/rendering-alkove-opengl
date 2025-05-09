/*!****************************************************************************
 * \file   DitheringPass.cpp
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-08-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "DitheringPass.h"
#include "Renderer.h"
#include "RendererStateSaver.h"

DitheringPass::DitheringPass() :
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/dithering.vert\nshaders/dithering.frag");

  renderMask = RenderMask::LocalLightsPass;
}

void DitheringPass::initialize()
{
  ditheredAImage = renderGraphBuilder->createScreenSizeTexture("dithered_A_image", 4);
  ditherABuffer = renderGraphBuilder->createScreenSizeFBO(
    "ditherABuffer",
    {
      "dithered_A_image"
    });
  ditheredBImage = renderGraphBuilder->createScreenSizeTexture("dithered_B_image", 4);
  ditherBBuffer = renderGraphBuilder->createScreenSizeFBO(
    "ditherBBuffer",
    {
      "dithered_B_image"
    });

  setProperty(
    "one_bit_lights", renderGraphBuilder->getTexture("one_bit_lights"));
}

void DitheringPass::addDitheringTexture(TextureManager::TextureID textureID)
{
  setProperty("noise", textureID);
}

void DitheringPass::addDitheringTexture(const std::string& texturePath)
{
  auto texture = TextureManager::getInstance().loadFile(texturePath);
  TextureManager::getInstance().setTextureParameters(
    texture, TextureManager::TextureParameters());
  setProperty("noise", texture);
}

void DitheringPass::draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const {

  Renderer::StateSaver savedState(*(renderGraphBuilder->getRenderer()));
  renderGraphBuilder->getRenderer()->setDepthState(Renderer::DepthState(false));

  shader->use();

  applyProperties();

  const Renderer::Viewport viewport =
    renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  shader->setUInt("height", viewport.height);
  shader->setUInt("width", viewport.width);
  shader->setUInt("firstPass", 1);

  ditherABuffer->bind();
  shader->bindTexture(15, "light_texture", renderGraphBuilder->getTexture("one_bit_lights"));

  for (int t = 0; t < 36; ++t) {
    shader->setUInt("px", t % 3);
    shader->setUInt("py", (t / 3) % 3);
    screenQuad->draw(GL_TRIANGLES);
    if (t % 2 == 1) {
      ditherABuffer->bind();
      shader->bindTexture(15, "light_texture", renderGraphBuilder->getTexture("dithered_B_image"));
    }
    else {
      ditherBBuffer->bind();
      shader->bindTexture(15, "light_texture", renderGraphBuilder->getTexture("dithered_A_image"));
    }
    shader->setUInt("firstPass", 0);
  }

  // Ends with B

  shader->unbindTexture(15);

  ditherABuffer->unbind();
}
