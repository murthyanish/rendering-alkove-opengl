/*!****************************************************************************
 * \file   RenderGraph.inl
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   02-10-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef RENDER_GRAPH_INL
#define RENDER_GRAPH_INL

#pragma once


template <typename T>
std::shared_ptr<T> RenderGraph::addPass() {
  static_assert(std::is_base_of<RenderPass, T>::value,
    "Pass must be derived from RenderPass class");

  std::type_index type = typeid(T);
  if (addedPassTypes.count(type) > 0) {
    // Already added, ignore second call
    return nullptr;
  }

  auto pass = std::make_shared<T>();
  pass->setRenderGraphBuilder(renderGraphBuilder.get());
  pass->initialize();
  addedPassTypes.insert(type);
  renderStack.push_back(pass);

  std::sort(renderStack.begin(), renderStack.end(),
    [&](const std::shared_ptr<RenderPass>& a, const std::shared_ptr<RenderPass>& b) {
      return maskSortIndex[a->getRenderMask()] < maskSortIndex[b->getRenderMask()];
    });

  return pass;
}


template <typename T>
std::shared_ptr<T> RenderGraph::addPass(std::shared_ptr<T> pass) {
  std::type_index type = typeid(T);
  if (addedPassTypes.count(type) > 0) {
    // Already added, ignore second call
    return nullptr;
  }

  pass->setRenderGraphBuilder(renderGraphBuilder);
  pass->initialize();
  renderStack.push_back(pass);
  addedPassTypes.insert(type);

  std::sort(renderStack.begin(), renderStack.end(),
    [&](const std::shared_ptr<RenderPass>& a, const std::shared_ptr<RenderPass>& b) {
      return maskSortIndex[a->getRenderMask()] < maskSortIndex[b->getRenderMask()];
    });

  return pass;
}

template<typename T>
std::shared_ptr<T> RenderGraph::getPass()
{
  static_assert(std::is_base_of<RenderPass, T>::value,
    "Pass must be derived from RenderPass class");

  for (const auto& pass : renderStack) {
    if (auto specificPass = std::dynamic_pointer_cast<T>(pass)) {
      return specificPass;
    }
  }
  return nullptr;
}

template<typename T>
void RenderGraph::removePass()
{
  static_assert(std::is_base_of<RenderPass, T>::value,
     "Pass must be derived from RenderPass class");

  auto it = std::find_if(renderStack.begin(), renderStack.end(),
        [](const std::shared_ptr<RenderPass>& pass) {
          return dynamic_cast<T*>(pass.get()) != nullptr;
        });

  if (it != renderStack.end()) {
    renderStack.erase(it);
  }
}

#endif // !RENDER_PASS_INL
