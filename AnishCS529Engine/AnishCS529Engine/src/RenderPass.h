/*!****************************************************************************
 * \file   RenderPass.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    CS529
 * \date   01-15-2025
 * \brief  Render Pass class
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#pragma once

#include <variant>
#include <optional>

#include "Matrix4.h"
#include "ShaderManager.h"
#include "Vector3.h"
#include "TextureManager.h"
#include "VectorTemplated.h"
#include "Mesh.h"
#include "SceneGraph.h"
#include "RenderFlags.h"
#include "RenderGraphBuilder.h"

#define CHECKERROR(msg) { \
    GLenum err; \
    while ((err = glGetError()) != GL_NO_ERROR) { \
        fprintf(stderr, "OpenGL error at %s:%d - %s (%d)\n", __FILE__, __LINE__, msg, err); \
    } \
}

/*!****************************************************************************
 * \brief Render Pass class used to render using a shader
 * 
 * ## Usage:
 * 
 * This class is meant to be used to render using a shader. Each shading pass
 * for rendering should be handled using a RenderPass.
 * 
 * Typically after the shader and resources are set, this class should be
 * registered to the RenderGraph, which will handle rendering.
 * 
 * ## Pre-registration calls:
 * 
 * - setResource(const std::string& name, void* value)
 * 
 * If FBO functionality is needed,
 * - setFBO(std::shared_ptr<FBO> FBO)
 * 
 * ## Explanation:
 * 
 * The class provides an interface for the Shader class. It stores the required
 * resources to run the specific pass, and applies them when apply() is run.
 * 
 * FBO functionality is supported if needed, and is diabled by default unless
 * the setFBO() call is used.
 * 
 *****************************************************************************/
class RenderPass : public std::enable_shared_from_this<RenderPass> {
public:
  using PropertyMap = std::unordered_map<
    std::string,
    std::variant<
      unsigned int, int, float, VectorTemplated<float, 2>, Vector3, Matrix4, TextureManager::TextureID>
  >;

  struct ComputeDispatchSize {
    unsigned int x;
    unsigned int y;
    unsigned int z;

    ComputeDispatchSize(unsigned int s) : x(s), y(s), z(s) {};
    ComputeDispatchSize(unsigned int _x, unsigned int _y, unsigned int _z) : x(_x), y(_y), z(_z) {};
  };

  RenderPass() = default;
  virtual ~RenderPass() = default;

  virtual void initialize() {};

  void setRenderGraphBuilder(
    RenderGraphBuilder* _renderGraphBuilder) {
    renderGraphBuilder = _renderGraphBuilder;
  }

  std::shared_ptr<Shader> addShader(std::string shaderFile);
  std::shared_ptr<Shader> addShader(std::shared_ptr<Shader> _shader);
  std::shared_ptr<Shader> getShader() const;

  template<typename T>
  void setProperty(const std::string& name, const T& value) {
    properties[name] = value;
  }

  template<typename T>
  T getProperty(const std::string& name) {
    return properties[name];
  }

  //void setFBO(std::shared_ptr<FBO> FBO);
  //std::shared_ptr<FBO> getFBO() const;

  void setTexture(const std::string& name, TextureManager::TextureID textureID);

  void setDrawMode(GLenum _drawMode);

  GLenum getDrawMode();

  uint32_t getRenderMask() { return renderMask; }

  virtual void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* sceneGraph) const;

  /* Utility functions */
  void enable();
  void disable();
  bool isEnabled() const;

  /* Compute specific */
  virtual void compute(
      ComputeDispatchSize dispatchSize);
  virtual void compute(
      ComputeDispatchSize dispatchSize, std::shared_ptr<Shader> shader);

protected:
  bool enabled;
  std::shared_ptr<Shader> shader;
  PropertyMap properties;
  GLenum drawMode = NULL;

  uint32_t renderMask = RenderMask::None;

  RenderGraphBuilder* renderGraphBuilder;

  //std::optional<std::shared_ptr<FBO>> FBO;

  std::optional<std::unordered_map<std::string, TextureManager::TextureID>> textureData;

  void applyProperties() const;
  void applyProperties(std::shared_ptr<Shader> shader) const;
  void unbindTextures() const;
  void unbindTextures(std::shared_ptr<Shader> shader) const;

  // Todo: turn this into per point light.
  //void applyLights(Lights lights) const;

};

#endif // RENDER_PASS_H
