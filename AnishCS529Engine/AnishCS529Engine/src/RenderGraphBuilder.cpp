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

#include "RendererStateSaver.h"

FBO* RenderGraphBuilder::createScreenSizeFBO(
  const std::string& name, std::vector<std::string> attachments) {

  Renderer::Viewport viewport = renderer->getCurrentState().viewport;

  return createFBO(name, attachments, viewport.width, viewport.height);
}

FBO* RenderGraphBuilder::createFBO(const std::string& name, std::vector<std::string> attachments, int width, int height)
{
  if (fbos.contains(name)) return fbos[name];

  FBO* fbo = new FBO();

  fbo->setViewport(0, 0, width, height);
  fbo->initialize();
  fbos[name] = fbo;

  for (const auto& attachment : attachments) {
    fbo->attachTexture(textures[attachment]);
  }

  fbo->finalize();
  fbo->addScreenSizeBufferUpdateCallback(renderer);

  Renderer::StateSaver statesaver(*renderer);

  fbo->useViewport();
  fbo->clear();

  return fbo;
}

TextureManager::TextureID RenderGraphBuilder::createScreenSizeTexture(
  const std::string& name, int channels) {

  if (textures.contains(name)) return textures[name];

  Renderer::Viewport viewport = renderer->getCurrentState().viewport;
  return createTexture(name, viewport.width, viewport.height, channels);
}

TextureManager::TextureID RenderGraphBuilder::createTexture(
  const std::string& name, int width, int height, int channels)
{
  if (textures.contains(name)) return textures[name];

  TextureManager::TextureInfo textureInfo = TextureManager::TextureInfo();
  textureInfo.width = width;
  textureInfo.height = height;

  switch (channels)
  {
  case 1:
    textureInfo.format = TEXTURE_R32F;
    break;
  case 3:
    textureInfo.format = TEXTURE_RGB32F;
    break;
  case 4:
    textureInfo.format = TEXTURE_RGBA32F;
    break;
  default:
    textureInfo.format = TEXTURE_RGBA32F;
    break;
  }

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

void RenderGraphBuilder::clearFBOs()
{
  Renderer::StateSaver state(*renderer);

  for (const auto& [name, fbo] : fbos) {
    if (fbo) {
      fbo->useViewport();
      fbo->clear();
    }
  }
}

void RenderGraphBuilder::deleteFBOs() {
  for ( const auto & [name, fbo] : fbos ) {
    if (fbo) delete fbo;
  }

  fbos.clear();
}

void RenderGraphBuilder::deleteTextures()
{
  textures.clear();
}

void RenderGraphBuilder::deleteResources()
{
  deleteFBOs();
  deleteTextures();
}
