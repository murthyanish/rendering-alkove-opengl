/*!****************************************************************************
 * \file   DitheringPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-08-2025
 * \brief  
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef DITHERING_PASS_H
#define DITHERING_PASS_H

#pragma once

#include "RenderPass.h"

class DitheringPass : public RenderPass {
protected:
  std::shared_ptr<Mesh> screenQuad;
  // Dithering Texture

public:
  DitheringPass();

  void initialize();
  void addDitheringTexture(TextureManager::TextureID textureID);
  void addDitheringTexture(const std::string& texturePath);

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* scene) const override;
};

#endif // !DITHERING_PASS_H
