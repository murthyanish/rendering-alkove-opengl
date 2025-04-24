/*!****************************************************************************
 * \file   LightingPass.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   02-10-2025
 * \brief  Lighting RenderPass class for 3D rendering
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef LIGHTING_PASS_H
#define LIGHTING_PASS_H

#pragma once

#include "RenderPass.h"

class LightingPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> screenQuad;
  GLuint irradianceSpecularPointCountsUBO;
  int irradianceSpecularPointCount = 10;
  
  std::vector<float> generateLowDiscrepancyPoints(int N = 10);
  void createIrradiancePointsUBO(int N, std::vector<float> points);

public:
  LightingPass();

  void initialize();

  void setIrradiancePointsCount(int N);
  
  void addIrradianceMap(TextureManager::TextureID textureID);
  void addIrradianceMap(const std::string& texturePath);
  void addSkydome(TextureManager::TextureID textureID);
  void addSkydome(const std::string& texturePath);

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const override;
};

#endif // !LIGHTING_PASS_H
