/*!****************************************************************************
 * \file   ShadowManager.cpp
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-21-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "ShadowManager.h"

#include "Renderer.h"
#include "RendererStateSaver.h"

ShadowManager::ShadowManager(RenderGraphBuilder* _renderGraphBuilder) :
  renderGraphBuilder(_renderGraphBuilder), shadows(), shadowPass(ShadowPass()) {}

void ShadowManager::update(SceneGraph* scene, Lights* lights)
{
  // Later add logic here for directional light, and for
  // updating changed lights

  if (momentWidth == -1) {
    setMomentWidth(5);
  }

  const float epsilon = 0.01f;

  for (int i = 0; i < lights->pointLights.size(); ++i) {
    const PointLight& light = lights->pointLights[i];
    if (light.castShadow) {
      // Init lights if they don't already exist.
      if (light.shadowIndex == -1) {
        const int shadowIndex = addShadow(i, lights->pointLights[i], ShadowParams());

        updateShadow(scene, shadows[shadowIndex], light.position);
        updateMomentShadowMap(shadows[shadowIndex]);
      }
      else {
        // Update shadows if any of the lights have moved.
        const int shadowIndex = light.shadowIndex;
        if ((shadows[shadowIndex].lastLightPosition - light.position).magnitudSquared() > epsilon) {
          updateShadow(scene, shadows[shadowIndex], light.position);
          //updateMomentShadowMap(shadows[shadowIndex]);
        }
      }
    }
  }
}

int ShadowManager::addShadow(
  int lightIndex, PointLight& light, const ShadowParams& params)
{
  light.shadowIndex = static_cast<int>(shadows.size());
  
  // Creating the Shadow FBOs
  ShadowFBOInfo fboInfo = ShadowFBOInfo();
  
  fboInfo.textureID = renderGraphBuilder->createTexture(
    "shadow_" + std::to_string(light.shadowIndex),
    params.resolution, params.resolution);


  fboInfo.fbo = renderGraphBuilder->createFBO(
    "shadow" + std::to_string(light.shadowIndex),
    {
      "shadow_" + std::to_string(light.shadowIndex)
    },
    params.resolution, params.resolution);

  // For now we will always create a Moment Shadow Map.
  renderGraphBuilder->createTexture(
    "moment_shadow_temp",
    params.resolution, params.resolution);

  fboInfo.momentTextureID = renderGraphBuilder->createTexture(
    "moment_shadow_" + std::to_string(light.shadowIndex),
    params.resolution, params.resolution);

  ShadowInfo shadowInfo = {
    Matrix4(),
    fboInfo,
    params,
    light.position,
    lightIndex
  };
  
  shadows.push_back(shadowInfo);
  return light.shadowIndex;
}

void ShadowManager::updateShadow(
  SceneGraph* scene, ShadowInfo& shadow, const Vector3& position)
{
  Renderer::StateSaver savedState(*(renderGraphBuilder->getRenderer()));
  shadow.fboInfo.fbo->bind();
  shadow.fboInfo.fbo->clear();


  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  const Matrix4 viewMatrix = Matrix4::lookAt(position, position - Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
  const Matrix4 projectionMatrix = Matrix4::perspective(
    45.0f * 3.14159f / 180.0f,
    1.0f,
    shadow.params.near,
    shadow.params.far);
  shadowPass.setProperty("ViewMatrix", viewMatrix);
  shadowPass.setProperty("ProjectionMatrix", projectionMatrix);
  // shadowPass.setProperty("ShadowNearPlane", shadow.params.near);
  // shadowPass.setProperty("ShadowFarPlane", shadow.params.far);
  shadowPass.draw(nullptr, scene);

  shadow.shadowMatrix =
    Matrix4::translation(0.5f) * Matrix4::scale(0.5f) *
    projectionMatrix * viewMatrix;

  glDisable(GL_CULL_FACE);

  shadow.fboInfo.fbo->unbind();
}

void ShadowManager::useShadow(int index, Shader* shader) const
{
  /*
  * Note: This is using a hardcoded texture unit.
  * 0-3 are always reserved to the shadows.
  */
  const ShadowInfo shadow = shadows[index];
  shader->bindTexture(0, "ShadowMap", shadow.fboInfo.momentTextureID);

  shader->setMat4("ShadowMatrix", shadow.shadowMatrix);

  // shader->setFloat("ShadowNearPlane", shadow.params.near);
  // shader->setFloat("ShadowFarPlane", shadow.params.far);
}

void ShadowManager::calculateMomentWeights(int width)
{
  std::vector<float> weights;
  weights.reserve(width * 2 + 2);
  weights.push_back(width);

  for (int i = -width; i < width + 1; ++i) {
    weights.push_back(
      expf(
        -(1.0 / 2.0) *
        powf(static_cast<float>(i) / (static_cast<float>(width) / 2.0), 2.0)));
  }

  glGenBuffers(1, &momentUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, momentUBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, 3, momentUBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * weights.size(), weights.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void ShadowManager::setMomentWidth(int width)
{
  momentWidth = width;
  calculateMomentWeights(momentWidth);
}

void ShadowManager::updateMomentShadowMap(ShadowInfo& shadow)
{
  auto tempTexture = renderGraphBuilder->getTexture("moment_shadow_temp");

  glBindImageTexture(
    0, shadow.fboInfo.textureID, 0,
    GL_FALSE, 0,
    GL_READ_ONLY,
    TextureManager::getInstance().getTextureInfo(
      shadow.fboInfo.textureID).format
  );
  
  glBindImageTexture(
    1, tempTexture, 0,
    GL_FALSE, 0,
    GL_WRITE_ONLY,
    TextureManager::getInstance().getTextureInfo(
      tempTexture).format
  );

  momentShadowMapPass.computeHor(RenderPass::ComputeDispatchSize(1024 / 128, 1024 / 1, 1));


  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


  glBindImageTexture(
    0, tempTexture, 0,
    GL_FALSE, 0,
    GL_READ_ONLY,
    TextureManager::getInstance().getTextureInfo(
      tempTexture).format
  );
  
  glBindImageTexture(
    1, shadow.fboInfo.momentTextureID, 0,
    GL_FALSE, 0,
    GL_WRITE_ONLY,
    TextureManager::getInstance().getTextureInfo(
      shadow.fboInfo.momentTextureID).format
  );

  momentShadowMapPass.computeVert(RenderPass::ComputeDispatchSize(1024 / 1, 1024 / 128, 1));
}
