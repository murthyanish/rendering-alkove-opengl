/*!****************************************************************************
 * \file   FinalOneBitPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-09-2025
 * \brief  
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef FINAL_ONE_BIT_PASS_H
#define FINAL_ONE_BIT_PASS_H

#pragma once

#include "RenderPass.h"

class FinalOneBitPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> screenQuad;

public:
  FinalOneBitPass();

  void initialize();

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const override;
};

#endif // !FINAL_ONE_BIT_PASS_H
