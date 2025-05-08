/*!****************************************************************************
 * \file   Map_Advanced.cpp
 * \author Li Kuang (li.kuang@digipen.edu)
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "MapLoader.h"
#include "Door.h"
#include "DeathPlane.h"
#include "InGameKey.h"
#include "LevelManager.h"

void MapLoader::advanced(float offsetX, float offsetY, float offsetZ,
                            SceneGraph& sceneGraph) {

    // Create box around the map
    Vector3 center(offsetX, offsetY, offsetZ);
    auto createWall = [&](const std::string& name, const Vector3& localPos, const Vector3& localScale) {
        auto wall = std::make_shared<GameObject>(name, GameObject::WALL);
        sceneGraph.addNode(wall);
        wall->setLocalPosition(center + localPos);
        wall->setLocalScaling(localScale);

        auto renderComp = wall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);

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
    float size = 400.0f;
    float wallThickness = 1.0f;

    float width = size;
    float height = 200.0f;
    float depth = size;

    /*sceneGraph.addPointLight(
      PointLight(
        Vector3(-100.0f, 0.0f, 100.0f),
        20.0f, 
        Vector3(1.0f), 
        10000.0f));*/
    /*sceneGraph.addPointLight(
      PointLight(
        Vector3(-110.5f + offsetX, 50.0f + offsetY, -120.0f + offsetZ),
        2.0f,
        Vector3(1.0f),
        50.0f));*/

    Vector3 initialPos = Vector3(
      -110.5f + offsetX, 10.0f + 0.25f + offsetY, -120.0f + offsetZ);

    for (int i = 0; i < 980; ++i) {
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
      sceneGraph.addPointLight(PointLight(lightPosition , lightIntensity , lightColor , lightRange));
    }

    for (int i = 0; i < 20; ++i) {
      auto lightPosition = Vector3(
        EngineMath::Random::boundedFloat(initialPos.x - 50, initialPos.x + 50),
        EngineMath::Random::boundedFloat(initialPos.y-20, initialPos.y + 80),
        EngineMath::Random::boundedFloat(initialPos.z - 50, initialPos.z + 50));
      auto lightIntensity = 100.0f;
      auto lightColor = Vector3(
        EngineMath::Random::boundedFloat(0.5f, 1.0f),
        EngineMath::Random::boundedFloat(0.5f, 1.0f),
        EngineMath::Random::boundedFloat(0.5f, 1.0f));
      auto lightRange = EngineMath::Random::boundedFloat(50.0f, 100.0f);
      sceneGraph.addPointLight(PointLight(lightPosition, lightIntensity, lightColor, lightRange));
    }

    // Walls
    createWall("LeftWall", Vector3(-width / 2.0f, height / 2.0f, 0.0f), Vector3(wallThickness, height, depth));
    createWall("RightWall", Vector3(width / 2.0f, height / 2.0f, 0.0f), Vector3(wallThickness, height, depth));
    createWall("BackWall", Vector3(0.0f, height / 2.0f, depth / 2.0f), Vector3(width, height, wallThickness));
    createWall("FrontWall", Vector3(0.0f, height / 2.0f, -depth / 2.0f), Vector3(width, height, wallThickness));
    createWall("TopWall", Vector3(0.0f, height, 0.0f), Vector3(width, wallThickness, depth));
    //createWall("BottomWall", Vector3(0.0f, 0.0f, 0.0f), Vector3(width, wallThickness, depth));

    {
        auto DP1 = std::make_shared<GameObject>("DP1");
        sceneGraph.addNode(DP1);
        DP1->setLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
        DP1->setLocalScaling(Vector3(width, wallThickness, depth));
        auto renderComp = DP1->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = DP1->addComponent<DeathPlane>();
        rigidBody->setPlayerName("PlayerBox");
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Door to next level
    {
        auto testDoor = std::make_shared<GameObject>("Door", GameObject::WALL);
        sceneGraph.addNode(testDoor);
        testDoor->setLocalPosition(Vector3(-171.0f + offsetX, 138.0f + offsetY, -148.0f + offsetZ));
        testDoor->setLocalScaling(Vector3(0.3f, 5.0f, 3.0f));
        auto renderComp = testDoor->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BrownConcrete);
        auto shape = std::make_shared<OBB>();
        auto doorComp = testDoor->addComponent<Door>();
        doorComp->setID(0);
        doorComp->setType(Door::DoorType::NEXTLEVEL);
        doorComp->setRequiresKey(false);
        doorComp->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        doorComp->initialize();
    }
 
    // ------------
    //   Wall run
    // ------------
    // 
    // Checkpoint pad
    {
        auto checkPoint1 = std::make_shared<GameObject>("checkPoint1", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint1);
        checkPoint1->setLocalPosition(Vector3(-130.5f + offsetX, 6.0f + offsetY, -130.0f + offsetZ));
        checkPoint1->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint1->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint1->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Todo: Add model load here
    {
      auto bunny = std::make_shared<GameObject>("bunny");
      sceneGraph.addNode(bunny);
      bunny->setLocalPosition(Vector3(-110.5f + offsetX, 10.0f + offsetY, -120.0f + offsetZ));
      bunny->setLocalScaling(Vector3(20.0f, 20.0f, 20.0f));
      bunny->setLocalRotation(Quaternion::fromEuler(Vector3(0.0f, PI, 0.0f)));
      auto renderComp = bunny->addComponent<Render3D>();
      auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
      renderComp->setMesh(bunnyMesh)->setMaterial(cracksMaterial);
    }

    {
      auto bunny = std::make_shared<GameObject>("bunny2");
      sceneGraph.addNode(bunny);
      bunny->setLocalPosition(Vector3(-115.5f + offsetX, 10.0f + offsetY, -120.0f + offsetZ));
      bunny->setLocalScaling(Vector3(20.0f, 20.0f, 20.0f));
      bunny->setLocalRotation(Quaternion::fromEuler(Vector3(0.0f, -PI/2.0f, 0.0f)));
      auto renderComp = bunny->addComponent<Render3D>();
      auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
      renderComp->setMesh(bunnyMesh)->setMaterial(cracksMaterial);
    }

    {
      auto bunny = std::make_shared<GameObject>("bunny3");
      sceneGraph.addNode(bunny);
      bunny->setLocalPosition(Vector3(-105.5f + offsetX, 10.0f + offsetY, -120.0f + offsetZ));
      bunny->setLocalScaling(Vector3(20.0f, 20.0f, 20.0f));
      bunny->setLocalRotation(Quaternion::fromEuler(Vector3(0.0f, PI / 2.0f, 0.0f)));
      auto renderComp = bunny->addComponent<Render3D>();
      auto bunnyMesh = Mesh::loadMesh("media/models/bun_zipper.ply");
      renderComp->setMesh(bunnyMesh)->setMaterial(cracksMaterial);
    }

    {
      auto bunnyFloor = std::make_shared<GameObject>("bunnyFloor");
      sceneGraph.addNode(bunnyFloor);
      bunnyFloor->setLocalPosition(Vector3(-110.5f + offsetX, 10.2f + offsetY, -120.0f + offsetZ));
      bunnyFloor->setLocalScaling(Vector3(15.0f, 1.0f, 15.0f));
      auto renderComp = bunnyFloor->addComponent<Render3D>();
      renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
      auto shape = std::make_shared<OBB>();
      auto rigidBody = bunnyFloor->addComponent<RigidBody>();
      rigidBody->setMass(0.0f)
        ->setDrag(1.0f)
        ->setShape(shape)
        ->setStatic(true)
        ->registerToPhysicsManager(PhysicsManager::Instance());
      rigidBody->initialize();
    }

    // Wall for wall running
    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall");
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-135.0f + offsetX, 20.0f + offsetY, -110.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 30.0f, 30.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-135.0f + offsetX, 13.0f + offsetY, -110.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.1f, 10.0f, 29.9f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Checkpoint2 
    {
        auto checkPoint2 = std::make_shared<GameObject>("checkPoint2", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint2);
        checkPoint2->setLocalPosition(Vector3(-130.5f + offsetX, 10.0f + offsetY, -95.0f + offsetZ));
        checkPoint2->setLocalScaling(Vector3(8.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint2->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint2->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Walls


    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall");
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-135.0f + offsetX, 15.0f + offsetY, -70.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 50.0f, 30.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-135.0f + offsetX, 15.0f + offsetY, -70.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.1f, 10.0f, 29.9f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall");
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-125.0f + offsetX, 15.0f + offsetY, -80.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 50.0f, 40.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-124.9f + offsetX, 22.0f + offsetY, -80.1f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.05f, 10.0f, 39.9f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall");
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-110.0f + offsetX, 15.0f + offsetY, -70.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 50.0f, 40.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // 3rd wall wallrun
    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-109.9f + offsetX, 22.0f + offsetY, -72.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.05f, 10.0f, 35.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall");
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-125.0f + offsetX, 15.0f + offsetY, -50.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(30.0f, 50.0f, 1.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-125.0f + offsetX, 25.0f + offsetY, -50.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(28.0f, 25.0f, 1.1f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    
    writeLetter(sceneGraph, "media/Map/words/arrow2.fbx", Vector3(-119.0f + offsetX, 22.0f + offsetY, -99.0f + offsetZ), Vector3(0.03f, 0.03f, 0.01f), Vector3(0.0f, 0.0f, -1.57f), wordMaterial);
    

    // Checkpoint3
    
    {
        if (LevelManager::Instance().getDifficulty() != FirstPersonControllerComponent::HARD) {
            if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::NORMAL) {
                auto checkPoint3 = std::make_shared<GameObject>("checkPoint3", GameObject::CHECKPOINT);
                sceneGraph.addNode(checkPoint3);
                checkPoint3->setLocalPosition(Vector3(-115.0f + offsetX, 16.0f + offsetY, -55.0f + offsetZ));
                checkPoint3->setLocalScaling(Vector3(8.0f, 1.0f, 8.0f));
                auto renderComp = checkPoint3->addComponent<Render3D>();
                renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
                auto shape = std::make_shared<OBB>();
                auto rigidBody = checkPoint3->addComponent<RigidBody>();
                rigidBody->setMass(0.0f)
                    ->setDrag(1.0f)
                    ->setShape(shape)
                    ->setStatic(true)
                    ->registerToPhysicsManager(PhysicsManager::Instance());
                rigidBody->initialize();
            }
            else {
                auto checkPoint3 = std::make_shared<GameObject>("checkPoint3", GameObject::CHECKPOINT);
                sceneGraph.addNode(checkPoint3);
                checkPoint3->setLocalPosition(Vector3(-125.0f + offsetX, 16.0f + offsetY, -55.0f + offsetZ));
                checkPoint3->setLocalScaling(Vector3(25.0f, 1.0f, 8.0f));
                auto renderComp = checkPoint3->addComponent<Render3D>();
                renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
                auto shape = std::make_shared<OBB>();
                auto rigidBody = checkPoint3->addComponent<RigidBody>();
                rigidBody->setMass(0.0f)
                    ->setDrag(1.0f)
                    ->setShape(shape)
                    ->setStatic(true)
                    ->registerToPhysicsManager(PhysicsManager::Instance());
                rigidBody->initialize();

            }

        }


    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-110.0f + offsetX, 22.0f + offsetY, -100.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(30.0f, 10.0f, 1.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Checkpoint4
    if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) 
    {
        auto checkPoint4 = std::make_shared<GameObject>("checkPoint4", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint4);
        checkPoint4->setLocalPosition(Vector3(-100.5f + offsetX, 20.0f + offsetY, -55.0f + offsetZ));
        checkPoint4->setLocalScaling(Vector3(16.0f, 1.0f, 40.0f));
        auto renderComp = checkPoint4->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint4->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }
    else {
        auto checkPoint4 = std::make_shared<GameObject>("checkPoint4", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint4);
        checkPoint4->setLocalPosition(Vector3(-100.5f + offsetX, 22.0f + offsetY, -40.0f + offsetZ));
        checkPoint4->setLocalScaling(Vector3(16.0f, 1.0f, 16.0f));
        auto renderComp = checkPoint4->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint4->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Extra Road
    if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) {
            {
            auto extraRoad = std::make_shared<GameObject>("extraRoad");
            sceneGraph.addNode(extraRoad);
            extraRoad->setLocalPosition(Vector3(-124.5f + offsetX, 24.0f + offsetY, -40.0f + offsetZ));
            extraRoad->setLocalScaling(Vector3(32.0f, 1.0f, 16.0f));
            auto renderComp = extraRoad->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = extraRoad->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
            
        auto platformAnimate = extraRoad->addComponent<Animate>();
        platformAnimate->setAnimateFunction(
                [currentTime = 0.0f, initialPos = extraRoad->getLocalPosition().y, phase = 1.0f](std::shared_ptr<GameObject> self, float deltaTime) mutable {

                    /* Affine Transformation Variables */
                    float affineSpeed = 5.0f;
                    float affinePosVarianceZ = 5.0f;

                    currentTime += deltaTime;

                    if ((phase == -1.0f) && (initialPos - affinePosVarianceZ) > self->getWorldPosition().y)
                        phase = 1.0f;
                    else if ((phase == 1.0f) && (initialPos + affinePosVarianceZ) < self->getWorldPosition().y)
                        phase = -1.0f;

                    Vector3 velocity = Vector3(0.0f, affineSpeed * phase, 0.0f);

                    Vector3 newPos = self->getWorldPosition() + (velocity * deltaTime);
                    self->setWorldPosition(newPos);
                }
        );

        platformAnimate->runAnimateFunction(true);
    
            }
            {
            auto JumpPad = std::make_shared<GameObject>("JumpPad");
            sceneGraph.addNode(JumpPad);
            JumpPad->setLocalPosition(Vector3(-133.0f + offsetX, 37.5f + offsetY, 38.0f + offsetZ));
            JumpPad->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
            auto renderComp = JumpPad->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = JumpPad->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
        }
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-95.0f + offsetX, 22.0f + offsetY, -90.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 10.0f, 20.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) {
        {
            auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
            sceneGraph.addNode(wallRunWall);
            wallRunWall->setLocalPosition(Vector3(-140.0f + offsetX, 30.0f + offsetY, -30.0f + offsetZ));
            wallRunWall->setLocalScaling(Vector3(1.0f, 10.0f, 35.0f));
            auto renderComp = wallRunWall->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = wallRunWall->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
        }

        {
            auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
            sceneGraph.addNode(wallRunWall);
            wallRunWall->setLocalPosition(Vector3(-130.0f + offsetX, 32.0f + offsetY, 0.0f + offsetZ));
            wallRunWall->setLocalScaling(Vector3(1.0f, 20.0f, 40.0f));
            auto renderComp = wallRunWall->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = wallRunWall->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
        }
    }
    else {
        {
            auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
            sceneGraph.addNode(wallRunWall);
            wallRunWall->setLocalPosition(Vector3(-140.0f + offsetX, 30.0f + offsetY, -30.0f + offsetZ));
            wallRunWall->setLocalScaling(Vector3(1.0f, 10.0f, 30.0f));
            auto renderComp = wallRunWall->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = wallRunWall->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
        }

        {
            auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
            sceneGraph.addNode(wallRunWall);
            wallRunWall->setLocalPosition(Vector3(-130.0f + offsetX, 32.0f + offsetY, 0.0f + offsetZ));
            wallRunWall->setLocalScaling(Vector3(1.0f, 20.0f, 30.0f));
            auto renderComp = wallRunWall->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = wallRunWall->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
        }
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-140.0f + offsetX, 38.0f + offsetY, 30.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 20.0f, 32.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    //{
    //    auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
    //    sceneGraph.addNode(wallRunWall);
    //    wallRunWall->setLocalPosition(Vector3(-105.0f + offsetX, 20.0f + offsetY, 70.0f + offsetZ));
    //    wallRunWall->setLocalScaling(Vector3(1.0f, 10.0f, 30.0f));
    //    auto renderComp = wallRunWall->addComponent<Render3D>();
    //    renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
    //    auto shape = std::make_shared<OBB>();
    //    auto rigidBody = wallRunWall->addComponent<RigidBody>();
    //    rigidBody->setMass(0.0f)
    //        ->setDrag(1.0f)
    //        ->setShape(shape)
    //        ->setStatic(true)
    //        ->registerToPhysicsManager(PhysicsManager::Instance());
    //    rigidBody->initialize();
    //}

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(-125.0f + offsetX, 42.0f + offsetY, 45.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(30.0f, 10.0f, 1.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Jump Pad
    {
        auto JumpPad = std::make_shared<GameObject>("JumpPad");
        sceneGraph.addNode(JumpPad);
        JumpPad->setLocalPosition(Vector3(-103.0f + offsetX, 42 + offsetY, 38.0f + offsetZ));
        JumpPad->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = JumpPad->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = JumpPad->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Checkpoint5
    {
        auto checkPoint5 = std::make_shared<GameObject>("checkPoint5", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint5);
        checkPoint5->setLocalPosition(Vector3(-115.5f + offsetX, 42.0f + offsetY, 55.0f + offsetZ));
        checkPoint5->setLocalScaling(Vector3(8.0f, 1.0f, 8.0f));
        auto renderComp = checkPoint5->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint5->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Moving platform going up/down
    {
        auto MovingPlatform = std::make_shared<GameObject>("MovingPlatform");
        sceneGraph.addNode(MovingPlatform);
        MovingPlatform->setLocalPosition(Vector3(-130.0f + offsetX, 47 + offsetY, 54.0f + offsetZ));
        MovingPlatform->setLocalScaling(Vector3(8.0f, 1.0f, 8.0f));
        auto renderComp = MovingPlatform->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();

        auto rigidBody = MovingPlatform->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();

        auto platformAnimate = MovingPlatform->addComponent<Animate>();
        platformAnimate->setAnimateFunction(
                [currentTime = 0.0f, initialPos = MovingPlatform->getLocalPosition().y, phase = 1.0f](std::shared_ptr<GameObject> self, float deltaTime) mutable {

                    /* Affine Transformation Variables */
                    float affineSpeed = 5.0f;
                    float affinePosVarianceZ = 5.0f;

                    currentTime += deltaTime;

                    if ((phase == -1.0f) && (initialPos - affinePosVarianceZ) > self->getWorldPosition().y)
                        phase = 1.0f;
                    else if ((phase == 1.0f) && (initialPos + affinePosVarianceZ) < self->getWorldPosition().y)
                        phase = -1.0f;

                    Vector3 velocity = Vector3(0.0f, affineSpeed * phase, 0.0f);

                    Vector3 newPos = self->getWorldPosition() + (velocity * deltaTime);
                    self->setWorldPosition(newPos);
                }
        );

        platformAnimate->runAnimateFunction(true);
    }

    // Pillar 1
    {
        auto Pillar = std::make_shared<GameObject>("Pillar");
        sceneGraph.addNode(Pillar);
        Pillar->setLocalPosition(Vector3(-130.0f + offsetX, 26.0f + offsetY, 70.0f + offsetZ));
        Pillar->setLocalScaling(Vector3(12.0f, 52.0f, 12.0f));
        auto renderComp = Pillar->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = Pillar->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Pillar 2
    {
        auto JumpPad = std::make_shared<GameObject>("JumpPad");
        sceneGraph.addNode(JumpPad);
        JumpPad->setLocalPosition(Vector3(-130.0f + offsetX, 28.5f + offsetY, 90.0f + offsetZ));
        JumpPad->setLocalScaling(Vector3(12.0f, 57.0f, 12.0f));
        auto renderComp = JumpPad->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = JumpPad->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Pillar 3
    {
        auto JumpPad2 = std::make_shared<GameObject>("JumpPad2");
        sceneGraph.addNode(JumpPad2);
        JumpPad2->setLocalPosition(Vector3(-135.0f + offsetX, 31 + offsetY, 110.0f + offsetZ));
        JumpPad2->setLocalScaling(Vector3(12.0f, 62.0f, 12.0f));
        auto renderComp = JumpPad2->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = JumpPad2->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }


    //{
    //    std::random_device rd;
    //    std::mt19937 gen(rd());
    //    std::uniform_real_distribution<float> zDist(-30.0f, 30.0f);
    //    //std::uniform_real_distribution<float> tiltDist(-10.0f, 10.0f);
    //
    //    for (float x = -100.0f; x <= 130.0f; x += 30.0f) {
    //        auto JumpPad = std::make_shared<GameObject>("JumpPad");
    //        sceneGraph.addNode(JumpPad);
    //
    //        float randomZOffset = zDist(gen);
    //        //float tiltX = tiltDist(gen);
    //        //float tiltZ = tiltDist(gen);
    //        JumpPad->setLocalPosition(Vector3(x + offsetX, 65 + offsetY, 130.0f + randomZOffset + offsetZ));
    //        //JumpPad->setLocalRotation(Vector3(tiltX, 0.0f, tiltZ));
    //        JumpPad->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
    //
    //        auto renderComp = JumpPad->addComponent<Render3D>();
    //        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
    //
    //        auto shape = std::make_shared<OBB>();
    //        auto rigidBody = JumpPad->addComponent<RigidBody>();
    //        rigidBody->setMass(0.0f)
    //            ->setDrag(1.0f)
    //            ->setShape(shape)
    //            ->setStatic(true)
    //            ->registerToPhysicsManager(PhysicsManager::Instance());
    //        rigidBody->initialize();
    //    }
    //}
    // 

    // ----------------
    // Platforms Going in Circle
    // ----------------
    {
        auto createCircularPlatform = [&](const std::string& name, const Vector3& center, const Vector3& scale, float dirFactor, float initialAngle = 0.0f) {
            auto platform = std::make_shared<GameObject>(name);
            sceneGraph.addNode(platform);
            platform->setLocalPosition(center);
            platform->setLocalScaling(scale);
            platform->setLocalRotation(Vector3(0.0f, 0.0f, 0.0f));
            platform->addComponent<Render3D>()->setMesh(boxMesh)->setMaterial(concreteMaterial);
            platform->addComponent<RigidBody>()
                ->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(std::make_shared<OBB>())
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance())
                ->initialize();

            // Parameters for platform creation
            float radius = 20.0f;
            float angularSpeed = 0.5f;

            // Angular speed based on difficulty
            auto difficulty = LevelManager::Instance().getDifficulty();
            if (difficulty == FirstPersonControllerComponent::EASY) {
                angularSpeed = -0.5f;
            }
            else if (difficulty == FirstPersonControllerComponent::NORMAL) {
                angularSpeed = 0.5f;
            }
            else if (difficulty == FirstPersonControllerComponent::HARD) {
                angularSpeed = 1.5f;
            }

            platform->addComponent<Animate>()->setAnimateFunction(
                [angle = initialAngle, center, radius, angularSpeed, dirFactor](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                        angle += deltaTime * angularSpeed * dirFactor;
                        float xOffset = radius * std::cos(angle);
                        float zOffset = radius * std::sin(angle);
                        self->setLocalPosition(center + Vector3(xOffset, 0.0f, zOffset));
                }
            )->runAnimateFunction(true);
            };

        float baseY = (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) ? 62.0f : 65.0f;

        // First set
        float startX = -90.0f;
        float endX = 90.0f;
        float step = 90.0f;
        Vector3 scale(20.0f, 3.0f, 20.0f);
        int count = 0;
        for (float x = startX; x <= endX; x += step) {
            Vector3 center(x + offsetX, baseY + offsetY, 115.0f + offsetZ);

            // Left side
            for (int i = 0; i < 4; i++) {
                float initialAngle = i * (3.14159265f / 2.0f); // 0, π/2, π, 3π/2
                createCircularPlatform("Platform" + std::to_string(count) + "Left" + std::to_string(i), center, scale, 1.0f, initialAngle);
            }
            // Right side
            for (int i = 0; i < 4; i++) {
                float initialAngle = i * (3.14159265f / 2.0f) + 3.14159265f; // π, 3π/2, 2π, 5π/2
                createCircularPlatform("Platform" + std::to_string(count) + "Right" + std::to_string(i), center, scale, 1.0f, initialAngle);
            }
            count++;
        }

        // Second set
        count = 0;
        for (float x = startX; x <= endX - 10; x += step) {
            Vector3 center(x + offsetX + 45.0f, baseY + offsetY, (115.0f + offsetZ) + 50.0f);

            // Left side
            for (int i = 0; i < 4; i++) {
                float initialAngle = i * (3.14159265f / 2.0f);
                createCircularPlatform("PlatformAlt" + std::to_string(count) + "Left" + std::to_string(i), center, scale, -1.0f, initialAngle);
            }
            // Right side
            for (int i = 0; i < 4; i++) {
                float initialAngle = i * (3.14159265f / 2.0f) + 3.14159265f;
                createCircularPlatform("PlatformAlt" + std::to_string(count) + "Right" + std::to_string(i), center, scale, -1.0f, initialAngle);
            }
            count++;
        }
    }








    // Checkpoint6
    {
        auto checkPoint6 = std::make_shared<GameObject>("checkPoint6", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint6);
        checkPoint6->setLocalPosition(Vector3(130.5f + offsetX, 68.0f + offsetY, 130.0f + offsetZ));
        checkPoint6->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint6->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint6->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Moving platform going up
    {
        std::vector<std::shared_ptr<GameObject>> movingBlocks;
        float spawnInterval = 5.0f;
        int maxBlocks = 4;
        Vector3 startPos(130.0f, 65.0f, 123.0f);
        Vector3 velocity(0.0f, 1.0f, -5.0f);

        auto spawnTimer = std::make_shared<float>(0.0f);
        auto spawnedCount = std::make_shared<int>(0);

        auto spawner = std::make_shared<GameObject>("BlockSpawner");
        sceneGraph.addNode(spawner);

        spawner->addComponent<Animate>()->setAnimateFunction(
            [=](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                *spawnTimer += deltaTime;

                if (*spawnedCount < maxBlocks && *spawnTimer >= spawnInterval) {
                    *spawnTimer = 0.0f;

                    auto block = std::make_shared<GameObject>("MovingBlock_" + std::to_string(*spawnedCount));
                    sceneGraph.addNode(block);
                    block->setLocalPosition(startPos);
                    block->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
                    block->setLocalRotation(Vector3(0.0f, 0.0f, 0.0f));

                    block->addComponent<Render3D>()->setMesh(boxMesh)->setMaterial(concreteMaterial);

                    block->addComponent<RigidBody>()
                        ->setMass(0.0f)
                        ->setDrag(1.0f)
                        ->setShape(std::make_shared<OBB>())
                        ->setStatic(true)
                        ->registerToPhysicsManager(PhysicsManager::Instance())
                        ->initialize();

                    block->addComponent<Animate>()->setAnimateFunction(
                        [velocity, startPos, time = 0.0f](std::shared_ptr<GameObject> self, float dt) mutable {
    time += dt;
    if (time >= 20.0f) {
    self->setLocalPosition(startPos);
    time = 0.0f;
    }
    else {
    self->setLocalPosition(self->getLocalPosition() + velocity * dt);
    }
                        }
                    )->runAnimateFunction(true);

                    movingBlocks.push_back(block);
                    (*spawnedCount)++;
                }
            }
        )->runAnimateFunction(true);
    }

    // Moving platform going down
    {
        std::vector<std::shared_ptr<GameObject>> movingBlocks;
        float spawnInterval = 5.0f;
        int maxBlocks = 4;
        Vector3 startPos(143.0f, 85.0f, 23.0f);
        Vector3 velocity(0.0f, -1.0f, 5.0f);

        auto spawnTimer = std::make_shared<float>(0.0f);
        auto spawnedCount = std::make_shared<int>(0);

        auto spawner = std::make_shared<GameObject>("BlockSpawner");
        sceneGraph.addNode(spawner);

        spawner->addComponent<Animate>()->setAnimateFunction(
            [=](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                *spawnTimer += deltaTime;

                if (*spawnedCount < maxBlocks && *spawnTimer >= spawnInterval) {
                    *spawnTimer = 0.0f;

                    auto block = std::make_shared<GameObject>("MovingBlock_" + std::to_string(*spawnedCount));
                    sceneGraph.addNode(block);
                    block->setLocalPosition(startPos);
                    block->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
                    block->setLocalRotation(Vector3(0.0f, 0.0f, 0.0f));

                    block->addComponent<Render3D>()->setMesh(boxMesh)->setMaterial(concreteMaterial);

                    block->addComponent<RigidBody>()
                        ->setMass(0.0f)
                        ->setDrag(1.0f)
                        ->setShape(std::make_shared<OBB>())
                        ->setStatic(true)
                        ->registerToPhysicsManager(PhysicsManager::Instance())
                        ->initialize();

                    // Animate block movement and reset after 20 seconds
                    block->addComponent<Animate>()->setAnimateFunction(
                        [velocity, startPos, time = 0.0f](std::shared_ptr<GameObject> self, float dt) mutable {
    time += dt;
    if (time >= 20.0f) {
    self->setLocalPosition(startPos);
    time = 0.0f;
    }
    else {
    self->setLocalPosition(self->getLocalPosition() + velocity * dt);
    }
                        }
                    )->runAnimateFunction(true);

                    movingBlocks.push_back(block);
                    (*spawnedCount)++;
                }
            }
        )->runAnimateFunction(true);
    }

    // Extra checkpoint
    if (LevelManager::Instance().getDifficulty() != FirstPersonControllerComponent::HARD) {

        auto extraCheckpoint = std::make_shared<GameObject>("extraCheckpoint", GameObject::CHECKPOINT);
        sceneGraph.addNode(extraCheckpoint);
        extraCheckpoint->setLocalPosition(Vector3(130 + offsetX, 63.0f + offsetY, -73.0f + offsetZ));
        extraCheckpoint->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = extraCheckpoint->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = extraCheckpoint->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();

    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(153.0f + offsetX, 90.0f + offsetY, 15.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 10.0f, 30.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Taller wall,easier to land in easy mode 
    if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(125.0f + offsetX, 68.0f + offsetY, -40.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 50.0f, 50.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }
    else
    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(125.0f + offsetX, 68.0f + offsetY, -40.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(1.0f, 10.0f, 50.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto wallRunWall = std::make_shared<GameObject>("WallRunWall", GameObject::RUNNABLE_WALL);
        sceneGraph.addNode(wallRunWall);
        wallRunWall->setLocalPosition(Vector3(100.0f + offsetX, 68.0f + offsetY, -65.0f + offsetZ));
        wallRunWall->setLocalScaling(Vector3(30.0f, 10.0f, 1.0f));
        auto renderComp = wallRunWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = wallRunWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }


    // Checkpoint7
    {
        auto checkPoint7 = std::make_shared<GameObject>("checkPoint7", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint7);
        checkPoint7->setLocalPosition(Vector3(78.0f + offsetX, 63.0f + offsetY, -72.0f + offsetZ));
        checkPoint7->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint7->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint7->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto createFerrisWheel = [&](const std::string& name, const Vector3& center, int platformCount, float radius, float speed) {
            for (int i = 0; i < platformCount; ++i) {
                float initialAngle = (2.0f * 3.14159265f / platformCount) * i;
                std::string platformName = name + "_" + std::to_string(i);
                auto platform = std::make_shared<GameObject>(platformName);
                sceneGraph.addNode(platform);
                float xOffset = radius * std::cos(initialAngle);
                float yOffset = radius * std::sin(initialAngle);
                platform->setLocalPosition(center + Vector3(xOffset, yOffset, 0.0f));
                platform->setLocalScaling(Vector3(16.0f, 4.0f, 16.0f));
                platform->setLocalRotation(Vector3(0.0f, 0.0f, 0.0f));
                platform->addComponent<Render3D>()->setMesh(boxMesh)->setMaterial(concreteMaterial);
                auto rigidBody = platform->addComponent<RigidBody>();
                auto shape = std::make_shared<OBB>();
                rigidBody->setMass(0.0f)->setDrag(1.0f)->setShape(shape)->setStatic(true)->registerToPhysicsManager(PhysicsManager::Instance());
                rigidBody->initialize();
                platform->addComponent<Animate>()->setAnimateFunction(
                    [angle = initialAngle, center, radius, speed](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                            angle += deltaTime * speed;
                            float newX = center.x + radius * std::cos(angle);
                            float newY = center.y + radius * std::sin(angle);
                            self->setLocalPosition(Vector3(newX, newY, center.z));
                    }
                )->runAnimateFunction(true);
            }
        };

        //writeLetter(sceneGraph, "media/Map/words/arrow2.fbx", Vector3(75.0f + offsetX, 64.5f + offsetY, -77.0f + offsetZ),
        //        Vector3(0.1f, 0.1f, 0.01f), Vector3(0.0f, 0.0f, 5.7f), BrownConcrete);

        createFerrisWheel("Wheel1", Vector3(57.0f + offsetX, 70.0f + offsetY, -86.0f + offsetZ), 3, 20.0f, 5.3f);
    }

    writeLetter(sceneGraph, "media/Map/words/arrow2.fbx", Vector3(57.0f + offsetX, 73.0f + offsetY, -100.0f + offsetZ), Vector3(0.03f, 0.03f, 0.01f), Vector3(0.0f, 0.0f, -0.57f), wordMaterial);

    writeLetter(sceneGraph, "media/Map/words/arrow2.fbx", Vector3(90.0f + offsetX, 110.0f + offsetY, -86.0f + offsetZ), Vector3(0.03f, 0.03f, 0.01f), Vector3(0.0f, 1.57f, 0.0f), wordMaterial);

    writeLetter(sceneGraph, "media/Map/words/arrow2.fbx", Vector3(107.0f + offsetX, 150.0f + offsetY, -86.0f + offsetZ), Vector3(0.03f, 0.03f, 0.01f), Vector3(0.0f, 1.57f, -2.57f), wordMaterial);


    {
        // Guide Image
        {
            auto image = std::make_shared<GameObject>("image");
            sceneGraph.addNode(image);
            image->setLocalPosition(Vector3(72.0f + offsetX, 68 + offsetY, -72.0f + offsetZ));
            image->setLocalScaling(Vector3(0.1f, 8.0f, 8.0f));
            auto renderComp = image->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(jumpImage);
        }
    }
    if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) {
        {
            auto extraPillar = std::make_shared<GameObject>("extraPillar");
            sceneGraph.addNode(extraPillar);
            extraPillar->setLocalPosition(Vector3(96 + offsetX, 63.0f + offsetY, -85.0f + offsetZ));
            extraPillar->setLocalScaling(Vector3(12.0f, 126.0f, 12.0f));
            auto renderComp = extraPillar->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
            auto shape = std::make_shared<OBB>();
            auto rigidBody = extraPillar->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();

        }
    }

    {
        auto checkPoint8 = std::make_shared<GameObject>("checkPoint8", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint8);
        checkPoint8->setLocalPosition(Vector3(120.5f + offsetX, 140.0f + offsetY, -86.0f + offsetZ));
        checkPoint8->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint8->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint8->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();

    }

    {
        auto deathPlane = std::make_shared<GameObject>("deathPlane");
        sceneGraph.addNode(deathPlane);
        deathPlane->setLocalPosition(Vector3(120.0f + offsetX, 160.0f + offsetY, 10.0f + offsetZ));
        deathPlane->setLocalScaling(Vector3(90.0f, 1.0f, 300.0f));
        auto renderComp = deathPlane->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(cracksMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = deathPlane->addComponent<DeathPlane>();
        rigidBody->setPlayerName("PlayerBox");
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    //{
    //    auto deathPlane = std::make_shared<GameObject>("deathPlane");
    //    sceneGraph.addNode(deathPlane);
    //    deathPlane->setLocalPosition(Vector3(120.0f + offsetX, 130.0f + offsetY, 25.0f + offsetZ));
    //    deathPlane->setLocalScaling(Vector3(30.0f, 1.0f, 190.0f));
    //    auto renderComp = deathPlane->addComponent<Render3D>();
    //    renderComp->setMesh(boxMesh)->setMaterial(cracksMaterial);
    //    auto shape = std::make_shared<OBB>();
    //    auto rigidBody = deathPlane->addComponent<DeathPlane>();
    //    rigidBody->setPlayerName("PlayerBox");
    //    rigidBody->setMass(0.0f)
    //        ->setDrag(1.0f)
    //        ->setShape(shape)
    //        ->setStatic(true)
    //        ->registerToPhysicsManager(PhysicsManager::Instance());
    //    rigidBody->initialize();
    //}

    {

        auto movingWallRun = [&](const std::string& name, const Vector3& basePos, const Vector3& scale, const Vector3& moveDir, float offset = 0.0f) {
            auto wall = std::make_shared<GameObject>(name, GameObject::RUNNABLE_WALL);
            sceneGraph.addNode(wall);
            wall->setLocalPosition(basePos);
            wall->setLocalScaling(scale);
            auto renderComp = wall->addComponent<Render3D>();
            renderComp->setMesh(boxMesh)->setMaterial(BlueConcrete);
            auto rigidBody = wall->addComponent<RigidBody>();
            auto shape = std::make_shared<OBB>();
            rigidBody->setMass(0.0f)->setDrag(1.0f)->setShape(shape)->setStatic(true)->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
            wall->addComponent<Animate>()->setAnimateFunction(
                [elapsedTime = offset, basePos, moveDir](std::shared_ptr<GameObject> self, float dt) mutable {
                    elapsedTime += dt;
                    self->setLocalPosition(basePos + moveDir * std::sin(elapsedTime));
                }
            )->runAnimateFunction(true);

            };

        if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::HARD) {
            movingWallRun("wallRight1", Vector3(105.0f + offsetX, 160.0f + offsetY, -50.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, 15.0f, 0.0f));
            movingWallRun("wallLeft1", Vector3(135.0f + offsetX, 160.0f + offsetY, -50.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, -15.0f, 0.0f));
            movingWallRun("wallRight2", Vector3(105.0f + offsetX, 160.0f + offsetY, -15.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, -15.0f, 0.0f));
            movingWallRun("wallLeft2", Vector3(135.0f + offsetX, 160.0f + offsetY, -15.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, 15.0f, 0.0f));
            movingWallRun("wallRight3", Vector3(105.0f + offsetX, 160.0f + offsetY, 20.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, 15.0f, 0.0f));
            movingWallRun("wallLeft3", Vector3(135.0f + offsetX, 160.0f + offsetY, 20.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, -15.0f, 0.0f));

            movingWallRun("wallRight4", Vector3(105.0f + offsetX, 160.0f + offsetY, 55.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, 15.0f, 0.0f));
            movingWallRun("wallLeft4", Vector3(135.0f + offsetX, 160.0f + offsetY, 55.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, -15.0f, 0.0f));
            movingWallRun("wallRight5", Vector3(105.0f + offsetX, 160.0f + offsetY, 90.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, 15.0f, 0.0f));
            movingWallRun("wallLeft5", Vector3(135.0f + offsetX, 160.0f + offsetY, 90.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, -15.0f, 0.0f));
            movingWallRun("wallRight6", Vector3(105.0f + offsetX, 160.0f + offsetY, 125.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, 15.0f, 0.0f));
            movingWallRun("wallLeft6", Vector3(135.0f + offsetX, 160.0f + offsetY, 125.0f + offsetZ), Vector3(1.0f, 35.0f, 25.0f), Vector3(0.0f, -15.0f, 0.0f));

            movingWallRun("wallLeft7", Vector3(135.0f + offsetX, 170.0f + offsetY, -100.0f + offsetZ), Vector3(1.0f, 20.0f, 25.0f), Vector3(0.0f, 0.0f, 20.0f));
            movingWallRun("wallRight7", Vector3(105.0f + offsetX, 170.0f + offsetY, -100.0f + offsetZ), Vector3(1.0f, 20.0f, 25.0f), Vector3(0.0f, 0.0f, -20.0f));
        }
        else {
            
            movingWallRun("wallRight1", Vector3(113.0f + offsetX, 160.0f + offsetY, -50.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, 15.0f, 0.0f), 0.0f);
            movingWallRun("wallLeft2", Vector3(127.0f + offsetX, 160.0f + offsetY, -15.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, 15.0f, 0.0f), 5.0f);
            movingWallRun("wallRight3", Vector3(113.0f + offsetX, 160.0f + offsetY, 20.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, 15.0f, 0.0f), 10.0f);
            movingWallRun("wallLeft4", Vector3(127.0f + offsetX, 160.0f + offsetY, 55.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, 15.0f, 0.0f), 15.0f);
            movingWallRun("wallRight5", Vector3(113.0f + offsetX, 160.0f + offsetY, 90.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, 15.0f, 0.0f), 20.0f);
            movingWallRun("wallLeft6", Vector3(127.0f + offsetX, 160.0f + offsetY, 125.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, 15.0f, 0.0f), 25.0f);
            
            movingWallRun("wallLeft1", Vector3(127.0f + offsetX, 160.0f + offsetY, -50.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, -15.0f, 0.0f), 0.0f);
            movingWallRun("wallRight2", Vector3(113.0f + offsetX, 160.0f + offsetY, -15.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, -15.0f, 0.0f), 5.0f);
            movingWallRun("wallLeft3", Vector3(127.0f + offsetX, 160.0f + offsetY, 20.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, -15.0f, 0.0f), 10.0f);
            movingWallRun("wallRight4", Vector3(113.0f + offsetX, 160.0f + offsetY, 55.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, -15.0f, 0.0f), 15.0f);
            movingWallRun("wallLeft5", Vector3(127.0f + offsetX, 160.0f + offsetY, 90.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, -15.0f, 0.0f), 20.0f);
            movingWallRun("wallRight6", Vector3(113.0f + offsetX, 160.0f + offsetY, 125.0f + offsetZ), Vector3(1.0f, 35.0f, 35.0f), Vector3(0.0f, -15.0f, 0.0f), 25.0f);


            movingWallRun("wallLeft4", Vector3(127.0f + offsetX, 170.0f + offsetY, -100.0f + offsetZ), Vector3(1.0f, 20.0f, 25.0f), Vector3(0.0f, 0.0f, 20.0f));
            movingWallRun("wallReft4", Vector3(113.0f + offsetX, 170.0f + offsetY, -100.0f + offsetZ), Vector3(1.0f, 20.0f, 25.0f), Vector3(0.0f, 0.0f, -20.0f));
            {
                auto extraCheckpoint = std::make_shared<GameObject>("extraCheckpoint", GameObject::CHECKPOINT);
                sceneGraph.addNode(extraCheckpoint);
                extraCheckpoint->setLocalPosition(Vector3(120.5f + offsetX, 130.0f + offsetY, 20.0f + offsetZ));
                extraCheckpoint->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
                auto renderComp = extraCheckpoint->addComponent<Render3D>();
                renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
                auto shape = std::make_shared<OBB>();
                auto rigidBody = extraCheckpoint->addComponent<RigidBody>();
                rigidBody->setMass(0.0f)
                    ->setDrag(1.0f)
                    ->setShape(shape)
                    ->setStatic(true)
                    ->registerToPhysicsManager(PhysicsManager::Instance());
                rigidBody->initialize();

            }
        }
        

    }

    {
        auto createFerrisWheelZ = [&](const std::string& name, const Vector3& center, int platformCount, float radius, float speed) {
            for (int i = 0; i < platformCount; ++i) {
                float initialAngle = (2.0f * 3.14159265f / platformCount) * i;
                std::string platformName = name + "_" + std::to_string(i);
                auto platform = std::make_shared<GameObject>(platformName);
                sceneGraph.addNode(platform);
                float yOffset = radius * std::cos(initialAngle);
                float zOffset = radius * std::sin(initialAngle);
                platform->setLocalPosition(center + Vector3(0.0f, yOffset, zOffset));
                platform->setLocalScaling(Vector3(15.0f, 2.0f, 15.0f));
                platform->addComponent<Render3D>()->setMesh(boxMesh)->setMaterial(concreteMaterial);
                auto rigidBody = platform->addComponent<RigidBody>();
                auto shape = std::make_shared<OBB>();
                rigidBody->setMass(0.0f)->setDrag(1.0f)->setShape(shape)->setStatic(true)
                    ->registerToPhysicsManager(PhysicsManager::Instance());
                rigidBody->initialize();
                platform->addComponent<Animate>()->setAnimateFunction(
                    [angle = initialAngle, center, radius, speed](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                        angle += deltaTime * speed;
                        float newY = center.y + radius * std::cos(angle);
                        float newZ = center.z + radius * std::sin(angle);
                        self->setLocalPosition(Vector3(center.x, newY, newZ));
                    }
                )->runAnimateFunction(true);
            }
            };

        if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::HARD) {
            createFerrisWheelZ("FerrisPlatformZ", Vector3(120.0f + offsetX, 160.0f + offsetY, 151.0f + offsetZ), 8, 30.0f, 0.2f);
            createFerrisWheelZ("FerrisPlatformZ", Vector3(150.0f + offsetX, 160.0f + offsetY, 151.0f + offsetZ), 8, 30.0f, 0.1f);
        }
        else {
            createFerrisWheelZ("FerrisPlatformZ", Vector3(120.0f + offsetX, 160.0f + offsetY, 151.0f + offsetZ), 8, 30.0f, -0.7f);
        }


    }

    {
        auto checkPoint9 = std::make_shared<GameObject>("checkPoint9", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint9);
        if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::HARD) {
            checkPoint9->setLocalPosition(Vector3(120.5f + offsetX, 162.0f + offsetY, 151.0f + offsetZ));
        }
        else {
            checkPoint9->setLocalPosition(Vector3(120.0f + offsetX, 162.0f + offsetY, 45.0f + offsetZ));
        }
        
        checkPoint9->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint9->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint9->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();

    }

    {
        auto checkPoint10 = std::make_shared<GameObject>("checkPoint10", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint10);
        checkPoint10->setLocalPosition(Vector3(120.5f + offsetX, 162.0f + offsetY, -86.0f + offsetZ));
        checkPoint10->setLocalScaling(Vector3(12.0f, 1.0f, 12.0f));
        auto renderComp = checkPoint10->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint10->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();

    }

    if (LevelManager::Instance().getDifficulty() == FirstPersonControllerComponent::EASY) {
        auto extraCheckpoint = std::make_shared<GameObject>("extraCheckpoint", GameObject::CHECKPOINT);
        sceneGraph.addNode(extraCheckpoint);
        extraCheckpoint->setLocalPosition(Vector3(120.5f + offsetX, 162.0f + offsetY, -135.0f + offsetZ));
        extraCheckpoint->setLocalScaling(Vector3(12.0f, 1.0f, 30.0f));
        auto renderComp = extraCheckpoint->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = extraCheckpoint->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto movingWallRun2 = [&](const std::string& name, const Vector3& basePos, const Vector3& scale, const Vector3& moveDir, int platformCount) {
            std::vector<std::shared_ptr<GameObject>> movingWalls;
            float spawnInterval = 5.0f;
            auto spawnTimer = std::make_shared<float>(0.0f);
            auto spawnedCount = std::make_shared<int>(0);

            auto spawner = std::make_shared<GameObject>("Spawner_" + name);
            sceneGraph.addNode(spawner);

            spawner->addComponent<Animate>()->setAnimateFunction(
                [=](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                        *spawnTimer += deltaTime;
                        if (*spawnedCount < platformCount && *spawnTimer >= spawnInterval) {
                            *spawnTimer = 0.0f;
                            std::string wallName = name + "_" + std::to_string(*spawnedCount);
                            auto wall = std::make_shared<GameObject>(wallName, GameObject::RUNNABLE_WALL);
                            sceneGraph.addNode(wall);
                            wall->setLocalPosition(basePos);
                            wall->setLocalScaling(scale);
                            wall->setLocalRotation(Vector3(0.0f, 0.0f, 0.0f));
                            wall->addComponent<Render3D>()->setMesh(boxMesh)->setMaterial(BlueConcrete);
                            auto rigidBody = wall->addComponent<RigidBody>();
                            rigidBody->setMass(0.0f)
                                ->setDrag(1.0f)
                                ->setShape(std::make_shared<OBB>())
                                ->setStatic(true)
                                ->registerToPhysicsManager(PhysicsManager::Instance());
                            rigidBody->initialize();
                            wall->addComponent<Animate>()->setAnimateFunction(
                                [moveVec = moveDir, basePos, time = 0.0f](std::shared_ptr<GameObject> self, float dt) mutable {
                                            time += dt;
                                            if (time >= 25.0f) {
                                                self->setLocalPosition(basePos);
                                                time = 0.0f;
                                            }
                                            else {
                                                self->setLocalPosition(self->getLocalPosition() + moveVec * dt);
                                            }
                                }
                            )->runAnimateFunction(true);
                            movingWalls.push_back(wall);
                            (*spawnedCount)++;
                        }
                }
            )->runAnimateFunction(true);
            };

        movingWallRun2("Wall1", Vector3(250.0f, 170.0f, -155.0f), Vector3(40.0f, 20.0f, 1.0f), Vector3(-20.0f, 0.0f, 0.0f), 5);
        movingWallRun2("Wall2", Vector3(200.0f, 170.0f, -140.0f), Vector3(40.0f, 20.0f, 1.0f), Vector3(-20.0f, 0.0f, 0.0f), 5);

        //movingWallRun2("Wall3", Vector3(-172.0f, 140.0f, -210.0f), Vector3(1.0f, 20.0f, 40.0f), Vector3(0.0f, 0.0f, 30.0f), 8);
    }

    {
        auto checkPoint11 = std::make_shared<GameObject>("checkPoint11", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkPoint11);
        checkPoint11->setLocalPosition(Vector3(-165.0f + offsetX, 135.0f + offsetY, -148.0f + offsetZ));
        checkPoint11->setLocalScaling(Vector3(20.0f, 1.0f, 20.0f));
        auto renderComp = checkPoint11->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(WhiteFloorTiles);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkPoint11->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();

    }



}
