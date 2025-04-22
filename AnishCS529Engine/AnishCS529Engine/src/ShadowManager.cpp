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
          updateMomentShadowMap(shadows[shadowIndex]);
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

  const Matrix4 viewMatrix = Matrix4::lookAt(position, position - Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f));
  const Matrix4 projectionMatrix = Matrix4::perspective(
    45.0f * 3.14159f / 180.0f,
    1.0f,
    shadow.params.near,
    shadow.params.far);
  shadowPass.setProperty("ViewMatrix", viewMatrix);
  shadowPass.setProperty("ProjectionMatrix", projectionMatrix);
  shadowPass.draw(nullptr, scene);

  shadow.shadowMatrix =
    Matrix4::translation(0.5f) * Matrix4::scale(0.5f) *
    projectionMatrix * viewMatrix;

  shadow.fboInfo.fbo->unbind();
}

void ShadowManager::useShadow(int index, Shader* shader) const
{
  /*
  * Note: This is using a hardcoded texture unit.
  * 0-3 are always reserved to the shadows.
  */
  const ShadowInfo shadow = shadows[index];
  shader->bindTexture(0, "ShadowMap", shadow.fboInfo.textureID);

  shader->setMat4("ShadowMatrix", shadow.shadowMatrix);
}

void ShadowManager::updateMomentShadowMap(ShadowInfo& shadow)
{
  glBindImageTexture(
    0, shadow.fboInfo.momentTextureID, 0,
    GL_FALSE, 0,
    GL_WRITE_ONLY,
    TextureManager::getInstance().getTextureInfo(
      shadow.fboInfo.momentTextureID).format
  );

  momentShadowMapPass.setProperty("ShadowMap", shadow.fboInfo.textureID);
  momentShadowMapPass.setProperty("alpha", alpha);
  momentShadowMapPass.compute(shadow.params.resolution);
}
