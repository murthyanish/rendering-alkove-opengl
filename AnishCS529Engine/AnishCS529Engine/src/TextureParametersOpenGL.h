/*!****************************************************************************
 * \file   TextureParametersOpenGL.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   02-08-2025
 * \brief  Texture Parameters for OpenGL support
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef TEXTURE_PARAMETERS_H
#define TEXTURE_PARAMETERS_H

#pragma once

#ifndef ENGINE_GRAPHICS_LIBRARY_H
#define ENGINE_GRAPHICS_LIBRARY_H
#include <glad/glad.h>
#endif // !ENGINE_GRAPHICS_LIBRARY_H


namespace Texture {
  using Format = GLint;
  using FilterMode = GLint;
  using WrapMode = GLint;
}

constexpr Texture::Format TEXTURE_RGBA = static_cast<GLint>(GL_RGBA);
constexpr Texture::Format TEXTURE_RGBA32F = static_cast<GLint>(GL_RGBA32F);
constexpr Texture::Format TEXTURE_RGB = static_cast<GLint>(GL_RGB);
constexpr Texture::Format TEXTURE_RGB32F = static_cast<GLint>(GL_RGB32F);
constexpr Texture::Format TEXTURE_RED = static_cast<GLint>(GL_RED);
constexpr Texture::Format TEXTURE_R32F = static_cast<GLint>(GL_R32F);

constexpr Texture::FilterMode TEXTURE_NEAREST = static_cast<GLint>(GL_NEAREST);
constexpr Texture::FilterMode TEXTURE_LINEAR = static_cast<GLint>(GL_LINEAR);
constexpr Texture::FilterMode TEXTURE_NEAREST_MIPMAP_NEAREST = static_cast<GLint>(GL_NEAREST_MIPMAP_NEAREST);
constexpr Texture::FilterMode TEXTURE_NEAREST_MIPMAP_LINEAR = static_cast<GLint>(GL_NEAREST_MIPMAP_LINEAR);
constexpr Texture::FilterMode TEXTURE_LINEAR_MIPMAP_NEAREST = static_cast<GLint>(GL_LINEAR_MIPMAP_NEAREST);
constexpr Texture::FilterMode TEXTURE_LINEAR_MIPMAP_LINEAR = static_cast<GLint>(GL_LINEAR_MIPMAP_LINEAR);

constexpr Texture::WrapMode TEXTURE_REPEAT = static_cast<GLint>(GL_REPEAT);
constexpr Texture::WrapMode TEXTURE_MIRRORED_REPEAT = static_cast<GLint>(GL_MIRRORED_REPEAT);
constexpr Texture::WrapMode TEXTURE_CLAMP_TO_EDGE = static_cast<GLint>(GL_CLAMP_TO_EDGE);

constexpr GLenum TEXTURE_2D = GL_TEXTURE_2D;
constexpr GLenum TEXTURE_3D = GL_TEXTURE_3D;

#endif // !TEXTURE_PARAMETERS_H
