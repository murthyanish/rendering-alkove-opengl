/*!****************************************************************************
 * \file   ShadowManager.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-21-2025
 * \brief  Shadow Manager class to handle creation and updation of shadow info
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef SHADOW_MANAGER_H
#define SHADOW_MANAGER_H

#pragma once

#include "Vector3.h"

#include "Lights.h"
#include "ShadowPass.h"
#include "MomentShadowMapPass.h"
#include "RenderGraphBuilder.h"

struct PointLight;

// Later let's look at adding shadows to Directional Lights.
//struct DirectionalLight;

class ShadowManager {
public:
  struct ShadowParams {
    // Could add more info here like Quaternion rotation for which direction 
    // to have it be generated, or a flag to generate a 360 shadow.
    float near = 1;
    float far = 50;
    unsigned int resolution = 1024;
  };
  
  struct ShadowFBOInfo {
     FBO* fbo;
     TextureManager::TextureID textureID;
     TextureManager::TextureID momentTextureID;
  };

  struct ShadowInfo {
    Matrix4 shadowMatrix;
    ShadowFBOInfo fboInfo;
    ShadowParams params;
    Vector3 lastLightPosition;
    int lightIndex;
  };

  ShadowManager() = delete;

  ShadowManager(RenderGraphBuilder* _renderGraphBuilder);

  void update(SceneGraph* scene, Lights* lights);

  int addShadow(
    int lightIndex, PointLight& light, const ShadowParams& params);

  void updateShadow(SceneGraph* scene, ShadowInfo& shadow, const Vector3& position);

  void useShadow(int index, Shader* shader) const;

  void setMomentWidth(int width);
  void updateMomentShadowMap(ShadowInfo& shadow);

private:
  int momentWidth = -1;
  GLuint momentUBO;

  const float alpha = 0.001f;
  std::vector<ShadowInfo> shadows;
  RenderGraphBuilder* renderGraphBuilder;

  ShadowPass shadowPass;
  MomentShadowMapPass momentShadowMapPass;

  void calculateMomentWeights(int width);
};

#endif // !SHADOW_MANAGER_H
