/*!****************************************************************************
 * \file   LevelManager.cpp
 * \author Nicholas Shaw (nick.shaw@digipen.edu) (Base class and manager code)
 * \author Anish Murthy (anish.murthy@digipen.edu) (Graphics/Physics functions)
 * \author Daoming Wang (daoming.wang@digipen.edu) (Controller and Pause menu)
 * \author Li Kuang (li.kuang@digipen.edu) (Map Management/Raycasting)
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "LevelManager.h"

void LevelManager::SystemInitalization()
{
    mainWindow = new GameWindow;

    currentLevel = 7;

    isFullscreen = false;
    mainWindow->setTitle("Traversal")->setInitialFullscreen(isFullscreen);
    mainWindow->initialize();

    /* Renderer setup */
    mainRenderer = new Renderer;
    mainRenderer->setGameWindow(mainWindow);
    mainRenderer->initialize();
    mainRenderer->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //glfwSwapInterval(0);

    /* IMGUI Init */
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(mainWindow->getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init();

    int textureMode = 1;

    //auto skydomePass = std::make_shared<SkydomePass>("media/beach.jpg");
    //mainRenderer->getRenderGraph()->addPass<SkydomePass>(skydomePass);

    //mainWindow->setVsync(true);

    /* Input setup */
    mainInput = new Input;
    std::vector<Key> keysToMonitor = {
      KEY_W, KEY_A, KEY_S, KEY_D, KEY_R, KEY_F,
      KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT, KEY_ESCAPE
    };
    mainInput->setGameWindow(mainWindow);
    mainInput->initialize();

    /* XInput setup */
    gamepad = new GamePad;
    gamepad->initialize();
    gamepadState = gamepad->CheckConnection();

    /* Framerate controller setup */
    mainFramerateController =
        FFramerateController::getController();

    /*PauseMenu::Instance().setInputSystem(mainInput);
    PauseMenu::Instance().setFramerateController(mainFramerateController);
    PauseMenu::Instance().setGamePad(gamepad);
    PauseMenu::Instance().registerPauseCallback(mainWindow);*/

    /* Audio System Initalization */ 
    AudioManager::instance().initialize();
    //AudioManager::instance().loadSound("music", "media/audio/FG15-SpyVsSpy-Pfrommer.mp3", false, true);
    AudioManager::instance().loadSound("walk", "media/audio/walk.mp3", true);
    AudioManager::instance().loadSound("run", "media/audio/footstep.mp3", true);
    AudioManager::instance().loadSound("slide", "media/audio/slide.mp3", true);
    AudioManager::instance().loadSound("jump", "media/audio/jump.mp3", true);
    AudioManager::instance().loadSound("key", "media/audio/key.ogg", true);
    AudioManager::instance().loadSound("hurt", "media/audio/hurt.mp3", true);
    AudioManager::instance().loadSound("click", "media/audio/click.ogg", false);
    //AudioManager::instance().playSound("music", Vector3(0.0f, 0.0f, 0.0f), 0.15f);

    /* Scenegraph setup */
    //SceneGraph mainSceneGraph;

    /* Raycast Manager Setup */
    RaycastManager::Instance().setSceneGraph(&mainSceneGraph);

}

void LevelManager::SystemShutdown()
{
  ImGui::DestroyContext();

  mainInput->shutdown();
  mainRenderer->shutdown();
  mainWindow->shutdown();
  gamepad->shutdown();
  AudioManager::instance().shutdown();

  delete mainInput;
  delete mainRenderer;
  delete mainWindow;
  delete gamepad;
}

void LevelManager::MeshMatInitializations()
{

#pragma region Player

  boxMesh = Mesh::getShapeMesh(Mesh::Cube);
  sphereMesh = Mesh::getShapeMesh(Mesh::Sphere);

  shadowMaterial = Material::getMaterial<TextureMaterial>("shadow");
  shadowMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
  shadowMaterial->setProperty("shininess", 1.0f);
  shadowMaterial->addTexture("media/textures/shadow5.png", 1.0f, 1.0f);

    // Digi Material
    digiMaterial = Material::getMaterial<TextureMaterial>("digi");
    digiMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
    digiMaterial->setProperty("shininess", 10.0f);
    digiMaterial->addTexture("media/textures/DigiPenLogo.jpg", 1.0f, 1.0f);

    // FMOD Material
    fmodMaterial = Material::getMaterial<TextureMaterial>("fmod");
    fmodMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
    fmodMaterial->setProperty("shininess", 10.0f);
    fmodMaterial->addTexture("media/textures/FmodLogo.jpg", 1.0f, 1.0f);

    //doorMaterial = Material::getMaterial<MainTestMaterial>("door", mainRenderer->getRenderGraph());
    //doorMaterial->setProperty("diffuse", Vector3(87 / 255.0f, 51 / 255.0f, 35 / 255.0f));
    //doorMaterial->setProperty("specular", Vector3(0.009f, 0.009f, 0.009f));
    //doorMaterial->setProperty("shininess", 20.0f);
    //doorMaterial->addTexture("media/textures/door.png", 1.0f, 1.0f);

#pragma endregion

  /*Map Loader*/
  std::shared_ptr<RenderGraph> rg = mainRenderer->getRenderGraph();

  MapLoader::instance().initializeResources();

}

void LevelManager::DisplayLogos()
{
    SceneGraph sceneGraph;

    //Create Object
    auto Logo = std::make_shared<GameObject>("Logo");
    sceneGraph.addNode(Logo);
    Logo->setLocalPosition(Vector3(0.0f, 0.0f, -10.0f));
    auto renderComp = Logo->addComponent<Render2D>();
    renderComp->setMaterial(digiMaterial)->setProperty("useTexture", 1);
    
    const Renderer::Viewport& viewPort = mainRenderer->getCurrentState().viewport;
    Logo->setLocalScaling(Vector3((float) viewPort.width/2, (float) viewPort.width/2, 1.0f));

    //Create Camera
    auto cam = std::make_shared<FreeCamera>("uicam");
      cam->lookAt(Vector3(0.0f, 0.0f, -1.0f))->setOrthographicProjection(
        -(viewPort.width / 2), (viewPort.width / 2),
        -(viewPort.height / 2), (viewPort.height / 2),
        1.0f,100.0f
      );
    sceneGraph.addCamera(cam);

    mainRenderer->getRenderGraph()->addPass<UIPass>();
    mainRenderer->setClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    //Count the Logos
    float logoTimer = 0.0f;
    float logoDuration = 5.0f;
    int currentLogo = 0;
    int numLogos = 2;

    int expectedFrameRate = 60;
    static int windowedPosX, windowedPosY, windowedWidth, windowedHeight;

    mainFramerateController->setTargetFramerate(expectedFrameRate);
    //sceneGraph.printSceneTree();

    bool mouseClicked = false;
    bool prevMouseClicked = true;

    while (currentLogo < numLogos) {

        //Restart the Renderer
        mainRenderer->clear();
        mainFramerateController->startFrame();              // record the time from frame start

        //Update the Input Manager
        mainInput->update();

        //Update the GamePad
        gamepad->update();

        //Full Screen Toggle
        if (mainInput->isKeyPressed(KEY_F11)) {

          isFullscreen = !isFullscreen;
          mainWindow->setFullscreen(isFullscreen);

        }

        //Update Logo Timer
        float deltaTime = 1.0f / expectedFrameRate;
        logoTimer += deltaTime;

        //Ambient Light
        float ambientIntensity = 1.0f;
        float fadeInTime = 0.5f;
        float fadeOutTime = 1.0f;
        if (logoTimer < fadeInTime) {
            ambientIntensity = logoTimer / fadeInTime;
        }
        else if (logoTimer > logoDuration - fadeOutTime) {
            ambientIntensity = std::max(0.0f, (logoDuration - logoTimer) / fadeOutTime);
        }
        renderComp->setProperty("transparency", ambientIntensity);

        bool mouseClicked = mainInput->isMouseButtonDown(0) || mainInput->isMouseButtonDown(1);
        bool skipped = (!prevMouseClicked && mouseClicked) ||
                       (mainInput->isKeyPressed(KEY_SPACE))||
                       (mainInput->isKeyPressed(KEY_ENTER))||
                       (mainInput->isKeyPressed(KEY_ESCAPE));

        if ( skipped || logoTimer >= logoDuration) {
            logoTimer = 0.0f;
            currentLogo++;

            if (currentLogo < numLogos) {
                switch (currentLogo)
                {
                case 0:
                    renderComp->setMaterial(digiMaterial);
                    break;
                case 1:
                    renderComp->setMaterial(fmodMaterial);
                    break;
                default:
                    break;
                }
            }
        }

        prevMouseClicked = mouseClicked;

        //Draw the Scene
        mainRenderer->getRenderGraph()->draw(&sceneGraph);

        //Update Scenegraph
        mainFramerateController->endFrame();

        //Swap Buffers and Update Window
        mainRenderer->swapBuffers();
        mainWindow->update();

    }

    mainRenderer->getRenderGraph()->removePass<UIPass>();
    mainRenderer->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    mainRenderer->clear();

}

void LevelManager::RunLevels()
{

  levelSwapFlag = false;

  createPlayerObject();
  initalizePlayerInLevel();

    switch (currentLevel)
    {
    case -1:
        LoadLevelMenu();
        break;
    case 0:
        LoadLevel0();
        break;
    case 1:
        LoadLevel1();
        break;
    case 2:
        LoadLevel2();
        break;
    case 3:
        LoadLevel3();
        break;
    case 4:
        LoadLevel4();
        break;
    case 5:
        LoadLevel5();
        break;
    case 6:
        LoadLevel6();
        break;
    case 7:
        LoadLevel7();
        break;
    default:        
        break;
    }

  ExecuteMainLoop();

  ClearLevel();

}

void LevelManager::ExecuteMainLoop()
{

  /* Main Loop Variables */
  float angleX = 0.0f;
  float angleY = 0.0f;
  float angleZ = 0.0f;
  float speed = 10.0f;
  float deltaTime = 0.0f;
  int expectedFrameRate = 60; // 1000;
  static int windowedPosX, windowedPosY, windowedWidth, windowedHeight;

  mainFramerateController->setTargetFramerate(expectedFrameRate);
  mainSceneGraph.printSceneTree();
  
  mainRenderer->getRenderGraph()->lightsSet = false;

  auto autoBunnyParent = std::dynamic_pointer_cast<GameObject>(mainSceneGraph.findNode("autoBunnyParent"));

  while (!levelSwapFlag) {

    mainRenderer->clear();
    mainFramerateController->startFrame();              // record the time from frame start

    //Update the Input Manager
    mainInput->update();

    //Update the GamePad
    gamepad->update();

    ////If Escape is Pressed Exit Loop
    //if (PauseMenu::Instance().isQuit()) {
    //	currentLevel = numLevels + 1;
    //	break;
    //}

    if (mainInput->isKeyPressed(KEY_F11)) {
      isFullscreen = !isFullscreen;
      mainWindow->setFullscreen(isFullscreen);
    }
    
    if (mainInput->isKeyPressed(KEY_ESCAPE)) {
      currentLevel = numLevels + 1;
      break;
    }
    
    if (mainInput->isKeyPressed(KEY_B)) {
      if(autoBunnyParent->isEnabled())
        autoBunnyParent->disable();
      else
        autoBunnyParent->enable();
    }

    // Physics update loop fixedStepTime
    /*while (mainFramerateController->shouldUpdatePhysics()) {
      PhysicsManager::Instance().update(mainFramerateController->getPhysicsTimestep());
      mainFramerateController->consumePhysicsTime();
    }*/

    auto fpc = playerBox->findComponent<FirstPersonControllerComponent>();
    if (fpc && fpc->isCreativeMode()) {
      for (int level = 0; level < 10; ++level) {
        Key levelKey;
        switch (level) {
        case 0: levelKey = KEY_0; break;
        case 1: levelKey = KEY_1; break;
        case 2: levelKey = KEY_2; break;
        case 3: levelKey = KEY_3; break;
        case 4: levelKey = KEY_4; break;
        case 5: levelKey = KEY_5; break;
        case 6: levelKey = KEY_6; break;
        case 7: levelKey = KEY_7; break;
        case 8: levelKey = KEY_8; break;
        case 9: levelKey = KEY_9; break;
        default: continue;
        }
        if (mainInput->isKeyPressed(levelKey)) {
          if (level == 9) {
            resetToMenu();
          }
          if (level < numLevels) {
            currentLevel = level;
            levelSwapFlag = true;
          }
          else {
            std::cerr << "[MapLoader] Invalid level!\n";
          }
          break;
        }
      }
    }


  //      // GamePad Detection
  //      if (gamepad->CheckConnection() != gamepadState) {
  //          PauseMenu::Instance().setState(true);
  //          gamepadState = gamepad->CheckConnection();
  //      }

    //if (PauseMenu::Instance().gameIsPaused()) {
    //	mainRenderer->getRenderGraph()->draw(&mainSceneGraph);
    //	mainInput->controlMouse(false);
    //	PauseMenu::Instance().run();
    //}
    //else {
      while(mainFramerateController->shouldUpdatePhysics()) {
          PhysicsManager::Instance().update(mainFramerateController->getPhysicsTimestep());
          mainFramerateController->consumePhysicsTime();
      }
      //Audio Update
      AudioManager::instance().update();
      AudioManager::instance().setListenerPosition(playerBox.get()->getWorldTransform().getPosition());
      /*if (mainInput->isKeyPressed(KEY_V)) {
        AudioManager::instance().togglePlaybackSpeed(0.7f);
      }*/
      AudioManager::instance().setListenerPosition(playerBox->getWorldPosition());

      checkPlayerBoundaries();
    
      mainSceneGraph.update(mainFramerateController->getFrameTime());
  
      mainRenderer->getRenderGraph()->draw(&mainSceneGraph);
    //}

    mainFramerateController->endFrame();

    //
    //#pragma region IMGUI
    //        ImGui_ImplOpenGL3_NewFrame();
    //        ImGui_ImplGlfw_NewFrame();
    //        ImGui::NewFrame();
    //        //ImGui::SetNextWindowPos(ImVec2(10, 10)); // Position at top-left
    //        //ImGui::SetNextWindowBgAlpha(0.35f); // Make it semi-transparent
    //
    //        ImGui::Begin("Overlay", nullptr,
    //            ImGuiWindowFlags_NoTitleBar |
    //            ImGuiWindowFlags_NoResize |
    //            ImGuiWindowFlags_AlwaysAutoResize |
    //            ImGuiWindowFlags_NoMove |
    //            ImGuiWindowFlags_NoScrollbar |
    //            ImGuiWindowFlags_NoInputs);
    //
    //        ImGui::Text("FPS: %.1f", mainFramerateController->getFPS());
    //        ImGui::Text("Frametime: %f", mainFramerateController->getFrameTime());
    //        ImGui::Text("RenderTime: %f", mainFramerateController->getRenderTime());
    //
    //        Vector3 playerPos = playerBox->getWorldTransform().getPosition();
    //        ImGui::Text("x: %.2f  y: %.2f  z: %.2f", playerPos.x, playerPos.y, playerPos.z);
    //        ImGui::Text("Timer: %.2f seconds", mainFramerateController->getTime());
    //        Vector3 listenerPos = AudioManager::instance().getListenerPosition();
    //        ImGui::Text("HP: %d", fpc->getHP());
    //        if (auto fpc = playerBox->findComponent<FirstPersonControllerComponent>()) {
    //            std::string diffStr;
    //            switch (fpc->getDifficulty()) {
    //            case FirstPersonControllerComponent::EASY:
    //                diffStr = "EASY";
    //                break;
    //            case FirstPersonControllerComponent::NORMAL:
    //                diffStr = "NORMAL";
    //                break;
    //            case FirstPersonControllerComponent::HARD:
    //                diffStr = "HARD";
    //                break;
    //            case FirstPersonControllerComponent::CHEATING:
    //                diffStr = "CHEATING";
    //                break;
    //            default:
    //                diffStr = "UNKNOWN";
    //                break;
    //            }
    //            ImGui::Text("Difficulty: %s", diffStr.c_str());
    //        }
    //
    //        ImGui::End();
    //
    //        ImGui::Render();
    //        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //#pragma endregion


    mainRenderer->swapBuffers();
    mainWindow->update();

    //glfwSwapBuffers(window);

  }
}

void LevelManager::checkPlayerBoundaries() {
    Vector3 playerPos = playerBox->getWorldTransform().getPosition();

    float maxX, minX, maxY, minY, minZ, maxZ;
    switch (currentLevel) {
    case 0:
        maxX = 10.0f;   minX = -400.0f;
        maxY = 100.0f;  minY = -45.0f;
        maxZ = 11.0f;   minZ = -11.0f;
        break;
    case 1:
        maxX = 150.0f;  minX = -400.0f;
        maxY = 150.0f;  minY = -45.0f;
        maxZ = 11.0f;   minZ = -11.0f;
        break;
    case 2:
        maxX = 150.0f;  minX = -400.0f;
        maxY = 150.0f;  minY = -45.0f;
        maxZ = 11.0f;   minZ = -11.0f;
        break;
    case 5:
        maxX = 1000.0f; minX = -1000.0f;
        maxY = 1000.0f; minY = -5.0f;
        maxZ = 1000.0f; minZ = -1000.0f;
        break;
    default:
        maxX = 1000.0f; minX = -1000.0f;
        maxY = 1000.0f; minY = -20.0f;
        maxZ = 1000.0f; minZ = -1000.0f;
        break;
    }

    if (playerDifficulty == FirstPersonControllerComponent::HARD) {
        minY = -100.0f;
    }

    if (playerPos.x > maxX || playerPos.x < minX ||
        playerPos.y > maxY || playerPos.y < minY ||
        playerPos.z > maxZ || playerPos.z < minZ)
    {
        auto fpc = playerBox->findComponent<FirstPersonControllerComponent>();
        if (fpc && !fpc->isCreativeMode()) {
            fpc->respawnPlayer(true);
        }
    }
}

void LevelManager::NextLevel()
{
    if (currentLevel == 6) {
        currentLevel = -1;
    }
    else {
        currentLevel++;
    }
    levelSwapFlag = true;
}

void LevelManager::ClearLevel()
{

  //Clear Physics and Scenegraph
  PhysicsManager::Instance().clearPhysicsManager();
  mainSceneGraph.clear();

}

void LevelManager::ShutdownLevels()
{
  // Empty for now, but we can add stuff later.
}

void LevelManager::initialize()
{
  SystemInitalization();
  MeshMatInitializations();
}

void LevelManager::shutdown() {
  SystemShutdown();
  ShutdownLevels();
}

#pragma region LevelLoaders

void LevelManager::LoadLevelMenu()
{
  MapLoader::instance().loadMap(-1, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel0()
{
  MapLoader::instance().loadMap(0, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel1()
{
  MapLoader::instance().loadMap(1, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel2()
{
  MapLoader::instance().loadMap(2, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel3()
{
  MapLoader::instance().loadMap(3, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel4()
{
  MapLoader::instance().loadMap(4, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel5()
{
  MapLoader::instance().loadMap(5, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel6()
{
    MapLoader::instance().loadMap(6, 0, 0, 0, mainSceneGraph);
}

void LevelManager::LoadLevel7()
{
  MapLoader::instance().loadMap(7, 0, 0, 0, mainSceneGraph);
}


#pragma endregion

bool LevelManager::GameComplete()
{
  return currentLevel >= numLevels;
}

void LevelManager::createPlayerObject()
{
#pragma region Initalizations

  playerBox = std::make_shared<GameObject>("PlayerBox", GameObject::Tag::PLAYER);
  mainSceneGraph.addNode(playerBox);

  cameraGameObject = std::make_shared<GameObject>("mainCamera");
  playerBox->addChild(cameraGameObject);

  camera = std::make_shared<AttachedCamera>("mainCamera");
  camera->attachToNode(cameraGameObject);
  mainSceneGraph.addCamera(camera);

#pragma endregion

#pragma region Camera

  //Perspective
  camera->setPerspectiveProjection(
    45.0f * 3.14159f / 180.0f,
    mainWindow->getAspectRatio(),
    0.1f,
    5000.0f);

  //Transform
  cameraGameObject->setLocalPosition(Vector3(0.0f, 34.0f, 0.0f));

  //auto cameraShape = std::make_shared<OBB>(
  //    Vector3(0.0f, 0.0f, 0.0f),  // half width/height of 50 for 100x100 box
  //    Vector3(0.5f, 0.5f, 0.5f));
  //camera->addComponent<PhysicsBody>()
  //    ->setMass(10.0f)->setDrag(1.0f)->setAngularDrag(1.0f)
  //    ->setShape(cameraShape)
  //    //->setDebug(true)
  //    ->registerToPhysicsManager(PhysicsManager::Instance());

#pragma endregion

  Vector3 LightDirection = Vector3(0.75f, -1.0f, 0.75f).normalized();

  mainSceneGraph.addAmbientLight(
    AmbientLight(Vector3(1, 1, 1), 0.05f));
  mainSceneGraph.addDirectionalLight(
    DirectionalLight(LightDirection, 0.0f, Vector3(1.0f, 1.0f, 1.0f)));

#pragma region PlayerBox

//Transform Values
    playerBox->setLocalPosition(startingPos1)
        ->setLocalScaling(Vector3(1.0f, 0.1f, 1.0f));

    playerBox->addComponent<KeyList>();
    //Render Component
    auto box1RenderComponent = playerBox->addComponent<Render3D>();
    box1RenderComponent
        ->setMesh(sphereMesh)
        ->setMaterial(shadowMaterial);

    //Create Shape
    auto shape1 = std::make_shared<OBB>(
    Vector3(0.0f, 0.0f, 0.0f),  // half width/height of 50 for 100x100 box
    Vector3(0.5f, 5.0f, 0.5f));

    // Create instances of bodies for boxes
    auto playerBoxPB = playerBox->addComponent<RigidBody>()
        ->usingGravity(true)
        ->setMass(10.0f)
        ->setDrag(1.5f)
        ->setShape(shape1)
        ->registerToPhysicsManager(PhysicsManager::Instance());
    playerBoxPB->initialize();

    auto playerBoxInputComponent = playerBox->addComponent<FirstPersonControllerComponent>()
        ->setInputSystem(mainInput)
        ->setGamePad(gamepad)
        ->setPhysicsBody(playerBoxPB.get())
        ->setBody(playerBox.get())
        ->setSceneRoot(mainSceneGraph.getRootNode())
        ->setCamera(cameraGameObject.get())
        //->setActionKey(FirstPersonControllerComponent::MoveForward, KEY_W)
        //->setActionKey(FirstPersonControllerComponent::MoveBackward, KEY_S)
        //->setActionKey(FirstPersonControllerComponent::MoveLeft, KEY_A)
        //->setActionKey(FirstPersonControllerComponent::MoveRight, KEY_D)
        //->setActionKey(FirstPersonControllerComponent::Jump, KEY_SPACE)
        //->setActionKey(FirstPersonControllerComponent::Sprint, KEY_LEFT_SHIFT)
        //->setActionKey(FirstPersonControllerComponent::Slide, KEY_LEFT_CONTROL)
        ->setActionKey(FirstPersonControllerComponent::Respawn, KEY_R)
        ->setActionKey(FirstPersonControllerComponent::Debug, KEY_9)
        ->setActionKey(FirstPersonControllerComponent::Creative, KEY_C)
        //->setActionKey(FirstPersonControllerComponent::Music, KEY_M)
        //->setActionKey(FirstPersonControllerComponent::Freeze, KEY_F)
        ->setActionKey(FirstPersonControllerComponent::Pause, KEY_ESCAPE)
        ->setGPActionKey(FirstPersonControllerComponent::Jump, XINPUT_GAMEPAD_A)
        //->setGPActionKey(FirstPersonControllerComponent::Sprint, XINPUT_GAMEPAD_LEFT_THUMB)
        ->setGPActionKey(FirstPersonControllerComponent::Sprint, XINPUT_GAMEPAD_LEFT_SHOULDER)
        ->setGPActionKey(FirstPersonControllerComponent::Slide, XINPUT_GAMEPAD_B)
        ->setGPActionKey(FirstPersonControllerComponent::Respawn, XINPUT_GAMEPAD_X)
        ->setGPActionKey(FirstPersonControllerComponent::Music, XINPUT_GAMEPAD_Y)
        ->setGPActionKey(FirstPersonControllerComponent::Pause, XINPUT_GAMEPAD_START);

    const Key* keys = PauseMenu::Instance().getActionKeys();
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::MoveForward, keys[PauseMenu::MoveForward]);
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::MoveBackward, keys[PauseMenu::MoveBackward]);
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::MoveLeft, keys[PauseMenu::MoveLeft]);
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::MoveRight, keys[PauseMenu::MoveRight]);
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::Jump, keys[PauseMenu::Jump]);
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::Sprint, keys[PauseMenu::Sprint]);
    playerBoxInputComponent->setActionKey(FirstPersonControllerComponent::Slide, keys[PauseMenu::Slide]);

    playerBoxInputComponent->setDifficulty(playerDifficulty);


#pragma endregion

  PauseMenu::Instance().setPlayer(playerBox);
}

void LevelManager::initalizePlayerInLevel()
{
    Vector3 activeSpawnPoint;
    Quaternion activeSpawnRotation;

    switch (currentLevel)
    {
    case -1:
        activeSpawnPoint = startingPos0;
        activeSpawnRotation = startingRot0;
        break;
    case 0:
        activeSpawnPoint = startingPos0;
        activeSpawnRotation = startingRot0;
        break;
    case 1:
        activeSpawnPoint = startingPos1;
        activeSpawnRotation = startingRot1;
        break;
    case 2:
        activeSpawnPoint = startingPos2;
        activeSpawnRotation = startingRot2;
        break;
    case 3:
        activeSpawnPoint = startingPos3;
        activeSpawnRotation = startingRot3;
        break;
    case 4:
        activeSpawnPoint = startingPos4;
        activeSpawnRotation = startingRot4;
        break;
    case 5:
        activeSpawnPoint = startingPos5;
        activeSpawnRotation = startingRot5;
        break;
    case 6:
        activeSpawnPoint = startingPos6;
        activeSpawnRotation = startingRot6;
        break;
    case 7:
      activeSpawnPoint = startingPos6;
      activeSpawnRotation = startingRot6;
      break;
    default:
        activeSpawnPoint = Vector3();
        activeSpawnRotation = Quaternion();
        break;
    }

    auto pbFPCController = playerBox->findComponent<FirstPersonControllerComponent>();
    pbFPCController->setRespawnCheckpoint(activeSpawnPoint, activeSpawnRotation);
    pbFPCController->respawnPlayer(true, true);

    

    //auto skydomePass = mainRenderer->getRenderGraph()->addPass<SkydomePass>();
    //skydomePass->addTexture(std::string("media/textures/skys/Newport_Loft_Ref.hdr"));

    mainRenderer->getRenderGraph()->addPass<GBufferPrepass>();

    auto lightingPass = mainRenderer->getRenderGraph()->addPass<LightingPass>();
    lightingPass->addIrradianceMap("media/textures/skys/Newport_Loft_Ref.irr.hdr");
    lightingPass->addSkydome("media/textures/skys/Newport_Loft_Ref.hdr");

    mainRenderer->getRenderGraph()->addPass<LocalLightsPass>();

}

void LevelManager::SetPlayerDifficulty(FirstPersonControllerComponent::Difficulty diff) {
  playerDifficulty = diff;
}

FirstPersonControllerComponent::Difficulty LevelManager::getDifficulty() const {
  return playerDifficulty;
}

void LevelManager::resetToMenu()
{
  currentLevel = -1;
  levelSwapFlag = true;

}
