/*!****************************************************************************
 * \file   LevelManager.h
 * \author Nicholas Shaw (nick.shaw@digipen.edu) (Base class and manager code)
 * \author Anish Murthy (anish.murthy@digipen.edu) (Graphics/Physics functions)
 * \author Daoming Wang (daoming.wang@digipen.edu) (Controller and Pause menu)
 * \author Li Kuang (li.kuang@digipen.edu) (Map Management/Raycasting)
 * \brief  Level Manager that handles main game logic
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#pragma once

#include "AttachedCamera.h"
#include "Audio.h"
#include "Camera.h"
#include "CollisionListener.h"
#include "EventManager.h"
#include "FirstPersonControllerComponent.h"
#include "FramerateController.h"
#include "FreeCamera.h"
#include "GameObject.h"
#include "GameWindow.h"
#include "GBufferPrepass.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Input.h"
#include "KeyList.h"
#include "LightingPass.h"
#include "LocalLightsPass.h"
#include "MapLoader.h"
#include "OBB.h"
#include "PauseMenu.h"
#include "PhysicsBody.h"
#include "PhysicsManager.h"
#include "Ray.h"
#include "RaycastHit.h"
#include "RaycastManager.h"
#include "Render2D.h"
#include "Render3D.h"
#include "Renderer.h"
#include "SceneGraph.h"
#include "SkydomePass.h"
#include "TextureMaterial.h"
#include "TrianglePrimitive.h"
#include "UIPass.h"


class LevelManager {

public:
	static LevelManager& Instance() {
		static LevelManager instance;
		return instance;
	}
	LevelManager(const LevelManager&) = delete;
	LevelManager& operator=(const LevelManager&) = delete;

	void initialize();
	void shutdown();

	void SystemInitalization();
	void SystemShutdown();

	void MeshMatInitializations();

	void DisplayLogos();

	void RunLevels();
	void ExecuteMainLoop();
	void checkPlayerBoundaries();
	void NextLevel();
	void ClearLevel();
	void ShutdownLevels();

	void LoadLevelMenu();
	void LoadLevel0();
	void LoadLevel1();
	void LoadLevel2();
	void LoadLevel3();
	void LoadLevel4();
	void LoadLevel5();
	void LoadLevel6();
	void LoadLevel7();

	bool GameComplete();

	void createPlayerObject();
	void initalizePlayerInLevel();
	void SetPlayerDifficulty(FirstPersonControllerComponent::Difficulty diff);
	FirstPersonControllerComponent::Difficulty getDifficulty() const;

	void resetToMenu();

private:
	LevelManager() = default;

	//Level Counter
	int currentLevel = -1;
	int numLevels = 8;
	bool levelSwapFlag = false;
	bool isFullscreen = true;
	bool gamepadState= false;
	FirstPersonControllerComponent::Difficulty playerDifficulty = FirstPersonControllerComponent::NORMAL;

	//Systems
	GameWindow* mainWindow;
	Renderer* mainRenderer;
	Input* mainInput;
	FFramerateController* mainFramerateController;
	SceneGraph mainSceneGraph;
	std::shared_ptr<GameObject> cameraGameObject;
	std::shared_ptr<AttachedCamera> camera;

	//Materials and Meshes
	std::shared_ptr<Mesh> boxMesh;
	std::shared_ptr<Mesh> sphereMesh;
	std::shared_ptr<TextureMaterial> shadowMaterial;
	std::shared_ptr<TextureMaterial> digiMaterial;
	std::shared_ptr<TextureMaterial> fmodMaterial;

	//Player References
	std::shared_ptr<GameObject> playerBox;
	Vector3 startingPos0 = Vector3(0.0f, 7.0f, 0.0f);
	Quaternion startingRot0 = Quaternion::fromEuler(Vector3(0.0f,0.5f*3.14159f,0.0f));
	Vector3 startingPos1 = Vector3(0.0f, 7.0f, 0.0f);
	Quaternion startingRot1 = Quaternion::fromEuler(Vector3(0.0f, 0.5f * 3.14159f, 0.0f));
	Vector3 startingPos2 = Vector3(0.0f, 7.0f, 0.0f);
	Quaternion startingRot2 = Quaternion::fromEuler(Vector3(0.0f, 0.5f * 3.14159f, 0.0f));
	Vector3 startingPos3 = Vector3(27.0f, 21.0f, 0.0f);
	Quaternion startingRot3 = Quaternion::fromEuler(Vector3(0.0f, 1.0f * 3.14159f, 0.0f));
	Vector3 startingPos4 = Vector3(0.0f, 0.0f, 0.0f);
	Quaternion startingRot4 = Quaternion::fromEuler(Vector3(0.0f, -0.5f * 3.14159f, 0.0f));
	Vector3 startingPos5 = Vector3(-131.0f, 7.0f, -130.0f);
	Quaternion startingRot5 = Quaternion::fromEuler(Vector3(0.0f, 1.0f * 3.14159f, 0.0f));
	Vector3 startingPos6 = Vector3(0.0f, 2.0f, -5.0f);
	Quaternion startingRot6 = Quaternion::fromEuler(Vector3(0.0f, 1.0f * 3.14159f, 0.0f));

	const float rad = PI / 180.0f;

};

#endif
