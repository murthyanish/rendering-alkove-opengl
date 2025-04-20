/*!****************************************************************************
 * \file   Map_Intermediate.cpp
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

void MapLoader::intermediate(
    float offsetX, float offsetY, float offsetZ,
    SceneGraph& sceneGraph) {

    // Keys & Doors test

    {
        std::shared_ptr<Mesh> keyMesh = Mesh::loadMesh("media/Map/key.fbx");
        auto key = std::make_shared<GameObject>("key", GameObject::WALL);
        sceneGraph.addNode(key);
        key->setLocalPosition(Vector3(-151.0f + offsetX, 5.5f + offsetY, offsetZ));
        key->setLocalScaling(Vector3(0.005f, 0.005f, 0.005f));
        auto renderComp = key->addComponent<Render3D>();
        renderComp->setMesh(keyMesh)->setMaterial(keyMaterial);

        auto shape = std::make_shared<OBB>(Vector3(0, 0, 0), Vector3(1.0f / 0.005f, 2.0f / 0.005f, 1.0f / 0.005f));
        auto keyComp = key->addComponent<InGameKey>();
        keyComp->setID(1);
        keyComp->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        keyComp->initialize();

        key->addComponent<Animate>()->setAnimateFunction(
             [rotationSpeed = 45.0f, amplitude = 1.0f, currentTime = 0.0f, initialPos = key->getLocalPosition()]
             (std::shared_ptr<GameObject> self, float deltaTime) mutable {
                 currentTime += deltaTime;
                 float yOffset = amplitude * std::sin(currentTime);
                 Vector3 newPos = initialPos;
                 newPos.y += yOffset;
                 self->setLocalPosition(newPos);
                 float radians = rotationSpeed * deltaTime * (3.14159265f / 180.0f);
                 self->setLocalRotation(Quaternion::fromEuler(0.0f, radians, 0.0f) * self->getLocalRotation());
             }
        )->runAnimateFunction(true);
    }

    {
        auto testDoor = std::make_shared<GameObject>("TestDoor", GameObject::WALL);
        sceneGraph.addNode(testDoor);
        testDoor->setLocalPosition(Vector3(-189.7f + offsetX, 8.5f + offsetY, 0.0f + offsetZ));
        testDoor->setLocalScaling(Vector3(0.3f, 5.0f, 3.0f));
        auto renderComp = testDoor->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(BrownConcrete);
        auto shape = std::make_shared<OBB>();
        auto doorComp = testDoor->addComponent<Door>();
        doorComp->setID(1);
        doorComp->setType(Door::DoorType::NEXTLEVEL);
        doorComp->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        doorComp->initialize();
    }
    // Room walls
    {
        auto LeftWall = std::make_shared<GameObject>("LeftWall", GameObject::WALL);
        sceneGraph.addNode(LeftWall);
        LeftWall->setLocalPosition(Vector3(-200.0f + offsetX, 4.0f + offsetY, 10.1f + offsetZ));
        LeftWall->setLocalScaling(Vector3(420.0f, 150.0f, 1.0f));
        auto renderComp = LeftWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = LeftWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto RightWall = std::make_shared<GameObject>("RightWall", GameObject::WALL);
        sceneGraph.addNode(RightWall);
        RightWall->setLocalPosition(Vector3(-200.0f + offsetX, 4.0f + offsetY, -10.1f + offsetZ));
        RightWall->setLocalScaling(Vector3(420.0f, 150.0f, 1.0f));
        auto renderComp = RightWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = RightWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto BackWall = std::make_shared<GameObject>("BackWall", GameObject::WALL);
        sceneGraph.addNode(BackWall);
        BackWall->setLocalPosition(Vector3(10.5f + offsetX, 4.0f + offsetY, 0.0f + offsetZ));
        BackWall->setLocalScaling(Vector3(1.0f, 150.0f, 20.0f));
        auto renderComp = BackWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = BackWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto FrontWall = std::make_shared<GameObject>("FrontWall", GameObject::WALL);
        sceneGraph.addNode(FrontWall);
        FrontWall->setLocalPosition(Vector3(-190.5f + offsetX, 4.0f + offsetY, 0.0f + offsetZ));
        FrontWall->setLocalScaling(Vector3(1.0f, 150.0f, 20.0f));
        auto renderComp = FrontWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = FrontWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    {
        auto TopWall = std::make_shared<GameObject>("TopWall", GameObject::WALL);
        sceneGraph.addNode(TopWall);
        TopWall->setLocalPosition(Vector3(-200.0f + offsetX, 50.0f + offsetY, 0.0f + offsetZ));
        TopWall->setLocalScaling(Vector3(420.0f, 10.0f, 20.0f));
        auto renderComp = TopWall->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = TopWall->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }


    // deathplane 
    {
        auto DP1 = std::make_shared<GameObject>("DP1");
        sceneGraph.addNode(DP1);
        DP1->setLocalPosition(Vector3(-200.0f + offsetX, -50.0f + offsetY, 0.0f + offsetZ));
        DP1->setLocalScaling(Vector3(420.0f, 1.0f, 20.0f));
        //DP1->setLocalRotation(Vector3(0.0f, 1.57f, 0.0f));
        auto renderComp = DP1->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(cracksMaterial);
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
    // Starting platform
    {
        auto mainFloor = std::make_shared<GameObject>("MainFloor", GameObject::CHECKPOINT);
        sceneGraph.addNode(mainFloor);
        mainFloor->setLocalPosition(Vector3(-0.0f + offsetX, 0.0f + offsetY, -0.0f + offsetZ));
        mainFloor->setLocalScaling(Vector3(10.0f, 1.0f, 10.0f));
        auto renderComp = mainFloor->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = mainFloor->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    float platformSizes[] = { 4.0f, 3.5f, 3.0f, 3.2f, 3.6f, 3.1f, 3.4f, 3.7f };
    float yVariation[] = { 0.3f, -5.2f, -3.5f, -1.4f, 0.f, 0.0f, 3.4f, 7.2f };
    float xSpacing = 8.0f;
    float xOffset[] = { 2.0f, 2.0f, -3.0f, 3.0f, -4.0f, 5.0f, -5.0f, 5.0f };

    for (int i = 0; i < 8; ++i) {
        auto platform = std::make_shared<GameObject>("Platform" + std::to_string(i + 1));
        sceneGraph.addNode(platform);
        float adjustedX = (i == 0) ? -(xSpacing * 1.5f) : -((i + 1) * xSpacing + 5.0f);
        platform->setLocalPosition(Vector3(adjustedX + offsetX, yVariation[i] + offsetY, -xOffset[i] + offsetZ));
        platform->setLocalScaling(Vector3(8.0f, 3.0f, 8.0f));
        auto renderComp = platform->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();

        if (i == 5 || i == 6) {
            auto rigidBody = platform->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
            auto platformAnimate = platform->addComponent<Animate>();
            float direction = (i == 5) ? -1.0f : 1.0f;
            platformAnimate->setAnimateFunction(
                [currentTime = 0.0f, initialPos = platform->getLocalPosition().x, phase = direction](std::shared_ptr<GameObject> self, float deltaTime) mutable {
                    float affineSpeed = 5.0f;
                    float affinePosVarianceX = 5.0f;

                    currentTime += deltaTime;

                    if ((phase == -1.0f) && (initialPos - affinePosVarianceX) > self->getWorldPosition().x)
                        phase = 1.0f;
                    else if ((phase == 1.0f) && (initialPos + affinePosVarianceX) < self->getWorldPosition().x)
                        phase = -1.0f;

                    Vector3 velocity = Vector3(affineSpeed * phase, 0.0f, 0.0f);
                    Vector3 newPos = self->getWorldPosition() + (velocity * deltaTime);
                    self->setWorldPosition(newPos);
                }
            );
            platformAnimate->runAnimateFunction(true);
        }
        if (i == 7) {
            auto rigidBody = platform->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();

            auto platformAnimate = platform->addComponent<Animate>();
            platformAnimate->setAnimateFunction(
                  [currentTime = 0.0f, initialPos = platform->getLocalPosition().y, phase = 1.0f](std::shared_ptr<GameObject> self, float deltaTime) mutable {

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
        else {
            auto rigidBody = platform->addComponent<RigidBody>();
            rigidBody->setMass(0.0f)
                ->setDrag(1.0f)
                ->setShape(shape)
                ->setStatic(true)
                ->registerToPhysicsManager(PhysicsManager::Instance());
            rigidBody->initialize();
        }
    }

    // Checkpoint
    {
        auto checkpoint = std::make_shared<GameObject>("checkpoint", GameObject::CHECKPOINT);
        sceneGraph.addNode(checkpoint);
        checkpoint->setLocalPosition(Vector3(-80.0f + offsetX, 11.0f + offsetY, 0.0f + offsetZ));
        checkpoint->setLocalScaling(Vector3(12.0f, 3.0f, 12.0f));
        auto renderComp = checkpoint->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkpoint->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Wall
    {
        auto upperFloor = std::make_shared<GameObject>("upperFloor");
        sceneGraph.addNode(upperFloor);
        upperFloor->setLocalPosition(Vector3(-125.0f + offsetX, 30.0f + offsetY, 0.0f + offsetZ));
        upperFloor->setLocalScaling(Vector3(50.0f, 40.0f, 20.0f));
        auto renderComp = upperFloor->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(LightBlueConcrete);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = upperFloor->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    // Under Wall
    {
        auto lowerFloor = std::make_shared<GameObject>("lowerFloor", GameObject::CHECKPOINT);
        sceneGraph.addNode(lowerFloor);
        lowerFloor->setLocalPosition(Vector3(-125.0f + offsetX, -5.0f + offsetY, 0.0f + offsetZ));
        lowerFloor->setLocalScaling(Vector3(35.0f, 10.0f, 17.0f));
        auto renderComp = lowerFloor->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = lowerFloor->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }

    //// Wall
    //{
    //    auto wall = std::make_shared<GameObject>("wall");
    //    sceneGraph.addNode(wall);
    //    wall->setLocalPosition(Vector3(-115.0f + offsetX, -10.0f + offsetY, 0.0f + offsetZ));
    //    wall->setLocalScaling(Vector3(1.0f, 7.0f, 17.0f));
    //    auto renderComp = wall->addComponent<Render3D>();
    //    renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
    //    auto shape = std::make_shared<OBB>();
    //    auto rigidBody = wall->addComponent<RigidBody>();
    //    rigidBody->setMass(0.0f)
    //        ->setDrag(1.0f)
    //        ->setShape(shape)
    //        ->setStatic(true)
    //        ->registerToPhysicsManager(PhysicsManager::Instance());
    //    rigidBody->initialize();
    //}

    //// Wall2
    //{
    //    auto wall = std::make_shared<GameObject>("wall");
    //    sceneGraph.addNode(wall);
    //    wall->setLocalPosition(Vector3(-125.0f + offsetX, -5.0f + offsetY, 3.5f + offsetZ));
    //    wall->setLocalScaling(Vector3(1.0f, 10.0f, 10.0f));
    //    auto renderComp = wall->addComponent<Render3D>();
    //    renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
    //    auto shape = std::make_shared<OBB>();
    //    auto rigidBody = wall->addComponent<RigidBody>();
    //    rigidBody->setMass(0.0f)
    //        ->setDrag(1.0f)
    //        ->setShape(shape)
    //        ->setStatic(true)
    //        ->registerToPhysicsManager(PhysicsManager::Instance());
    //    rigidBody->initialize();
    //}

    //{
    //    auto wall = std::make_shared<GameObject>("wall");
    //    sceneGraph.addNode(wall);
    //    wall->setLocalPosition(Vector3(-135.0f + offsetX, -5.0f + offsetY, -3.5f + offsetZ));
    //    wall->setLocalScaling(Vector3(1.0f, 10.0f, 10.0f));
    //    auto renderComp = wall->addComponent<Render3D>();
    //    renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
    //    auto shape = std::make_shared<OBB>();
    //    auto rigidBody = wall->addComponent<RigidBody>();
    //    rigidBody->setMass(0.0f)
    //        ->setDrag(1.0f)
    //        ->setShape(shape)
    //        ->setStatic(true)
    //        ->registerToPhysicsManager(PhysicsManager::Instance());
    //    rigidBody->initialize();
    //}

    // Moving platform going up
   /* {
        std::vector<std::shared_ptr<GameObject>> movingBlocks;
        float spawnInterval = 5.0f;
        int maxBlocks = 3;
        Vector3 startPos(-138.0f, -1.0f, -6.0f);
        Vector3 velocity(-3.0f, 0.41f, 0.0f);

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
                    block->setLocalScaling(Vector3(5.0f, 2.5f, 5.0f));
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
                            if (time >= 15.0f) {
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
    }*/

    // Checkpoint 2
    {
        auto checkpoint2 = std::make_shared<GameObject>("checkpoint2");
        sceneGraph.addNode(checkpoint2);
        checkpoint2->setLocalPosition(Vector3(-180.0f + offsetX, -6.0f + offsetY, 0.0f + offsetZ));
        checkpoint2->setLocalScaling(Vector3(20.0f, 25.0f, 17.0f));
        auto renderComp = checkpoint2->addComponent<Render3D>();
        renderComp->setMesh(boxMesh)->setMaterial(concreteMaterial);
        auto shape = std::make_shared<OBB>();
        auto rigidBody = checkpoint2->addComponent<RigidBody>();
        rigidBody->setMass(0.0f)
            ->setDrag(1.0f)
            ->setShape(shape)
            ->setStatic(true)
            ->registerToPhysicsManager(PhysicsManager::Instance());
        rigidBody->initialize();
    }
}
