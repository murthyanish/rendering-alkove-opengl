/*!****************************************************************************
 * \file   MapLoader.h
 * \author Lixiao Kuang
 * \date   03-11-2025
 * \brief  Map loader that loads different maps based on an ID.
 *
 * Usage:
 * 
 *   std::shared_ptr<RenderGraph> rg = mainRenderer->getRenderGraph();
 *   MapLoader::instance().initializeResources(rg);
 *   MapLoader::instance().loadMap(mapId, offsetX, offsetY, offsetZ,
 *       sceneGraph, camera, floorMesh, floorMaterial);
 *
 * Map IDs:
 *   1 = wallrun map
 *   2 = jump map
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#pragma once
#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <memory>
#include <iostream>
#include <string>

#include "GameObject.h"
#include "SceneGraph.h"
#include "Vector3.h"
#include "Camera.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include "Render3D.h"
#include "RigidBody.h"
#include "OBB.h"
#include "Animate.h"
#include "TimeControlledRB.h"
#include "RenderGraph.h"


class MapLoader {
public:
    /// Returns an instance of MapLoader.
    static MapLoader& instance();

    /*!
     * \brief Load a map given a map ID and positional offset.
     *
     * \param mapId         An integer representing the map type (example, 1 = wallrun, 2 = jump).
     * \param offsetX       X offset to add to each object's base position.
     * \param offsetY       Y offset to add to each object's base position.
     * \param offsetZ       Z offset to add to each object's base position.
     * \param sceneGraph    Reference to the SceneGraph where objects will be added.
     * \param camera        Shared pointer to the Camera.
     * \param floorMesh     Shared pointer to the Mesh used by map objects.
     * \param floorMaterial Shared pointer to the Material used by map objects.
     */

    void initializeResources();

    void loadMap(int mapId, float offsetX, float offsetY, float offsetZ,
                 SceneGraph& sceneGraph);

private:
    MapLoader() = default;
    ~MapLoader() = default;
    MapLoader(const MapLoader&) = delete;
    MapLoader& operator=(const MapLoader&) = delete;

    // Materials
    std::shared_ptr<Mesh> boxMesh;
    std::shared_ptr<Mesh> sphereMesh;

    std::shared_ptr<TextureMaterial> concreteMaterial;
    std::shared_ptr<TextureMaterial> cracksMaterial;
    std::shared_ptr<TextureMaterial> LightBlueConcrete;
    std::shared_ptr<TextureMaterial> YellowConcrete;
    std::shared_ptr<TextureMaterial> BrownConcrete;
    std::shared_ptr<TextureMaterial> BlueConcrete;
    std::shared_ptr<TextureMaterial> WhiteFloorTiles;
    std::shared_ptr<TextureMaterial> keyMaterial;
    std::shared_ptr<TextureMaterial> shadowMaterial;
    std::shared_ptr<TextureMaterial> jumpImage;
    std::shared_ptr<TextureMaterial> wordMaterial;


    //void loadWallrun(float offsetX, float offsetY, float offsetZ,
    //                     SceneGraph& sceneGraph,
    //                     std::shared_ptr<Camera> camera);

    void menu(float offsetX, float offsetY, float offsetZ,
          SceneGraph& sceneGraph);

    void tutorial(float offsetX, float offsetY, float offsetZ,
              SceneGraph& sceneGraph);

    void tutorial2(float offsetX, float offsetY, float offsetZ,
              SceneGraph& sceneGraph);

    void intermediate(float offsetX, float offsetY, float offsetZ,
         SceneGraph& sceneGraph);

    void intermediate2(float offsetX, float offsetY, float offsetZ,
  SceneGraph& sceneGraph);

    void intermediate3(float offsetX, float offsetY, float offsetZ,
SceneGraph& sceneGraph);

    void advanced(float offsetX, float offsetY, float offsetZ,
             SceneGraph& sceneGraph);

    void victory(float offsetX, float offsetY, float offsetZ,
      SceneGraph& sceneGraph);
    
    void graphics(float offsetX, float offsetY, float offsetZ,
      SceneGraph& sceneGraph);

    void writeLetter(SceneGraph& sceneGraph, const std::string& meshPath, const Vector3& position,
        const Vector3& scale, const Vector3& rotation,
        std::shared_ptr<Material> keyMaterial);

    void createBlock(SceneGraph& sceneGraph,
                     const std::string& name,
                     const Vector3& pos,
                     const Vector3& scale,
                     const Vector3& rotation,
                     const std::shared_ptr<Material>& material);
    void createBlock(SceneGraph& sceneGraph,
                 const std::string& name,
                 const Vector3& pos,
                 const Vector3& scale,
                 const std::shared_ptr<Material>& material);
};

#endif // MAPLOADER_H
