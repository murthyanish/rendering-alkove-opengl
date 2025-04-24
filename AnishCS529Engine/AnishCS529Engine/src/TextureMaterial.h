/*!****************************************************************************
 * \file   TextureMaterial.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   02-10-2025
 * \brief  Texture Material to draw basic textures
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef TEXTURE_MATERIAL_H
#define TEXTURE_MATERIAL_H

#pragma once

#include "Material.h"

class TextureMaterial : public Material {
public:
  TextureMaterial() {
    // 0b0001 is the renderMask for the default lighting pass
    defaultRenderMask = RenderMask::DefaultLit;
    setProperty("useTexture", 1);
    setProperty("HDR", 0);

    // Temporary because I didn't have time to fix skydome pass
    setProperty("skydome", 0);
  };

  TextureMaterial(
    const std::string& textureFile,
    float scaleX = 1.0f, float scaleY = 1.0f) : Material() {
    // 0b0001 is the renderMask for the default lighting pass
    defaultRenderMask = RenderMask::DefaultLit;

    addTexture(textureFile, scaleX, scaleY);
  }

  TextureMaterial(
    TextureManager::TextureID textureID,
    float scaleX = 1.0f, float scaleY = 1.0f) : Material() {
    // 0b0001 is the renderMask for the default lighting pass
    defaultRenderMask = RenderMask::DefaultLit;

    addTexture(textureID, scaleX, scaleY);
  }

  TextureManager::TextureID addTexture(
    const std::string& textureFile, float scaleX = 1.0f, float scaleY = 1.0f);
  TextureManager::TextureID addTexture(
    TextureManager::TextureID textureID, float scaleX = 1.0f, float scaleY = 1.0f);

  TextureManager::TextureID addNormalMap(
    const std::string& textureFile,
    float scaleX = 1.0f, float scaleY = 1.0f);
  TextureManager::TextureID addNormalMap(
    TextureManager::TextureID textureID,
    float scaleX = 1.0f, float scaleY = 1.0f);

  void addDefaultTexture();
private:
  static const std::string defaultTextureFile;

};

#endif // !TEXTURE_MATERIAL_H
