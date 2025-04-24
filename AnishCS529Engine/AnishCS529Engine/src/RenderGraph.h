/*!****************************************************************************
 * \file   RenderGraph.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    CS529
 * \date   01-16-2025
 * \brief  RenderGraph class
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef RENDER_GRAPH_H
#define RENDER_GRAPH_H

#pragma once

#include <typeindex>

#include "Mesh.h"
#include "SceneGraph.h"
#include "RenderPass.h"
#include "RenderGraphBuilder.h"
#include "ShadowManager.h"

class Renderer;
class GameWindow;

class RenderGraph {
public:
  RenderGraph() = default;

  void initialize();
  void shutdown();

  template <typename T>
  std::shared_ptr<T> addPass();
  template <typename T>
  std::shared_ptr<T> addPass(std::shared_ptr<T> pass);
  template <typename T>
  std::shared_ptr<T> getPass();
  template <typename T>
  void removePass();

  void clear();

  void setRenderer(Renderer* _renderer) { renderer = _renderer; }

  Renderer* getRenderer() { return renderer; }

  std::shared_ptr<RenderGraphBuilder> getRenderGraphBuilder() { return renderGraphBuilder; }

  void draw(SceneGraph* scene);

  bool lightsSet = false;
private:
  Renderer* renderer;
  std::shared_ptr<RenderGraphBuilder> renderGraphBuilder;

  std::shared_ptr<ShadowManager> shadowManager;

  std::vector<std::shared_ptr<RenderPass>> renderStack;
  unsigned int uboLights;
  unsigned int ssboLights;
  // Quick fix to stop repeated light updates, this SHOULD be removed.
  unsigned int uboCamera;

  std::unordered_map<uint32_t, size_t> maskSortIndex;
  std::unordered_set<std::type_index> addedPassTypes;

  void initializeLightBuffers();
  void initializeCameraUBO();

  void updateLightBuffers(const Lights& lights) const;
  void updateCameraUBO(std::shared_ptr<Camera> camera) const;
};

#include "RenderGraph.inl"

#endif // !RENDER_GRAPH_H
