/*!****************************************************************************
 * \file   PauseMenu.h
 * \author Wang Daoming (KeitherJinn@outlook.com)
 * \par    **DigiPen Email**
 *    daoming.wang@digipen.edu
 * \par    **Course**
 *    CS529
 * \date   10-14-2024
 * \brief  File conatining the custom Vector3 implementation for use with
 * OpenGL functions and other game logic.
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#pragma once

#include "GameObject.h"
#include "FirstPersonControllerComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Input.h"
#include "stb_image.h"
#include <variant>

class PauseMenu {
public:
	static PauseMenu& Instance() {
		static PauseMenu instance;
		return instance;
	}
	PauseMenu(const PauseMenu
		&) = delete;
	PauseMenu& operator=(const PauseMenu&) = delete;

	void resetTimer();
	void setInputSystem(Input* ip);
	void setFramerateController(FFramerateController* _frc);
	void setPlayer(std::shared_ptr<GameObject> pl);
	void setState(bool state);
	bool gameIsPaused();
	bool isQuit();
	bool isStart();
	void setStart(bool val);
	void run();
	float getMusicVolume();
	float getSFXVolume();

	enum Action {
		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,
		Jump,
		Sprint,
		Slide,
		NUM_ACTIONS
	};

	static constexpr const char* ActionNames[NUM_ACTIONS] = {
		"Move Forward", "Move Backward", "Move Left", "Move Right", "Jump", "Sprint", "Slide"
	};
	Key actionKeys[NUM_ACTIONS] = {
		KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, KEY_LEFT_SHIFT, KEY_LEFT_CONTROL
	};

	const Key* getActionKeys() const { return actionKeys; }

	static std::string GetKeyName(Key key)
	{
		switch (key) {
		case KEY_W: return "W";
		case KEY_A: return "A";
		case KEY_S: return "S";
		case KEY_D: return "D";
		case KEY_LEFT_SHIFT: return "Shift";
		case KEY_LEFT_CONTROL: return "Ctrl";
		case KEY_SPACE: return "Space";
		default: return "Key " + std::to_string((int)key);
		}
	}
	void pauseCallback(GLFWwindow* pWindow, int width, int height);
	void registerPauseCallback(GameWindow* gameWIndow);

private:
	enum MenuType {
		MainPauseMenu,
		HowToPlay,
		Settings,
		Quit,
		Credits,
		Test
	};

	

	// For remapping state
	int remapActionIndex = -1; // -1:not remapping, [0,NUM_ACTIONS)
	bool waitingForKey = false;

	PauseMenu() = default;

	void mainPauseMenu();
	void howToPlay();
	void credits();
	void settings();
	void quitMenu();
	void testMenu();

	bool isPaused = false;
	bool quit = false;
	bool toStart = false;
	bool enteredState = false;

	float musicVolume = 0.15f;
	float SFXVolume = 1.0f;
	float mouseXSensivity = 0.1f;
	float mouseYSensivity = 0.1f;

	float gpLXSensitivity = 3.0f;
	float gpLYSensitivity = 3.0f;
	float gpRXSensitivity = 3.0f;
	float gpRYSensitivity = 3.0f;

	float time = 0.0f;
	Input* input;
	MenuType menuType = MainPauseMenu;
	FFramerateController* frc;
	std::shared_ptr<GameObject> player;

	
};

#endif
