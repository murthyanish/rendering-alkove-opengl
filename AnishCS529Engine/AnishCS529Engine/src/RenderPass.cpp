/*!****************************************************************************
 * \file   RenderPass.cpp
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
#include "RenderPass.h"

std::shared_ptr<Shader> RenderPass::addShader(std::string shaderFile)
{
  shader = ShaderManager::getInstance().loadFile(shaderFile);
  return shader;
}

std::shared_ptr<Shader> RenderPass::addShader(std::shared_ptr<Shader> _shader)
{
  shader = _shader;
  return shader;
}

std::shared_ptr<Shader> RenderPass::getShader() const
{
  return shader;
}

void RenderPass::setTexture(const std::string& name, TextureManager::TextureID textureID)
{
  if (!textureData) {
    textureData = std::unordered_map<std::string, TextureManager::TextureID>();
  }
  (*textureData)[name] = textureID;
}

void RenderPass::setDrawMode(GLenum _drawMode)
{
  drawMode = _drawMode;
}

GLenum RenderPass::getDrawMode()
{
  return drawMode;
}

void RenderPass::draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* sceneGraph) const
{
  shader->use();
  applyProperties();

  shader->setDrawMode(drawMode);
  sceneGraph->draw(shader, renderMask);
  shader->setDrawMode(NULL);

  unbindTextures();
  shader->unuse();

}

void RenderPass::applyProperties() const
{
  /* Todo: this is VERY inefficient. We don't need to be using variant
  * and doing TWO loops over the same variant data is really bad for
  * performance. Please fix this when you get time.
  * 
  * Also a nightmare to manage. I forgot to add Vector2 to one and
  * took half an hour to realise.
  */

  /*
  * Texture Units 0-9 are reserved.
  * Here's a list of the reserved textures:
  * 0 - shadow
  */

  unsigned int textureUnit = 10;

  for (const auto& [name, value] : properties) {
    if (auto item = std::get_if<unsigned int>(&value)) {
      shader->setUInt(name, *item);
    }
    else if (auto item = std::get_if<int>(&value)) {
      shader->setInt(name, *item);
    }
    else if (auto item = std::get_if<float>(&value)) {
      shader->setFloat(name, *item);
    }
    else if (auto item = std::get_if<VectorTemplated<float, 2>>(&value)) {
      shader->setVec2(name, (*item)[0], (*item)[1]);
    }
    else if (auto item = std::get_if<Vector3>(&value)) {
      shader->setVec3(name, *item);
    }
    else if (auto item = std::get_if<Matrix4>(&value)) {
      shader->setMat4(name, *item);
    }
    else if (auto item = std::get_if<TextureManager::TextureID>(&value)) {
      shader->bindTexture(textureUnit, name, *item);
      ++textureUnit;
    }
  }
}

void RenderPass::unbindTextures() const
{
  /* Todo: this is also very unefficient, we shouldn't
  * iterate every property just to unbind textures.
  */

  unsigned int textureUnit = 10;

  for (const auto& [name, value] : properties) {
    if (auto item = std::get_if<TextureManager::TextureID>(&value)) {
      shader->unbindTexture(textureUnit);
      ++textureUnit;
    }
  }
}

/*!****************************************************************************
 * \brief Enable the Pass
 *
 *****************************************************************************/
void RenderPass::enable() {
  enabled = true;
}

/*!****************************************************************************
 * \brief Disable the Pass
 *
 *****************************************************************************/
void RenderPass::disable() {
  enabled = false;
}

/*!****************************************************************************
 * \brief Is Pass enabled
 *
 * \return \b bool true if Pass enabled.
 *****************************************************************************/
bool RenderPass::isEnabled() const {
  return enabled;
}
