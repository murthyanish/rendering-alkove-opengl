/*!****************************************************************************
 * \file   MapGraphics.cpp
 * \author Anish Murthy (anish.murthy.dev@gmail.com)
 * \par    **DigiPen Email**
 *    anish.murthy@digipen.edu
 * \par    **Course**
 *    GAM541
 * \date   04-19-2025
 * \brief  Testing map for graphics
 *
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 *
 *****************************************************************************/
#include "precompiled.h"
#include "MapLoader.h"
#include "SolidColorMaterial.h"
#include "GlobalVariables.h"


void MapLoader::graphics(float offsetX, float offsetY, float offsetZ,
                            SceneGraph& sceneGraph) {

  Vector3 center(offsetX, offsetY, offsetZ);

  auto skydomeMaterial = std::make_shared<TextureMaterial>("media/textures/skys/Newport_Loft_Ref.hdr");
  skydomeMaterial->setProperty("skydome", 1);

  auto skydome = std::make_shared<GameObject>("skydome");

  sceneGraph.addNode(skydome);
  skydome->setLocalScaling(Vector3(1000.0f));

  auto skydomeRender = skydome->addComponent<Render3D>();
  skydomeRender->setMaterial(skydomeMaterial);
  skydomeRender->setMesh(Mesh::createMesh("skydome", Mesh::Sphere));

  auto createWall = [&](const std::string& name, const Vector3& localPos, const Vector3& localScale) {
    auto wall = std::make_shared<GameObject>(name, GameObject::RUNNABLE_WALL);
    sceneGraph.addNode(wall);
    wall->setLocalPosition(center + localPos);
    wall->setLocalScaling(localScale);

    auto renderComp = wall->addComponent<Render3D>();
    renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);

    auto shape = std::make_shared<OBB>();
    auto rigidBody = wall->addComponent<RigidBody>();
    rigidBody->setMass(0.0f)
      ->setDrag(1.0f)
      ->setShape(shape)
      ->setStatic(true)
      ->registerToPhysicsManager(PhysicsManager::Instance());
    rigidBody->initialize();
    };

  // Dimensions
  float size = 20.0f;
  float wallThickness = 1.0f;

  float width = size;
  float height = 30.0f;
  float depth = size;



  // Walls
  //createWall("LeftWall", Vector3(-width / 2.0f, height / 2.0f, 0.0f), Vector3(wallThickness, height, depth));
  //createWall("RightWall", Vector3(width / 2.0f, height / 2.0f, 0.0f), Vector3(wallThickness, height, depth));
  //createWall("BackWall", Vector3(0.0f, height / 2.0f, depth / 2.0f), Vector3(width, height, wallThickness));
  //createWall("FrontWall", Vector3(0.0f, height / 2.0f, -depth / 2.0f), Vector3(width, height, wallThickness));
  //createWall("TopWall", Vector3(0.0f, height, 0.0f), Vector3(width, wallThickness, depth));
  
  createWall("BottomWall", Vector3(0.0f, 0.0f, 0.0f), Vector3(width, wallThickness, depth));

  {
    auto testDoor = std::make_shared<GameObject>("Door", GameObject::WALL);
    sceneGraph.addNode(testDoor);
    testDoor->setLocalPosition(Vector3(0.0f + offsetX, 2.5f + offsetY, 9.0f + offsetZ));
    testDoor->setLocalScaling(Vector3(0.3f, 5.0f, 3.0f));
    testDoor->setLocalRotation(Vector3(0.0f, 1.57f, 0.0f));
    auto renderComp = testDoor->addComponent<Render3D>();
    renderComp->setMesh(boxMesh)->setMaterial(BrownConcrete);
  }

  // Lights

  sceneGraph.addPointLight(
    PointLight(
      Vector3(-110.5f + offsetX, 50.0f + offsetY, -120.0f + offsetZ),
      2.0f,
      Vector3(1.0f),
      50.0f));


        /*for (int i = 0; i < 980; ++i) {
          auto lightPosition = Vector3(
            EngineMath::Random::boundedFloat(initialPos.x - 10, initialPos.x + 10),
            EngineMath::Random::boundedFloat(initialPos.y, initialPos.y + 20),
            EngineMath::Random::boundedFloat(initialPos.z - 10, initialPos.z + 10));
          auto lightIntensity = EngineMath::Random::boundedFloat(2.0f, 4.0f);
          auto lightColor = Vector3(
            EngineMath::Random::boundedFloat(0.5f, 1.0f),
            EngineMath::Random::boundedFloat(0.5f, 1.0f),
            EngineMath::Random::boundedFloat(0.5f, 1.0f));
          auto lightRange = EngineMath::Random::boundedFloat(10.0f, 20.0f);
          sceneGraph.addPointLight(PointLight(lightPosition, lightIntensity, lightColor, lightRange));
        }*/

        // Materials

  auto bunnyMaterial = Material::getMaterial<SolidColorMaterial>("bunnyMaterial");
  bunnyMaterial->setProperty("diffuse", Vector3(1.0f, 0.3f, 0.3f));
  bunnyMaterial->setProperty("specular", Vector3(0.015f, 0.015f, 0.015f));
  bunnyMaterial->setProperty("shininess", 100.0f);

  auto bunnyMaterial2 = Material::getMaterial<SolidColorMaterial>("bunnyMaterial2");
  bunnyMaterial2->setProperty("diffuse", Vector3(0.3f, 1.0f, 0.3f));
  bunnyMaterial2->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  bunnyMaterial2->setProperty("shininess", 20.0f);

  auto bunnyMaterial3 = Material::getMaterial<SolidColorMaterial>("bunnyMaterial3");
  bunnyMaterial3->setProperty("diffuse", Vector3(0.3f, 0.3f, 1.0f));
  bunnyMaterial3->setProperty("specular", Vector3(0.001f, 0.001f, 0.001f));
  bunnyMaterial3->setProperty("shininess", 2.0f);

  Vector3 initialPos = Vector3(0.0f, 3.0f + 0.25f, 0.0f);

  float intensity = 1.0f;
  float range = 30.0f;
  float lightWidth = 8.0f;
  float lightHeight = 12.0f;
  float lightDepth = 8.0f;
  Vector3 initialPosition = Vector3(0.0f, 8.0f, 0.0f);

  /*sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(lightWidth, lightHeight, lightDepth),
      intensity, Vector3(1.0f, 0.5f, 1.0f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(lightWidth, lightHeight, -lightDepth),
      intensity, Vector3(1.0f, 1.0f, 0.5f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(lightWidth, -lightHeight, lightDepth),
      intensity, Vector3(0.5f, 1.0f, 1.0f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(lightWidth, -lightHeight, -lightDepth),
      intensity, Vector3(1.0f, 0.5f, 1.0f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(-lightWidth, lightHeight, lightDepth),
      intensity, Vector3(1.0f, 1.0f, 0.5f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(-lightWidth, lightHeight, -lightDepth),
      intensity, Vector3(0.5f, 1.0f, 1.0f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(-lightWidth, -lightHeight, lightDepth),
      intensity, Vector3(1.0f, 0.5f, 1.0f), range)
  );
  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(-lightWidth, -lightHeight, -lightDepth),
      intensity, Vector3(1.0f, 1.0f, 0.5f), range)
  );*/

  /*for (int i = 0; i < 1000; ++i) {
    auto lightPosition = Vector3(
      EngineMath::Random::boundedFloat(initialPosition.x - lightWidth, initialPosition.x + lightWidth),
      EngineMath::Random::boundedFloat(initialPosition.y - lightHeight, initialPosition.y + lightHeight),
      EngineMath::Random::boundedFloat(initialPosition.z - lightDepth, initialPosition.z + lightDepth));
    auto lightIntensity = EngineMath::Random::boundedFloat(0.5f, intensity);
    auto lightColor = Vector3(
      EngineMath::Random::boundedFloat(0.5f, 1.0f),
      EngineMath::Random::boundedFloat(0.5f, 1.0f),
      EngineMath::Random::boundedFloat(0.5f, 1.0f));
    auto lightRange = EngineMath::Random::boundedFloat(2.0f, range);
    sceneGraph.addPointLight(PointLight(lightPosition, lightIntensity, lightColor, lightRange));
  }*/


  sceneGraph.addPointLight(
    PointLight(
      initialPosition + Vector3(0.0f, 20.0f, 0.0f),
      2000.0f, Vector3(1.0f, 1.0f, 1.0f), 1000.0f,
      1
      )
  );


  // Objects

  {
    auto bunny = std::make_shared<GameObject>("bunny");
    sceneGraph.addNode(bunny);
    bunny->setLocalPosition(Vector3(-2.0f, 3.0f, 0.0f));
    bunny->setLocalScaling(Vector3(10.0f, 10.0f, 10.0f));
    bunny->setLocalRotation(Quaternion::fromEuler(Vector3(0.0f, PI, 0.0f)));
    auto renderComp = bunny->addComponent<Render3D>();
    auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
    renderComp->setMesh(bunnyMesh)->setMaterial(bunnyMaterial)->setRenderMask(RenderMask::ShadowLit);
  }

  {
    auto bunny = std::make_shared<GameObject>("bunny2");
    sceneGraph.addNode(bunny);
    bunny->setLocalPosition(Vector3(0.0f, 3.0f, 0.0f));
    bunny->setLocalScaling(Vector3(10.0f, 10.0f, 10.0f));
    bunny->setLocalRotation(Quaternion::fromEuler(Vector3(0.0f, PI, 0.0f)));
    auto renderComp = bunny->addComponent<Render3D>();
    auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
    renderComp->setMesh(bunnyMesh)->setMaterial(bunnyMaterial2)->setRenderMask(RenderMask::ShadowLit);
  }

  {
    auto bunny = std::make_shared<GameObject>("bunny3");
    sceneGraph.addNode(bunny);
    bunny->setLocalPosition(Vector3(2.0f, 3.0f, 0.0f));
    bunny->setLocalScaling(Vector3(10.0f, 10.0f, 10.0f));
    bunny->setLocalRotation(Quaternion::fromEuler(Vector3(0.0f, PI, 0.0f)));
    auto renderComp = bunny->addComponent<Render3D>();
    auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
    renderComp->setMesh(bunnyMesh)->setMaterial(bunnyMaterial3)->setRenderMask(RenderMask::ShadowLit);
  }

  {
    auto bunnyFloor = std::make_shared<GameObject>("bunnyFloor");
    sceneGraph.addNode(bunnyFloor);
    bunnyFloor->setLocalPosition(Vector3(0.0f, 3.1f, 0.0f));
    bunnyFloor->setLocalScaling(Vector3(6.0f, 0.5f, 6.0f));
    auto renderComp = bunnyFloor->addComponent<Render3D>();
    renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete)->setRenderMask(RenderMask::ShadowLit);
    auto shape = std::make_shared<OBB>();
    auto rigidBody = bunnyFloor->addComponent<RigidBody>();
    rigidBody->setMass(0.0f)
      ->setDrag(1.0f)
      ->setShape(shape)
      ->setStatic(true)
      ->registerToPhysicsManager(PhysicsManager::Instance());
    rigidBody->initialize();
  }

  {
    auto teapot = std::make_shared<GameObject>("teapot");
    sceneGraph.addNode(teapot);
    teapot->setLocalPosition(Vector3(0.0f, 5.0f, 0.0f));
    teapot->setLocalScaling(Vector3(1.0f, 1.0f, 1.0f));
    teapot->setLocalRotation(Quaternion::fromEuler(Vector3(-PI/2.0f, 0.0f, 0.0f)));
    auto renderComp = teapot->addComponent<Render3D>();
    auto teapotMesh = Mesh::createTeapotMesh("teapot");
    renderComp->setMesh(teapotMesh)->setMaterial(cracksMaterial)->setRenderMask(RenderMask::ShadowLit);
  }


  auto autoBunnyParent = std::make_shared<GameObject>("autoBunnyParent");
  sceneGraph.addNode(autoBunnyParent);

  /*auto autoBunnyMaterial = Material::getMaterial<SolidColorMaterial>("autoBunnyMaterial");
  autoBunnyMaterial->setProperty("diffuse", Vector3(0.0f));
  autoBunnyMaterial->setProperty("specular", Vector3(0.8f));
  autoBunnyMaterial->setProperty("shininess", 128.0f);

  for (int i = 0; i < 1000; ++i) {
    auto bunny = std::make_shared<GameObject>("autoBunny"+std::to_string(i));
    autoBunnyParent->addChild(bunny);
    bunny->setLocalPosition(Vector3(
      EngineMath::Random::boundedFloat(initialPosition.x - lightWidth, initialPosition.x + lightWidth),
      EngineMath::Random::boundedFloat(initialPosition.y - lightHeight, initialPosition.y + lightHeight),
      EngineMath::Random::boundedFloat(initialPosition.z - lightDepth, initialPosition.z + lightDepth)
    ));
    bunny->setLocalScaling(Vector3(EngineMath::Random::boundedFloat(1.0f,4.0f)));
    bunny->setLocalRotation(Quaternion(
      EngineMath::Random::boundedFloat(0.0f, 1.0f),
      EngineMath::Random::boundedFloat(0.0f, 1.0f),
      EngineMath::Random::boundedFloat(0.0f, 1.0f)));
    auto renderComp = bunny->addComponent<Render3D>();
    auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
    renderComp->setMesh(bunnyMesh)->setMaterial(autoBunnyMaterial);
  }*/

}