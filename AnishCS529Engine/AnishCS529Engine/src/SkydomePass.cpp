/*!****************************************************************************
 * \file   SkydomePass.cpp
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
#include "SkydomePass.h"

SkydomePass::SkydomePass() :
  RenderPass(), sphereMesh(Mesh::createSphereMesh("sphere", 32)),
  scale(Matrix4::scale(Vector3(2000.0f))) {

  this->addShader("shaders/skydome.vert\nshaders/skydome.frag");
  shader->initializeUBO("camera", 0);

  renderMask = RenderMask::SkydomePass;
}

void SkydomePass::addTexture(TextureManager::TextureID textureID)
{
  skydomeTexture = textureID;
  HDR = TextureManager::getInstance().isHDR(skydomeTexture);
  setProperty("skydomeTexture", skydomeTexture);
  setProperty("HDR", HDR);
}

void SkydomePass::addTexture(const std::string& textureName)
{
  skydomeTexture = TextureManager::getInstance().loadFile(textureName);
  HDR = TextureManager::getInstance().isHDR(skydomeTexture);
  setProperty("skydomeTexture", skydomeTexture);
  setProperty("HDR", HDR);
}

void SkydomePass::initialize() {
  renderGraphBuilder->createScreenSizeTexture("GBuffer_position");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_normal");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_diffuse");
  renderGraphBuilder->createScreenSizeTexture("GBuffer_specular");
  gbuffer = renderGraphBuilder->createScreenSizeFBO(
    "GBuffer",
    {
      "GBuffer_position",
      "GBuffer_normal",
      "GBuffer_diffuse",
      "GBuffer_specular"
    });
}

void SkydomePass::draw(
  std::shared_ptr<Camera> camera,
  SceneGraph* sceneGraph) const {
  glDepthMask(GL_FALSE);

  gbuffer->bind();

  gbuffer->useViewport();

  gbuffer->clear();

  shader->use();

  applyProperties();

  sphereMesh->draw(GL_TRIANGLES);

  unbindTextures();

  shader->unuse();

  gbuffer->unbind();

  glDepthMask(GL_TRUE);
}
