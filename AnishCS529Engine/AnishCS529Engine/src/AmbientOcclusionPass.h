/*!****************************************************************************
 * \file   AmbientOcclusionPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-30-2025
 * \brief  Ambient Occlusion functionality using Alchemy AO
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef AMBIENT_OCCLUSION_PASS_H
#define AMBIENT_OCCLUSION_PASS_H

#pragma once

#include "RenderPass.h"

class AmbientOcclusionPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> screenQuad;
  int ambientOcclusionPointsCount = 10;

  FBO* aoBuffer;

  int filterWidth = -1;
  GLuint aoFilterUBO;

  std::shared_ptr<Shader> compShaderBilateralHor;
  std::shared_ptr<Shader> compShaderBilateralVert;

  TextureManager::TextureID ambientFactor;
  TextureManager::TextureID ambientFactorTemp;
  TextureManager::TextureID filteredAmbientFactor;

  void calculateFilterWeights(int width);

public:
  AmbientOcclusionPass();

  void initialize();

  void setAmbientOcclusionPointsCount(int N);

  void setFilterWidth(int width);

  void drawAmbientOcclusionTexture() const;

  void computeBilateralBlurFilter(int width, int height) const;

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const override;
};

#endif // !AMBIENT_OCCLUSION_PASS_H
