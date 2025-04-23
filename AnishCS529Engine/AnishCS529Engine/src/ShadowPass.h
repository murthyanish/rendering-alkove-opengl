/*!****************************************************************************
 * \file   ShadowPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   04-20-2025
 * \brief  Shadow Calculation Pass
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef SHADOW_PASS_H
#define SHADOW_PASS_H

#pragma once

#include "RenderPass.h"

class ShadowPass : public RenderPass {
public:
  ShadowPass();
};

#endif // !SHADOW_PASS_H
