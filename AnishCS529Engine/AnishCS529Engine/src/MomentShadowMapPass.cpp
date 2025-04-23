#include "precompiled.h"
#include "MomentShadowMapPass.h"

MomentShadowMapPass::MomentShadowMapPass() : RenderPass()
{
  this->addShader("shaders/gaussean_blur_hor.comp");
  shaderVert = ShaderManager::getInstance().loadFile("shaders/gaussean_blur_vert.comp");


  renderMask = RenderMask::MomentShadowMapPass;
}

void MomentShadowMapPass::computeHor(ComputeDispatchSize dispatch)
{
  compute(dispatch);
}

void MomentShadowMapPass::computeVert(ComputeDispatchSize dispatch)
{
  shaderVert->use();

  applyProperties();

  glDispatchCompute(dispatch.x, dispatch.y, dispatch.z);

  unbindTextures();
  shaderVert->unuse();
}
