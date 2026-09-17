/*!****************************************************************************
 * \file   Material.h
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \author Mariojulio Osmin Zaldivar Alvarez (ex-Adjunct Professor)
 *   (zaldivaralvarez@gmail.com) (Advising and Base Code)
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * \brief  Material class
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include <unordered_map>
#include <variant>
#include <optional>

#include "Matrix4.h"
#include "Mesh.h"
#include "RenderGraph.h"
#include "Vector3.h"
#include "RenderFlags.h"

 /*!****************************************************************************
  * \brief Class to manage Materials used by objects
  *
  * This class handles creation of shaders and setting parameters for them.
  *****************************************************************************/
class Material : public std::enable_shared_from_this<Material> {
public:
  using PropertyMap = RenderPass::PropertyMap;

  Material() = default;

  virtual ~Material() = default;

  template<typename T>
  void setProperty(const std::string& name, const T& value) {
    properties[name] = value;
  }

  template<typename T>
  void setTempProperty(const std::string& name, const T& value) {
    tempProperties[name] = value;
  }

  void clearTempProperties() {
    tempProperties.clear();
  }

  virtual void apply(std::shared_ptr<Shader> shader) const;

  /* Material factory */
  static std::unordered_map<std::string, std::shared_ptr<Material>> basicMaterials;

  template <typename T>
  static std::shared_ptr<T> getMaterial(
    const std::string& name);

  uint32_t getDefaultRenderMask() { return defaultRenderMask; }

protected:
  std::shared_ptr<RenderGraph> renderGraph;
  PropertyMap properties;
  PropertyMap tempProperties;
  uint32_t defaultRenderMask = RenderMask::None;
};


#include "Material.inl"

#endif // MATERIAL_H

// Challenge for you: Can you think of a way to templetize the setProperty() function?
// What about the properties map? Instead of a variant, could you use a template parameter? In that case, how would you handle setting the uniforms?
