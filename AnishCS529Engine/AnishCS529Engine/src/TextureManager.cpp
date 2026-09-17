/*!****************************************************************************
 * \file   TextureManager.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Gary Herron (Professor)
 *   (gherron@digipen.edu) (Advising and Base Code)
 * \date   02-08-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureID TextureManager::loadFile(const std::string& filePath)
{
  /* Checking if Texture was already loaded and returning it. */
  if (loadedFiles.contains(filePath)) {
    return loadedFiles[filePath];
  }

  if (stbi_is_hdr(filePath.c_str())) {
    return loadHDRFile(filePath);
  }
  else {
    return  loadSDRFile(filePath);
  }
}

TextureManager::TextureID TextureManager::createTexture(
  const unsigned int width, const unsigned int height,
  Texture::Format internal_format, const void* data,
  TextureParameters textureParameters)
{
  TextureInfo textureInfo = TextureInfo();
  textureInfo.width = width;
  textureInfo.height = height;
  textureInfo.format = internal_format;

  return createOpenGLTexture(textureInfo, data, textureParameters);
}

TextureManager::TextureID TextureManager::createTexture(
  const TextureInfo& textureInfo, const void * data,
  TextureParameters textureParameters)
{
  return createOpenGLTexture(textureInfo, data, textureParameters);
}

void TextureManager::resizeTexture(TextureID id, int width, int height)
{
  /*assert(
    (std::find(textures.begin(), textures.end(), id) != textures.end()) &&
    "ERROR::TEXTUREMANAGER::RESIZETEXTURE::INVALIDTEXTURE");*/

  glBindTexture(TEXTURE_2D, id.id);
  textures[id].width = width;
  textures[id].height = height;

  Texture::Format format;
  if (textures[id].format == TEXTURE_RGB || textures[id].format == TEXTURE_RGB32F)
    format = TEXTURE_RGB;
  else if (textures[id].format == TEXTURE_RED || textures[id].format == TEXTURE_R32F)
    format = TEXTURE_RED;
  else if (textures[id].format == TEXTURE_RGBA || textures[id].format == TEXTURE_RGBA32F)
    format = TEXTURE_RGBA;
  else {
    std::cout << "WARNING::TEXTUREMANAGER::CREATETEXTURE::INVALIDFORMAT"
      << "::Defaulting to TEXTURE_RGBA" << std::endl;
    format = TEXTURE_RGBA;
  }

  glTexImage2D(
    TEXTURE_2D, 0, textures[id].format, textures[id].width, textures[id].height,
    0, format, GL_UNSIGNED_BYTE, nullptr);

  glBindTexture(TEXTURE_2D, 0);
}

void TextureManager::setTextureParameters(TextureID id, TextureParameters textureParameters)
{
  glBindTexture(TEXTURE_2D, id.id);
  setTextureParameters(textureParameters);
  glBindTexture(TEXTURE_2D, 0);
}

bool TextureManager::isHDR(TextureID id)
{
  return (textures[id].format == TEXTURE_RGBA32F);
}

const TextureManager::TextureInfo& TextureManager::getTextureInfo(TextureManager::TextureID id)
{
  return textures[id];
}

TextureManager::TextureID TextureManager::loadHDRFile(const std::string& filepath)
{
  TextureParameters textureParameters = TextureParameters(10, 
    TEXTURE_NEAREST, TEXTURE_LINEAR_MIPMAP_LINEAR,
    TEXTURE_REPEAT, TEXTURE_REPEAT);

  TextureInfo textureInfo = TextureInfo();

  stbi_set_flip_vertically_on_load(true);
  int channels;
  float* data = stbi_loadf(
    filepath.c_str(), &(textureInfo.width), &(textureInfo.height), &channels, 4);

  assert(("ERROR::TEXTURE::LOADTEXTURE::LOADFAILED::" + filepath).c_str() && data);

  textureInfo.format = TEXTURE_RGBA32F;

  TextureID id = createOpenGLTexture(textureInfo, data, textureParameters, GL_FLOAT);

  stbi_image_free(data);

  return id;
}

TextureManager::TextureID TextureManager::loadSDRFile(const std::string& filepath)
{
  TextureParameters textureParameters = TextureParameters( 10,
    TEXTURE_NEAREST, TEXTURE_LINEAR_MIPMAP_LINEAR,
    TEXTURE_REPEAT, TEXTURE_REPEAT);

  TextureInfo textureInfo = TextureInfo();

  stbi_set_flip_vertically_on_load(true);
  int channels;
  unsigned char* data = stbi_load(
    filepath.c_str(), &(textureInfo.width), &(textureInfo.height), &channels, 0);

  assert(("ERROR::TEXTURE::LOADTEXTURE::LOADFAILED::" + filepath).c_str() && data);

  if (channels == 3)
    textureInfo.format = TEXTURE_RGB;
  else if (channels == 4)
    textureInfo.format = TEXTURE_RGBA;

  TextureID id = createOpenGLTexture(textureInfo, data, textureParameters);

  stbi_image_free(data);

  return id;
}

void TextureManager::generateMipmaps()
{
  glGenerateMipmap(TEXTURE_2D);
}

void TextureManager::setTextureParameters(TextureParameters textureParameters)
{
  glTexParameteri(TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, textureParameters.max_level);
  glTexParameteri(TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureParameters.mag_filter);
  glTexParameteri(TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureParameters.min_filter);

  glTexParameteri(TEXTURE_2D, GL_TEXTURE_WRAP_S, textureParameters.wrap_s);
  glTexParameteri(TEXTURE_2D, GL_TEXTURE_WRAP_T, textureParameters.wrap_t);

  if (
    textureParameters.min_filter == TEXTURE_NEAREST_MIPMAP_NEAREST ||
    textureParameters.min_filter == TEXTURE_NEAREST_MIPMAP_LINEAR ||
    textureParameters.min_filter == TEXTURE_LINEAR_MIPMAP_NEAREST ||
    textureParameters.min_filter == TEXTURE_LINEAR_MIPMAP_LINEAR
    ) {
    generateMipmaps();
  }
}

TextureManager::TextureID TextureManager::createOpenGLTexture(
  const TextureInfo& textureInfo, const void* data,
  TextureParameters textureParameters, GLenum dataFormat)
{
  TextureID id = TextureID();
  glGenTextures(1, &(id.id));

  textures[id] = textureInfo;

  glBindTexture(TEXTURE_2D, id);

  Texture::Format format;
  if (textureInfo.format == TEXTURE_RGB || textureInfo.format == TEXTURE_RGB32F)
    format = TEXTURE_RGB;
  else if (textures[id].format == TEXTURE_RED || textures[id].format == TEXTURE_R32F)
    format = TEXTURE_RED;
  else if (textureInfo.format == TEXTURE_RGBA || textureInfo.format == TEXTURE_RGBA32F)
     format = TEXTURE_RGBA;
  else {
    std::cout << "WARNING::TEXTUREMANAGER::CREATETEXTURE::INVALIDFORMAT" 
      << "::Defaulting to TEXTURE_RGBA" << std::endl;
    format = TEXTURE_RGBA;
  }

  glTexImage2D(
    TEXTURE_2D, 0, textureInfo.format, textureInfo.width, textureInfo.height,
    0, format, dataFormat, data);

  setTextureParameters(textureParameters);

  glBindTexture(TEXTURE_2D, 0);

  return id;
}
