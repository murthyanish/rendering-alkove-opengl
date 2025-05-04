/*!****************************************************************************
 * \file   RenderFlags.cpp
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
#include "RenderFlags.h"

static std::vector<uint32_t> RenderPassOrder = {
    RenderMask::SkydomePass,
    RenderMask::ShadowPass,
    RenderMask::MomentShadowMapPass,
    RenderMask::GBufferPrepass,
    RenderMask::DebugPass,
    RenderMask::AmbientOcclusionPass,
    RenderMask::LightingPass,
    RenderMask::LocalLightsPass,
    RenderMask::UIPass
};