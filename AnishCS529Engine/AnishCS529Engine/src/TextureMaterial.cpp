/*!****************************************************************************
 * \file   TextureMaterial.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   02-10-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "TextureMaterial.h"

const std::string TextureMaterial::defaultTextureFile = "media/FactoryMaterial.bmp";

TextureManager::TextureID TextureMaterial::addTexture(
  const std::string& textureFile,
  float scaleX, float scaleY)
{
  TextureManager::TextureID texture = TextureManager::getInstance().loadFile(textureFile);
  this->setProperty("mainTexture", texture);
  this->setProperty("mainTextureScale", 
    VectorTemplated<float, 2>({ scaleX, scaleY }));
  this->setProperty("HDR", TextureManager::getInstance().isHDR(texture));

  this->setProperty("useNormalMap", false);
  return texture;
}

TextureManager::TextureID TextureMaterial::addTexture(
  TextureManager::TextureID textureID,
  float scaleX, float scaleY)
{
  this->setProperty("mainTexture", textureID);
  this->setProperty("mainTextureScale",
    VectorTemplated<float, 2>({ scaleX, scaleY }));
  this->setProperty("HDR", TextureManager::getInstance().isHDR(textureID));

  this->setProperty("useNormalMap", false);
  return textureID;
}

TextureManager::TextureID TextureMaterial::addNormalMap(
  const std::string& textureFile,
  float scaleX, float scaleY)
{
  TextureManager::TextureID texture = TextureManager::getInstance().loadFile(textureFile);
  this->setProperty("normalMap", texture);
  this->setProperty("normalMapScale",
    VectorTemplated<float, 2>({ scaleX, scaleY }));
  this->setProperty("useNormalMap", true);
  return texture;
}

TextureManager::TextureID TextureMaterial::addNormalMap(
  TextureManager::TextureID textureID,
  float scaleX, float scaleY)
{
  this->setProperty("normalMap", textureID);
  this->setProperty("normalMapScale",
    VectorTemplated<float, 2>({ scaleX, scaleY }));
  this->setProperty("useNormalMap", true);
  return textureID;
}

void TextureMaterial::addDefaultTexture()
{
  addTexture("media/FactoryMaterial.bmp", 1.0f, 1.0f);
}
