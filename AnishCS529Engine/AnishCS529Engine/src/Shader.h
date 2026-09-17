/*!****************************************************************************
 * \file   Shader.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * \brief  Shader class to create and manage shaders and set up
 * uniforms.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef SHADER_H
#define SHADER_H

#pragma once

#include <glad/glad.h>

#ifndef ENGINE_GLFW_H
#define ENGINE_GLFW_H
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif // ENGINE_GLFW_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Matrix4.h"
#include "Vector3.h"
#include "TextureManager.h"

class Renderer;

class Shader {
public:
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  ~Shader();

  void use();
  void unuse();

  /* Todo: figure out if we can make this generic using std::variant? */
  void setUInt(const std::string& name, unsigned int value) const;
  void setInt(const std::string& name, int value) const;
  void set2Int(const std::string& name, int v0, int v1) const;
  void setFloat(const std::string& name, float value) const;
  void setVec2(const std::string& name, float x, float y) const;
  void setVec3(const std::string& name, const Vector3& value) const;
  void setVec4(const std::string& name, float x, float y, float z, float w) const;
  void setMat4(const std::string& name, const Matrix4& value) const;

  void bindTexture(
    unsigned int textureUnit,
    const std::string& name,
    TextureManager::TextureID textureID) const;
  void unbindTexture(
    unsigned int textureUnit) const;

  // Image Textures
  void bindImageTexture(
    unsigned int bindingUnit,
    TextureManager::TextureID textureID,
    GLenum access) const;

  unsigned int getId() { return programID; };

  void setDrawMode(GLenum _drawMode);
  GLenum getDrawMode();

  void initializeUBO(const std::string& name, unsigned int blockBinding);
  
private:
  Shader(const std::string& shaderFilePaths);

  std::unordered_map<GLenum, std::vector<GLuint>> shaderIDs;
  mutable std::unordered_map<std::string, GLint> uniformLocationCache; // Todo: Why is this mutable?
  GLuint programID;
  GLenum drawMode = 0;

  GLenum readShaderType(std::string path);
  std::string shaderTypeToString(GLenum type);
  std::string readShaderFile(std::string path);
  GLuint loadShader(GLenum type, const GLchar* source);
  void deleteShader(GLuint id);
  void linkShaders();
  void cacheUniforms();
  GLint getUniformLocation(const std::string& name) const;

  std::string toLowerCase(const std::string& str);

  friend class ShaderManager;
};

#endif // SHADER_H
