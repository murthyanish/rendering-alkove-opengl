#include "precompiled.h"
#include "SurfaceColorPass.h"
#include "Renderer.h"

SurfaceColorPass::SurfaceColorPass() :
  RenderPass(), screenQuad(Mesh::createSquareMesh("screenQuad", 1.0f)) {
  this->addShader("shaders/surface_color.vert\nshaders/surface_color.frag");

  setProperty("enableIrradiance", 0);
  setProperty("enableIBLSpecular", 0);
  renderMask = RenderMask::LightingPass;
}

void SurfaceColorPass::initialize()
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
}

void SurfaceColorPass::draw(
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
