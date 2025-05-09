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

DitheringPass::DitheringPass() :
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/dithering.vert\nshaders/dithering.frag");

  renderMask = RenderMask::LightingPass;
}

void DitheringPass::initialize()
{
  setProperty(
    "GBuffer_position", renderGraphBuilder->getTexture("GBuffer_position"));
  setProperty(
    "GBuffer_normal", renderGraphBuilder->getTexture("GBuffer_normal"));
  setProperty(
    "GBuffer_diffuse", renderGraphBuilder->getTexture("GBuffer_diffuse"));
  setProperty(
    "GBuffer_specular", renderGraphBuilder->getTexture("GBuffer_specular"));
  setProperty(
    "GBuffer_eye_vector", renderGraphBuilder->getTexture("GBuffer_eye_vector"));

  setProperty(
    "edge_texture", renderGraphBuilder->getTexture("edge_texture"));
  setProperty(
    "one_bit_lights", renderGraphBuilder->getTexture("one_bit_lights"));
}

void DitheringPass::addDitheringTexture(TextureManager::TextureID textureID)
{
  setProperty("dither_texture", textureID);
}

void DitheringPass::addDitheringTexture(const std::string& texturePath)
{
  auto texture = TextureManager::getInstance().loadFile(texturePath);
  TextureManager::getInstance().setTextureParameters(
    texture, TextureManager::TextureParameters());
  setProperty("dither_texture", texture);
}

void DitheringPass::draw(
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
