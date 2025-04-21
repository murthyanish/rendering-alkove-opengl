#include "precompiled.h"
#include "MomentShadowMapPass.h"

MomentShadowMapPass::MomentShadowMapPass() : RenderPass()
{
  //this->addShader("shaders/moment_shadow_map.comp");

  renderMask = RenderMask::MomentShadowMapPass;
}
