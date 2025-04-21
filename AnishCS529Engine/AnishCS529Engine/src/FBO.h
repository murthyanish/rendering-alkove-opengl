/*!****************************************************************************
 * \file   FBO.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   04-09-2025
 * \brief  A slight encapsulation of a Frame Buffer Object (i'e' Render
 * Target) and its associated texture.  When the FBO is "Bound", the
 * output of the graphics pipeline is captured into the texture.  When
 * it is "Unbound", the texture is available for use as any normal
 * texture.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef FBO_H
#define FBO_H

#pragma once

#include "TextureManager.h"

class Renderer;

class FBO {
public:
  struct Viewport {
    int x;
    int y;
    int width;
    int height;
  };

  unsigned int fboID;
  std::vector<unsigned int> textureIDs;
  Viewport viewport;  // Size of the Buffer
  GLuint depthBuffer;
  unsigned int attachedTextures = 0;

  ~FBO() = default;

  void initialize();
  void finalize();

  void setViewport(
    const int _x, const int _y,
    const int _width, const int _height);

  void updateTextures();
  void updateViewport(
    const int _x, const int _y,
    const int _width, const int _height);

  void addScreenSizeBufferUpdateCallback(Renderer* renderer);

  void attachTexture(TextureManager::TextureID textureID);

  // Don't use this unless you have cached the screen viewport.
  void useViewport();

  // Bind this FBO to receive the output of the graphics pipeline.
  void bind();

  // Unbind this FBO from the graphics pipeline;  graphics goes to screen by default.
  void unbind();
  void clear();
};

#endif // !FBO_H
