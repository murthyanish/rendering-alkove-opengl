/*!****************************************************************************
 * \file   MapLoader.cpp
 * \author Li Kuang (li.kuang@digipen.edu)
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "DeathPlane.h"
#include "Door.h"
#include "InGameKey.h"
#include "KeyList.h"
#include "MapLoader.h"
#include "Matrix4.h"

MapLoader& MapLoader::instance() {
    static MapLoader instance;
    return instance;
}

void MapLoader::initializeResources() {
  // Get meshes
  boxMesh = Mesh::getShapeMesh(Mesh::Cube);
  sphereMesh = Mesh::getShapeMesh(Mesh::Sphere);

  // Concrete Material
  concreteMaterial = Material::getMaterial<TextureMaterial>("concrete");
  concreteMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  concreteMaterial->setProperty("shininess", 10.0f);
  concreteMaterial->addTexture("media/textures/Concrete.png", 2.0f, 2.0f);

  // Cracks Material
  cracksMaterial = Material::getMaterial<TextureMaterial>("cracks");
  cracksMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  cracksMaterial->setProperty("shininess", 10.0f);
  cracksMaterial->addTexture("media/textures/cracks.png");

  // Additional Materials

  LightBlueConcrete = Material::getMaterial<TextureMaterial>("LightBlueConcrete");
  LightBlueConcrete->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  LightBlueConcrete->setProperty("shininess", 10.0f);
  LightBlueConcrete->addTexture("media/textures/LightBlueConcrete.png", 2.0f, 2.0f);

  YellowConcrete = Material::getMaterial<TextureMaterial>("YellowConcrete");
  YellowConcrete->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  YellowConcrete->setProperty("shininess", 10.0f);
  YellowConcrete->addTexture("media/textures/YellowConcrete.png", 20.0f, 20.0f);

  BrownConcrete = Material::getMaterial<TextureMaterial>("BrownConcrete");
  BrownConcrete->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  BrownConcrete->setProperty("shininess", 10.0f);
  BrownConcrete->addTexture("media/textures/BrownConcrete.png", 0.5f, 1.0f);

  BlueConcrete = Material::getMaterial<TextureMaterial>("BlueConcrete");
  BlueConcrete->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  BlueConcrete->setProperty("shininess", 10.0f);
  BlueConcrete->addTexture("media/textures/BlueConcrete.png", 3.0f, 3.0f);

  WhiteFloorTiles = Material::getMaterial<TextureMaterial>("WhiteFloorTiles");
  WhiteFloorTiles->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  WhiteFloorTiles->setProperty("shininess", 10.0f);
  WhiteFloorTiles->addTexture("media/textures/WhiteFloorTiles.png", 20.0f, 20.0f);

  keyMaterial = Material::getMaterial<TextureMaterial>("key");
  keyMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  keyMaterial->setProperty("shininess", 20.0f);
  keyMaterial->addTexture("media/textures/key.png", 1.0f, 1.0f);

  jumpImage = Material::getMaterial<TextureMaterial>("jumpImage");
  jumpImage->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  jumpImage->setProperty("shininess", 1.0f);
  jumpImage->addTexture("media/textures/jumpImage.png", 1.0f, 1.0f);

  wordMaterial = Material::getMaterial<TextureMaterial>("words");
  wordMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  wordMaterial->setProperty("shininess", 1.0f);
  wordMaterial->addTexture("media/textures/words.png", 1.0f, 1.0f);
}


void MapLoader::loadMap(int mapId, float offsetX, float offsetY, float offsetZ,
                        SceneGraph& sceneGraph) {


    switch (mapId) {
    case -1:
        menu(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 0:
        tutorial(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 1:
        tutorial2(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 2:
        intermediate(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 3:
        intermediate2(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 4:
        intermediate3(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 5:
        advanced(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 6:
        victory(offsetX, offsetY, offsetZ, sceneGraph);
        break;
    case 7:
      graphics(offsetX, offsetY, offsetZ, sceneGraph);
      break;

    default:
        std::cerr << "[MapLoader] Map ID " << mapId << " is not recognized.\n";
        break;
    }
}

void MapLoader::writeLetter(SceneGraph& sceneGraph,
                            const std::string& meshPath,
                            const Vector3& position,
                            const Vector3& scale,
                            const Vector3& rotation,
                            std::shared_ptr<Material> keyMaterial)
{
    std::shared_ptr<Mesh> mesh = Mesh::loadMesh(meshPath);
    auto letter = std::make_shared<GameObject>("letter", GameObject::WALL);
    sceneGraph.addNode(letter);
    letter->setLocalPosition(position);
    letter->setLocalScaling(scale);
    letter->setLocalRotation(rotation);
    auto renderComp = letter->addComponent<Render3D>();
    renderComp->setMesh(mesh)->setMaterial(keyMaterial);
}

void MapLoader::createBlock(SceneGraph& sceneGraph,
                            const std::string& name,
                            const Vector3& pos,
                            const Vector3& scale,
                            const Vector3& rotation,
                            const std::shared_ptr<Material>& material)
{
    // Name of the block
    auto block = std::make_shared<GameObject>(name);
    sceneGraph.addNode(block);

    // Block's properties 
    block->setLocalPosition(pos);
    block->setLocalScaling(scale);
    block->setLocalRotation(rotation);

    // Render3D component
    auto renderComp = block->addComponent<Render3D>();
    renderComp->setMesh(boxMesh)->setMaterial(material);

    // OBB
    auto shape = std::make_shared<OBB>();
    auto rigidBody = block->addComponent<RigidBody>();
    rigidBody->setMass(0.0f)
        ->setDrag(1.0f)
        ->setShape(shape)
        ->setStatic(true)
        ->registerToPhysicsManager(PhysicsManager::Instance());
    rigidBody->initialize();
}

// Overload without rotation 
void MapLoader::createBlock(SceneGraph& sceneGraph,
                            const std::string& name,
                            const Vector3& pos,
                            const Vector3& scale,
                            const std::shared_ptr<Material>& material)
{
    createBlock(sceneGraph, name, pos, scale, Vector3(0.0f, 0.0f, 0.0f), material);
}









