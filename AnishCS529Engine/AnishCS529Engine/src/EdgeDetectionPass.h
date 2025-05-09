/*!****************************************************************************
 * \file   EdgeDetectionPass.h
 * \author Anish Murthy (anish.murthy@digipen.edu)
 * \date   05-08-2025
 * \brief  Edge Detection Pass for Disctinct Geometry based edges
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef EDGE_DETECTION_PASS_H
#define EDGE_DETECTION_PASS_H

#pragma once

#include "RenderPass.h"

class EdgeDetectionPass : public RenderPass {
private:
  std::shared_ptr<Mesh> screenQuad;

  FBO* edgeBuffer;

  int filterWidth = -1;

  TextureManager::TextureID edgeTexture;
public:
  EdgeDetectionPass();

  void initialize();

  void draw(
    std::shared_ptr<Camera> camera,
    SceneGraph* sceneGraph) const override;

};

#endif // !EDGE_DETECTION_PASS_H
