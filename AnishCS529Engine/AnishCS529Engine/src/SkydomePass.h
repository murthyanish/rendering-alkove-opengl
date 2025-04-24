/*!****************************************************************************
 * \file   SkydomePass.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   04-06-2025
 * \brief  Skydome Pass for rendering
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef SKYDOME_PASS_H
#define SKYDOME_PASS_H

#pragma once

#include "RenderPass.h"
#include "TextureManager.h"

class SkydomePass : public RenderPass {
private:
  TextureManager::TextureID skydomeTexture;
  bool HDR;
  std::shared_ptr<Mesh> sphereMesh;
  Matrix4 scale;
  FBO* gbuffer;

public:
  SkydomePass();

  void addTexture(TextureManager::TextureID textureID);
  void addTexture(const std::string& textureName);

  void initialize() override;

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* sceneGraph) const override;
};

#endif // !SKYDOME_PASS_H
