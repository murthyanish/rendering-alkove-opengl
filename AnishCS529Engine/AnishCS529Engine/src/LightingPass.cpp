/*!****************************************************************************
 * \file   LightingPass.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   04-18-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "LightingPass.h"
#include "Renderer.h"

LightingPass::LightingPass() : 
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/lighting.vert\nshaders/lighting.frag");

  setProperty("enableIrradiance", 0);
  setProperty("enableIBLSpecular", 0);
  renderMask = RenderMask::LightingPass;
}

void LightingPass::initialize()
{
  setProperty(
    "GBuffer_position", renderGraphBuilder->getTexture("GBuffer_position"));
  setProperty(
    "GBuffer_normal", renderGraphBuilder->getTexture("GBuffer_normal"));
  setProperty(
    "GBuffer_diffuse", renderGraphBuilder->getTexture("GBuffer_diffuse"));
  setProperty(
    "GBuffer_specular", renderGraphBuilder->getTexture("GBuffer_specular"));
  setProperty(
    "GBuffer_eye_vector", renderGraphBuilder->getTexture("GBuffer_eye_vector"));
  // For testing
  irradianceSpecularPointCount = 40;
  
  createIrradiancePointsUBO(
    irradianceSpecularPointCount,
    generateLowDiscrepancyPoints(irradianceSpecularPointCount));

  setProperty(
    "ambient_factor", renderGraphBuilder->getTexture("filtered_ambient_factor"));
}

void LightingPass::setIrradiancePointsCount(int N) {
  irradianceSpecularPointCount = N;
}

std::vector<float> LightingPass::generateLowDiscrepancyPoints(int N)
{
  std::vector<float> points;
  points.reserve(N * 2 + 2);
  points.push_back(static_cast<float>(N));
  points.push_back(0.0f);

  int kk;
  int pos = 0;
  float p;
  float u;

  for (int k = 0; k < N; k++) {
    for (p = 0.5f, kk = k, u = 0.0f; kk; p *= 0.5f, kk >>= 1)
      if (kk & 1)
        u += p;
    float v = (k + 0.5) / N;
    points.push_back(u);
    points.push_back(v);
  }

  std::cout << "Irradiance points: ";
  for (int i = 2; i < N * 2 + 2; i += 2)
    std::cout << "[" << points[i] << ", " << points[i + 1] << "], ";
  std::cout << std::endl;

  return points;
}

void LightingPass::createIrradiancePointsUBO(
  int N, std::vector<float> points)
{
  glGenBuffers(1, &irradianceSpecularPointCountsUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, irradianceSpecularPointCountsUBO);
  // Binding 4 is dedicated to this now
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, irradianceSpecularPointCountsUBO);
  glBufferData(
    GL_UNIFORM_BUFFER, sizeof(float) * points.size(),
    points.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightingPass::addIrradianceMap(TextureManager::TextureID textureID)
{
  setProperty("enableIrradiance", 1);
  setProperty("IrradianceMap", textureID);
}

void LightingPass::addIrradianceMap(const std::string& texturePath)
{
  setProperty("enableIrradiance", 1);
  auto texture = TextureManager::getInstance().loadFile(texturePath);
  TextureManager::getInstance().setTextureParameters(
    texture, TextureManager::TextureParameters());
  setProperty("IrradianceMap", texture);
}

void LightingPass::addSkydome(TextureManager::TextureID textureID)
{
  setProperty("enableIBLSpecular", 1);
  setProperty("skydome", textureID);
}

void LightingPass::addSkydome(const std::string& texturePath)
{
  setProperty("enableIBLSpecular", 1);
  auto texture = TextureManager::getInstance().loadFile(texturePath);
  TextureManager::getInstance().setTextureParameters(
    texture, TextureManager::TextureParameters());
  setProperty("skydome", texture);
}

void LightingPass::draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const {

  shader->use();

  applyProperties();

  const Renderer::Viewport viewport = 
    renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  shader->setUInt("height", viewport.height);
  shader->setUInt("width", viewport.width);

  screenQuad->draw(GL_TRIANGLES);
}
