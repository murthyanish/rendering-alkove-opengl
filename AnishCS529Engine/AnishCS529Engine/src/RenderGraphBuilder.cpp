/*!****************************************************************************
 * \file   RenderGraphBuilder.cpp
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
#include "RenderGraphBuilder.h"
#include "Renderer.h"

FBO* RenderGraphBuilder::createScreenSizeFBO(
  const std::string& name, std::vector<std::string> attachments) {

  Renderer::Viewport viewport = renderer->getCurrentState().viewport;

  return createFBO(name, attachments, viewport.width, viewport.height);
}

FBO* RenderGraphBuilder::createFBO(const std::string& name, std::vector<std::string> attachments, int width, int height)
{
  FBO* fbo = new FBO();

  fbo->setViewport(0, 0, width, height);
  fbo->initialize();
  fbos[name] = fbo;

  for (const auto& attachment : attachments) {
    fbo->attachTexture(textures[attachment]);
  }

  fbo->finalize();
  fbo->addScreenSizeBufferUpdateCallback(renderer);

  return fbo;
}

TextureManager::TextureID RenderGraphBuilder::createScreenSizeTexture(
  const std::string& name) {

  Renderer::Viewport viewport = renderer->getCurrentState().viewport;
  return createTexture(name, viewport.width, viewport.height);
}

TextureManager::TextureID RenderGraphBuilder::createTexture(const std::string& name, int width, int height)
{
  TextureManager::TextureInfo textureInfo = TextureManager::TextureInfo();
  textureInfo.width = width;
  textureInfo.height = height;
  textureInfo.format = TEXTURE_RGBA32F;
  TextureManager::TextureParameters textureParameters = TextureManager::TextureParameters();
  textureParameters.max_level = 0;
  textureParameters.wrap_s = TEXTURE_CLAMP_TO_EDGE;
  textureParameters.wrap_t = TEXTURE_CLAMP_TO_EDGE;
  textureParameters.mag_filter = TEXTURE_LINEAR;
  textureParameters.min_filter = TEXTURE_LINEAR;
  TextureManager::TextureID texture = TextureManager::getInstance().createTexture(textureInfo, nullptr, textureParameters);
  textures[name] = texture;

  return texture;
}

FBO* RenderGraphBuilder::getFBO(const std::string & name) {
  return fbos[name];
}

TextureManager::TextureID RenderGraphBuilder::getTexture(const std::string & name) {
  return textures[name];
}

void RenderGraphBuilder::clearFBOs() {
  for ( const auto & [name, fbo] : fbos ) {
    if (fbo) delete fbo;
  }

  fbos.clear();
}

void RenderGraphBuilder::clearTextures()
{
  textures.clear();
}

void RenderGraphBuilder::clear()
{
  clearFBOs();
  clearTextures();
}
