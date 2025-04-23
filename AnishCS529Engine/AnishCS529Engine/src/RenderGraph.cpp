/*!****************************************************************************
 * \file   RenderGraph.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   02-10-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "RenderGraph.h"

#include "Renderer.h"
#include "RendererStateSaver.h"

/*!****************************************************************************
 * \brief Initialize the RenderGraph
 * 
 * This is used to initialzie the EBOs for the camera and the lights to reduce
 * uniform updates for each render pass.
 * 
 *****************************************************************************/
void RenderGraph::initialize()
{
  renderGraphBuilder = std::make_shared<RenderGraphBuilder>(renderer);
  shadowManager = std::make_shared<ShadowManager>(renderGraphBuilder.get());
  
  renderGraphBuilder->setShadowManager(shadowManager.get());

  for (size_t i = 0; i < RenderMask::RenderPassOrder.size(); ++i) {
    maskSortIndex[RenderMask::RenderPassOrder[i]] = i;
  }

  initializeLightBuffers();
  initializeCameraUBO();
}

void RenderGraph::shutdown()
{}

void RenderGraph::clear()
{
  renderStack.clear();
  //renderGraphBuilder->clear();
}

void RenderGraph::draw(SceneGraph* scene)
{
  for (const auto& camera : scene->getCameras()) {

    updateCameraUBO(camera);

    // Todo: temporarily using this quick fix so we don't need to always
    // update lights. We should instead keep track of dirty lights and
    // only update those.
    if (!lightsSet) {
      updateLightBuffers(*(scene->getLights()));
      lightsSet = true;
      shadowManager->update(scene, scene->getLights());
    }

    for (const auto& pass : renderStack) {
      // Using state saver so I can reset state after without doing manual effort.
      Renderer::StateSaver savedState(*(renderer));
      pass->draw(camera, scene);
    }
  }
}

/*!****************************************************************************
 * \brief Initialize the Lights UBO
 * 
 * This sets up the UBO for the SunLight and AmbientLight specifically.
 * This allows us to set up the lights only once at the start of the draw loop
 * and then reuse it in all the render passes.
 * 
 * Any shader that has the UBO binding 1 set to "lights" will use these values.
 * 
 *****************************************************************************/
void RenderGraph::initializeLightBuffers()
{
  glGenBuffers(1, &uboLights);
  glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
  glBufferData(
    GL_UNIFORM_BUFFER,
    sizeof(LightingPassLights),
    NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(
    GL_UNIFORM_BUFFER, 1, uboLights, 0,
    sizeof(LightingPassLights));

  glGenBuffers(1, &ssboLights);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboLights);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboLights); // Binding point 2 matches GLSL
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

/*!****************************************************************************
 * \brief Initialize the Camera UBO
 * 
 * This sets up the UBO for the camera's viewProjectionMatrix specifically.
 * This allows us to set up the camera only once at the start of the draw loop
 * and then reuse it in all the render passes.
 * 
 * Any shader that has the UBO binding 0 set to "camera" will use these values.
 * 
 *****************************************************************************/
void RenderGraph::initializeCameraUBO()
{
  glGenBuffers(1, &uboCamera);
  glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
  glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix4) * 3) + sizeof(float), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboCamera, 0, sizeof(Matrix4) * 2);
}

void RenderGraph::updateLightBuffers(const Lights& lights) const
{
  LightingPassLights lpLights = { lights.sunLight, lights.ambientLight };
  glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
  glBufferSubData(GL_UNIFORM_BUFFER,
    0, sizeof(LightingPassLights),
    &lpLights);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboLights);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLight) * lights.pointLights.size(), lights.pointLights.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RenderGraph::updateCameraUBO(std::shared_ptr<Camera> camera) const
{
  glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
  glBufferSubData(GL_UNIFORM_BUFFER,
    0, sizeof(Matrix4),
    camera->getProjectionMatrix().getData());
  glBufferSubData(GL_UNIFORM_BUFFER,
    sizeof(Matrix4), sizeof(Matrix4),
    camera->getViewMatrix().getData());
  glBufferSubData(GL_UNIFORM_BUFFER,
    sizeof(Matrix4) * 2, sizeof(Matrix4),
    camera->getInverseViewMatrix().getData());
  glBufferSubData(GL_UNIFORM_BUFFER,
    sizeof(Matrix4) * 3, sizeof(float),
    &(camera->getExposure()));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
