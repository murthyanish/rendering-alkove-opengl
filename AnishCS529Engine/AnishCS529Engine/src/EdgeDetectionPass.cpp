#include "precompiled.h"
#include "EdgeDetectionPass.h"
#include "Renderer.h"

EdgeDetectionPass::EdgeDetectionPass()
{
  this->addShader("shaders/edge_detection.comp");

  //Reusing this mask because they use similar order
  renderMask = RenderMask::AmbientOcclusionPass;
}

void EdgeDetectionPass::initialize()
{
  edgeTexture = renderGraphBuilder->createScreenSizeTexture("edge_texture", 1);

  const Renderer::Viewport viewport =
    renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  setProperty("screenWidth", viewport.width);
  setProperty("screenHeight", viewport.height);

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
}

void EdgeDetectionPass::draw(
  std::shared_ptr<Camera> camera,
  SceneGraph* sceneGraph) const {

  glBindImageTexture(
    1, edgeTexture, 0,
    GL_FALSE, 0,
    GL_WRITE_ONLY,
    TextureManager::getInstance().getTextureInfo(
      edgeTexture).format
  );

  const Renderer::Viewport viewport =
    renderGraphBuilder->getRenderer()->getCurrentState().viewport;

  compute(RenderPass::ComputeDispatchSize(viewport.width / 16, viewport.height / 16, 1));

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
