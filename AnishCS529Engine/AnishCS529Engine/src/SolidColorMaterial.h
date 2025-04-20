/*!****************************************************************************
 * \file   SolidColorMaterial.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \date   02-10-2025
 * \brief  Solid Color Material to draw basic shapes
 *
 *****************************************************************************/
#ifndef SOLID_COLOR_MATERIAL_H
#define SOLID_COLOR_MATERIAL_H

#pragma once

#include "Material.h"

class SolidColorMaterial : public Material {
public:
  SolidColorMaterial() : Material() {
    // 0b0001 is the renderMask for the default lighting pass
    defaultRenderMask = RenderMask::DefaultLit;
    setProperty("useTexture", 0);
  }
};

#endif // !SOLID_COLOR_MATERIAL_H
