/*!****************************************************************************
 * \file   AmbientOcclusionPass.cpp
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-30-2025
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "AmbientOcclusionPass.h"
#include "Renderer.h"

AmbientOcclusionPass::AmbientOcclusionPass() :
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/ambient_occlusion.vert\nshaders/ambient_occlusion.frag");

  compShaderBilateralHor = ShaderManager::getInstance().loadFile("shaders/bilateral_blur_hor.comp");
  compShaderBilateralVert = ShaderManager::getInstance().loadFile("shaders/bilateral_blur_vert.comp");

  renderMask = RenderMask::AmbientOcclusionPass;

  filterWidth = 10;
}

void AmbientOcclusionPass::initialize()
{
  ambientFactor = renderGraphBuilder->createScreenSizeTexture("ambient_factor", 1);
  aoBuffer = renderGraphBuilder->createScreenSizeFBO(
    "aoBuffer",
    {
      "ambient_factor"
    });

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
  ambientOcclusionPointsCount = 10;

  ambientFactorTemp = renderGraphBuilder->createScreenSizeTexture("ambient_factor_temp", 1);
  filteredAmbientFactor = renderGraphBuilder->createScreenSizeTexture("filtered_ambient_factor", 1);

  calculateFilterWeights(filterWidth);
}

void AmbientOcclusionPass::setAmbientOcclusionPointsCount(int N) {
  ambientOcclusionPointsCount = N;
}

void AmbientOcclusionPass::drawAmbientOcclusionTexture() const
{
  aoBuffer->bind();

  aoBuffer->clear();

  shader->use();

  applyProperties();

  const Renderer::Viewport viewport =
    renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  shader->setUInt("height", viewport.height);
  shader->setUInt("width", viewport.width);

  screenQuad->draw(GL_TRIANGLES);

  unbindTextures();
  shader->unuse();

  aoBuffer->unbind();
}

void AmbientOcclusionPass::calculateFilterWeights(int width)
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

  glGenBuffers(1, &aoFilterUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, aoFilterUBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, 5, aoFilterUBO); // 5 reserved for this
  glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * weights.size(), weights.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void AmbientOcclusionPass::setFilterWidth(int width)
{
  filterWidth = width;
  calculateFilterWeights(filterWidth);
}

void AmbientOcclusionPass::computeBilateralBlurFilter(int width, int height) const
{
  {
    // Horizontal blur
    glBindImageTexture(
      0, ambientFactor, 0,
      GL_FALSE, 0,
      GL_READ_ONLY,
      TextureManager::getInstance().getTextureInfo(
        ambientFactor).format
    );

    glBindImageTexture(
      1, ambientFactorTemp, 0,
      GL_FALSE, 0,
      GL_WRITE_ONLY,
      TextureManager::getInstance().getTextureInfo(
        ambientFactorTemp).format
    );

    compShaderBilateralHor->use();

    applyProperties(compShaderBilateralHor);

    glDispatchCompute(width / 16, height / 1, 1);

    unbindTextures(compShaderBilateralHor);

    compShaderBilateralHor->unuse();
  }

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  {
    // Vertical blur
    glBindImageTexture(
      0, ambientFactorTemp, 0,
      GL_FALSE, 0,
      GL_READ_ONLY,
      TextureManager::getInstance().getTextureInfo(
        ambientFactorTemp).format
    );

    glBindImageTexture(
      1, filteredAmbientFactor, 0,
      GL_FALSE, 0,
      GL_WRITE_ONLY,
      TextureManager::getInstance().getTextureInfo(
        filteredAmbientFactor).format
    );

    compShaderBilateralVert->use();

    applyProperties(compShaderBilateralVert);

    glDispatchCompute(width / 1, height / 16, 1);

    unbindTextures(compShaderBilateralVert);
    compShaderBilateralVert->unuse();
  }
}

void AmbientOcclusionPass::draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const {

  drawAmbientOcclusionTexture();
  auto viewport = renderGraphBuilder->getRenderer()->getCurrentState().viewport;
  computeBilateralBlurFilter(viewport.width, viewport.height);
}
