/*!****************************************************************************
 * \file   PauseMenu.cpp
 * \author Daoming Wang (daoming.wang@digipen.edu)
 * 
 * Copyright � 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"
#include "PauseMenu.h"

void PauseMenu::setInputSystem(Input* ip) {
	input = ip;
	return;
}

void PauseMenu::setFramerateController(FFramerateController* _frc) {
	frc = _frc;
	return;
}

void PauseMenu::setPlayer(std::shared_ptr<GameObject> pl) {
	player = pl;
	return;
}

void PauseMenu::setState(bool state) {
	if (!isPaused && state)
		enteredState = true;
	isPaused = state;
	return;
}

bool PauseMenu::gameIsPaused() {
	return isPaused;
}

bool PauseMenu::isQuit() {
	return quit;
}

bool PauseMenu::isStart() {
	return toStart;
}

void PauseMenu::setStart(bool val) {
	toStart = val;
	return;
}

float PauseMenu::getMusicVolume() {
	return musicVolume;
}

float PauseMenu::getSFXVolume() {
	return SFXVolume;
}

void PauseMenu::pauseCallback(GLFWwindow * pWindow, int width, int height) {
	isPaused = true;

	if ( width == 0 || height == 0 ) {
		AudioManager::instance().pauseSound();
	}
	else {
		AudioManager::instance().resumeSound();
	}
}

void PauseMenu::registerPauseCallback(GameWindow * gameWindow) {
	gameWindow->setResizeCallback2([this](GLFWwindow * pWindow, int width, int height) {
		this->pauseCallback(pWindow, width, height);
		});
}

void PauseMenu::testMenu() {
	//test stuffs
}

/*!****************************************************************************
 * \brief Main Pause menu function, shows the main pause screen
 * 
 *****************************************************************************/
void PauseMenu::mainPauseMenu() {
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
	io.FontGlobalScale = 2.0f;

	if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
		io.KeyMap[ImGuiKey_Space] = KEY_ENTER;
	}

	if (ImGui::IsKeyDown(ImGuiKey_Space)) {
		io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get the display size
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	// Set next window to cover the entire screen
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(displaySize);

	ImGui::Begin("Pause Menu", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	drawList->AddRectFilled(ImVec2(0, 0), displaySize, IM_COL32(0, 0, 0, 96));

	// Center the menu
	ImVec2 center = ImVec2(displaySize.x / 2, displaySize.y / 2);
	ImGui::SetCursorPos(ImVec2(center.x - 100, center.y - 200));

	float windowWidth = ImGui::GetWindowSize().x;
	float textWidth = ImGui::CalcTextSize("Game Paused").x;
	ImGui::Text("Game Paused");

	float buttonWidth = 200.0f;
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

	if (
		ImGui::Button("Resume", ImVec2(buttonWidth, 40)) ||
		(!enteredState && (
			ImGui::IsKeyPressed(ImGuiKey_Escape)
			))
	){
		frc->clearPhysicsTime();
		isPaused = false;
		enteredState = true;
		input->controlMouse(true);
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Back to Start", ImVec2(buttonWidth, 40))) {
		frc->clearPhysicsTime();
		isPaused = false;
		toStart = true;
		input->controlMouse(true);
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("How To Play", ImVec2(buttonWidth, 40))) {
		menuType = HowToPlay;
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Settings", ImVec2(buttonWidth, 40))) {
		menuType = Settings;
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Credits", ImVec2(buttonWidth, 40))) {
		menuType = Credits;
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Quit", ImVec2(buttonWidth, 40))) {
		menuType = Quit;
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return;
}

/*!****************************************************************************
 * \brief Credits screen
 *
 *****************************************************************************/
void PauseMenu::credits() {
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
	io.FontGlobalScale = 2.0f;

	if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
		io.KeyMap[ImGuiKey_Space] = KEY_ENTER;
	}

	if (ImGui::IsKeyDown(ImGuiKey_Space)) {
		io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get the display size
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	// Set next window to cover the entire screen
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(displaySize);

	ImGui::Begin("How To Play", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	drawList->AddRectFilled(ImVec2(0, 0), displaySize, IM_COL32(0, 0, 0, 96));

	// Center the menu
	ImVec2 center = ImVec2(displaySize.x / 2, displaySize.y / 2);
	ImGui::SetCursorPos(ImVec2(center.x - 350, center.y - 400));

	float windowWidth = ImGui::GetWindowSize().x;

	ImGui::Text("Credits");
	ImGui::Text(" ");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Team Hawthorn");
	ImGui::Text(" ");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Producer               Nicolas Shaw");
	ImGui::Text(" ");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Gameplay Programmer    Nicolas Shaw, Daoming Wang");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Level Designer         Lixiao Kuang");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Audio Programmer       Lixiao Kuang");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Engine Programmer      Arnish Murthy");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Graphics Programmer    Arnish Murthy");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Physics Programmer     Arnish Murthy, Daoming Wang");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("UI Programmer          Daoming Wang");
	ImGui::Text(" ");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Made using FMOD Studio by Firelight Technologies Pty Ltd.");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Made using GLFW3 by GLFW community");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Made using XInput by Microsoft");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Made using Dear ImGui by Github community");
	ImGui::Text(" ");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("Created at Digipen");
	ImGui::SetCursorPosX(center.x - 350);
	ImGui::Text("President              Claude Comair");

	float buttonWidth = 200.0f;
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Go Back", ImVec2(buttonWidth, 40)) ||
		(!enteredState && (
			ImGui::IsKeyPressed(ImGuiKey_Escape)
			))
		) {
		menuType = MainPauseMenu;
		enteredState = true;
	}

	ImGui::SetCursorPosX(center.x - 350);
	ImGui::SetCursorPosY(displaySize.y - 40);
	ImGui::Text("Copyright 2025 DigiPen, All Rights Reserved.");

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return;
}

/*!****************************************************************************
 * \brief How To Play function, shows how to play screen
 * 
 *****************************************************************************/
void PauseMenu::howToPlay() {
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
	io.FontGlobalScale = 2.0f;

	if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
		io.KeyMap[ImGuiKey_Space] = KEY_ENTER;
	}

	if (ImGui::IsKeyDown(ImGuiKey_Space)) {
		io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get the display size
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	// Set next window to cover the entire screen
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(displaySize);

	ImGui::Begin("How To Play", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	drawList->AddRectFilled(ImVec2(0, 0), displaySize, IM_COL32(0, 0, 0, 96));

	// Center the menu
	ImVec2 center = ImVec2(displaySize.x / 2, displaySize.y / 2);
	ImGui::SetCursorPos(ImVec2(center.x - 250, center.y - 200));

	float windowWidth = ImGui::GetWindowSize().x;

	ImGui::Text("How To Play:");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Move: W-A-S-D/Controller Left Joystick");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Look around: Mouse/Controller Right joystick");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Jump: Space/Controller A");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Sprint: LShift/Controller Left shoulder");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Slide: LCtrl/Controller B");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Respawn: R/Controller X");
	ImGui::SetCursorPosX(center.x - 250);
	ImGui::Text("Music on/off: M/Controller Right shoulder");

	float buttonWidth = 200.0f;
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Go Back", ImVec2(buttonWidth, 40)) ||
		(!enteredState && (
			ImGui::IsKeyPressed(ImGuiKey_Escape)
			))
		) {
		menuType = MainPauseMenu;
		enteredState = true;
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return;
}

/*!****************************************************************************
 * \brief settings function, player can set their preferences
 * 
 *****************************************************************************/
void PauseMenu::settings() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.FontGlobalScale = 2.0f;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(displaySize);

    ImGui::Begin("Settings", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(ImVec2(0, 0), displaySize, IM_COL32(0, 0, 0, 96));

    ImVec2 center = ImVec2(displaySize.x / 2, displaySize.y / 2);
    ImGui::SetCursorPos(ImVec2(center.x - 60, center.y - 350));

    float windowWidth = ImGui::GetWindowSize().x;
    ImGui::Text("Settings");
    ImGui::Text(" ");

    float buttonWidth = 130.0f;
    float itemSpacing = ImGui::GetStyle().ItemSpacing.x;

    ImGui::PushItemWidth(400.0f);

#pragma region Volume

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 1.0f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (musicVolume > 0.01f) {
			musicVolume -= 0.05f;
		}
		else {
			musicVolume = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (musicVolume < 0.99f) {
			musicVolume += 0.05f;
		}
		else {
			musicVolume = 1.0f;
		}
	}

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("SFX Volume", &SFXVolume, 0.0f, 1.0f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (SFXVolume > 0.01f) {
			SFXVolume -= 0.05f;
		}
		else {
			SFXVolume = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (SFXVolume < 0.99f) {
			SFXVolume += 0.05f;
		}
		else {
			SFXVolume = 1.0f;
		}
	}

#pragma endregion

	ImGui::Text(" ");

#pragma region Mouse

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("MouseX Sensivity", &mouseXSensivity, 0.0f, 0.2f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (mouseXSensivity > 0.005f) {
			mouseXSensivity -= 0.01f;
		}
		else {
			mouseXSensivity = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (mouseXSensivity < 0.195f) {
			mouseXSensivity += 0.01f;
		}
		else {
			mouseXSensivity = 0.2f;
		}
	}

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("MouseY Sensivity", &mouseYSensivity, 0.0f, 0.2f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (mouseYSensivity > 0.005f) {
			mouseYSensivity -= 0.01f;
		}
		else {
			mouseYSensivity = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (mouseYSensivity < 0.195f) {
			mouseYSensivity += 0.01f;
		}
		else {
			mouseYSensivity = 0.2f;
		}
	}

#pragma endregion

	ImGui::Text(" ");

#pragma region GamePad

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("Left Joystick X Sensivity", &gpLXSensitivity, 0.0f, 6.0f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (gpLXSensitivity > 0.05f) {
			gpLXSensitivity -= 0.25f;
		}
		else {
			gpLXSensitivity = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (gpLXSensitivity < 5.95f) {
			gpLXSensitivity += 0.25f;
		}
		else {
			gpLXSensitivity = 6.0f;
		}
	}

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("Left Joystick Y Sensivity", &gpLYSensitivity, 0.0f, 6.0f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (gpLYSensitivity > 0.05f) {
			gpLYSensitivity -= 0.25f;
		}
		else {
			gpLYSensitivity = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (gpLYSensitivity < 5.95f) {
			gpLYSensitivity += 0.25f;
		}
		else {
			gpLYSensitivity = 6.0f;
		}
	}

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("Right Joystick X Sensivity", &gpRXSensitivity, 0.0f, 6.0f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (gpRXSensitivity > 0.05f) {
			gpRXSensitivity -= 0.25f;
		}
		else {
			gpRXSensitivity = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (gpRXSensitivity < 5.95f) {
			gpRXSensitivity += 0.25f;
		}
		else {
			gpRXSensitivity = 6.0f;
		}
	}

	ImGui::SetCursorPosX((windowWidth - 400.f) * 0.5f);
	ImGui::SliderFloat("Right Joystick Y Sensivity", &gpRYSensitivity, 0.0f, 6.0f);

	if ((ImGui::IsKeyPressed(ImGuiKey_LeftArrow) && ImGui::IsItemFocused())) {
		if (gpRYSensitivity > 0.05f) {
			gpRYSensitivity -= 0.25f;
		}
		else {
			gpRYSensitivity = 0.0f;
		}
	}
	if ((ImGui::IsKeyPressed(ImGuiKey_RightArrow) && ImGui::IsItemFocused())) {
		if (gpRYSensitivity < 5.95f) {
			gpRYSensitivity += 0.25f;
		}
		else {
			gpRYSensitivity = 6.0f;
		}
	}

#pragma endregion

    Key* actionKeys = this->actionKeys;


    // Key assignment
    if (waitingForKey && remapActionIndex >= 0 && remapActionIndex < NUM_ACTIONS) {
        for (int k = 32; k <= 348; ++k) // GLFW keys 32-348
        {
            if (input->isKeyPressed(static_cast<Key>(k))) {
                if (k == GLFW_KEY_ESCAPE) {
                    // Cancel assign
                    remapActionIndex = -1;
                    waitingForKey = false;
                }
                else {
                    actionKeys[remapActionIndex] = static_cast<Key>(k);
                    remapActionIndex = -1;
                    waitingForKey = false;
                }
                break;
            }
        }
    }

    // Key Label Mapping
    auto GetKeyLabel = [](Key k)->std::string {
        switch (k) {
        case KEY_SPACE: return "Space";
        case GLFW_KEY_APOSTROPHE: return "'";
        case GLFW_KEY_COMMA: return ",";
        case GLFW_KEY_MINUS: return "-";
        case GLFW_KEY_PERIOD: return ".";
        case GLFW_KEY_SLASH: return "/";
        case KEY_0: return "0";
        case KEY_1: return "1";
        case KEY_2: return "2";
        case KEY_3: return "3";
        case KEY_4: return "4";
        case KEY_5: return "5";
        case KEY_6: return "6";
        case KEY_7: return "7";
        case KEY_8: return "8";
        case KEY_9: return "9";
        case GLFW_KEY_SEMICOLON: return ";";
        case GLFW_KEY_EQUAL: return "=";
        case KEY_A: return "A";
        case KEY_B: return "B";
        case KEY_C: return "C";
        case KEY_D: return "D";
        case KEY_E: return "E";
        case KEY_F: return "F";
        case KEY_G: return "G";
        case KEY_H: return "H";
        case KEY_I: return "I";
        case KEY_J: return "J";
        case KEY_K: return "K";
        case KEY_L: return "L";
        case KEY_M: return "M";
        case KEY_N: return "N";
        case KEY_O: return "O";
        case KEY_P: return "P";
        case KEY_Q: return "Q";
        case KEY_R: return "R";
        case KEY_S: return "S";
        case KEY_T: return "T";
        case KEY_U: return "U";
        case KEY_V: return "V";
        case KEY_W: return "W";
        case KEY_X: return "X";
        case KEY_Y: return "Y";
        case KEY_Z: return "Z";
        case GLFW_KEY_LEFT_BRACKET: return "[";
        case GLFW_KEY_BACKSLASH: return "\\";
        case GLFW_KEY_RIGHT_BRACKET: return "]";
        case GLFW_KEY_GRAVE_ACCENT: return "`";
        case GLFW_KEY_WORLD_1:      return "WLD1";
        case GLFW_KEY_WORLD_2:      return "WLD2";
        // Control/Function
        case GLFW_KEY_ESCAPE:       return "Esc";
        case GLFW_KEY_ENTER:        return "Enter";
        case GLFW_KEY_TAB:          return "Tab";
        case GLFW_KEY_BACKSPACE:    return "Backspace";
        case GLFW_KEY_INSERT:       return "Insert";
        case GLFW_KEY_DELETE:       return "Delete";
        case GLFW_KEY_RIGHT:        return "Right";
        case GLFW_KEY_LEFT:         return "Left";
        case GLFW_KEY_DOWN:         return "Down";
        case GLFW_KEY_UP:           return "Up";
        case GLFW_KEY_PAGE_UP:      return "PageUp";
        case GLFW_KEY_PAGE_DOWN:    return "PageDown";
        case GLFW_KEY_HOME:         return "Home";
        case GLFW_KEY_END:          return "End";
        case GLFW_KEY_CAPS_LOCK:    return "Caps";
        case GLFW_KEY_SCROLL_LOCK:  return "Scroll";
        case GLFW_KEY_NUM_LOCK:     return "NumLock";
        case GLFW_KEY_PRINT_SCREEN: return "PrtScrn";
        case GLFW_KEY_PAUSE:        return "Pause";
        // Function keys
        case GLFW_KEY_F1:  return "F1";
        case GLFW_KEY_F2:  return "F2";
        case GLFW_KEY_F3:  return "F3";
        case GLFW_KEY_F4:  return "F4";
        case GLFW_KEY_F5:  return "F5";
        case GLFW_KEY_F6:  return "F6";
        case GLFW_KEY_F7:  return "F7";
        case GLFW_KEY_F8:  return "F8";
        case GLFW_KEY_F9:  return "F9";
        case GLFW_KEY_F10: return "F10";
        case GLFW_KEY_F11: return "F11";
        case GLFW_KEY_F12: return "F12";
        case GLFW_KEY_F13: return "F13";
        case GLFW_KEY_F14: return "F14";
        case GLFW_KEY_F15: return "F15";
        case GLFW_KEY_F16: return "F16";
        case GLFW_KEY_F17: return "F17";
        case GLFW_KEY_F18: return "F18";
        case GLFW_KEY_F19: return "F19";
        case GLFW_KEY_F20: return "F20";
        case GLFW_KEY_F21: return "F21";
        case GLFW_KEY_F22: return "F22";
        case GLFW_KEY_F23: return "F23";
        case GLFW_KEY_F24: return "F24";
        case GLFW_KEY_F25: return "F25";
        // Numpad
        case GLFW_KEY_KP_0: return "Num 0";
        case GLFW_KEY_KP_1: return "Num 1";
        case GLFW_KEY_KP_2: return "Num 2";
        case GLFW_KEY_KP_3: return "Num 3";
        case GLFW_KEY_KP_4: return "Num 4";
        case GLFW_KEY_KP_5: return "Num 5";
        case GLFW_KEY_KP_6: return "Num 6";
        case GLFW_KEY_KP_7: return "Num 7";
        case GLFW_KEY_KP_8: return "Num 8";
        case GLFW_KEY_KP_9: return "Num 9";
        case GLFW_KEY_KP_DECIMAL:  return "Num .";
        case GLFW_KEY_KP_DIVIDE:   return "Num /";
        case GLFW_KEY_KP_MULTIPLY: return "Num *";
        case GLFW_KEY_KP_SUBTRACT: return "Num -";
        case GLFW_KEY_KP_ADD:      return "Num +";
        case GLFW_KEY_KP_ENTER:    return "Num Enter";
        case GLFW_KEY_KP_EQUAL:    return "Num =";
        // Modifiers
        case GLFW_KEY_LEFT_SHIFT:   return "Shift";
        case GLFW_KEY_LEFT_CONTROL: return "Ctrl";
        case GLFW_KEY_LEFT_ALT:     return "Alt";
        case GLFW_KEY_LEFT_SUPER:   return "Super";
        case GLFW_KEY_RIGHT_SHIFT:   return "RShift";
        case GLFW_KEY_RIGHT_CONTROL: return "RCtrl";
        case GLFW_KEY_RIGHT_ALT:     return "RAlt";
        case GLFW_KEY_RIGHT_SUPER:   return "RSuper";
        case GLFW_KEY_MENU:         return "Menu";
        default: return std::string("Key ") + std::to_string((int)k);
        }
    };

    //----------------------------------------------
    // CENTERED TWO COLUMN KEYBIND UI
    //----------------------------------------------
        std::string title = "Key Bindings";
        ImVec2 titleSize = ImGui::CalcTextSize(title.c_str());
        ImGui::SetCursorPosX((windowWidth - titleSize.x) * 0.5f);
        ImGui::Text("%s", title.c_str());

        // --- Centered binding rows ---
        float totalBindWidth = 400.f;
        float margin = (windowWidth - totalBindWidth) * 0.5f;
        float labelWidth = 200.f;
        float cellHeight = ImGui::GetTextLineHeightWithSpacing();

        for (int i = 0; i < NUM_ACTIONS; ++i)
        {
            float row_start_y = ImGui::GetCursorPosY();
            ImGui::SetCursorPosX(margin);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%s", ActionNames[i]);
            ImGui::SameLine(margin + labelWidth);

            std::string buttonId = std::string("##keybind") + std::to_string(i);
            std::string btnLabel = (remapActionIndex == i && waitingForKey) ? "Press key..." : GetKeyLabel(actionKeys[i]);

            if (ImGui::Button((btnLabel + buttonId).c_str(), ImVec2(buttonWidth, 0)))
            {
                remapActionIndex = i;
                waitingForKey = true;
            }
            ImGui::SetCursorPosY(row_start_y + cellHeight);
        }

    ImGui::Text(" ");
    ImGui::PopItemWidth();

    // Apply key binding
    if (player) {
        auto fpc = player->findComponent<FirstPersonControllerComponent>();
        if (fpc) {
            fpc->setActionKey(FirstPersonControllerComponent::MoveForward, actionKeys[MoveForward]);
            fpc->setActionKey(FirstPersonControllerComponent::MoveBackward, actionKeys[MoveBackward]);
            fpc->setActionKey(FirstPersonControllerComponent::MoveLeft, actionKeys[MoveLeft]);
            fpc->setActionKey(FirstPersonControllerComponent::MoveRight, actionKeys[MoveRight]);
            fpc->setActionKey(FirstPersonControllerComponent::Jump, actionKeys[Jump]);
            fpc->setActionKey(FirstPersonControllerComponent::Sprint, actionKeys[Sprint]);
            fpc->setActionKey(FirstPersonControllerComponent::Slide, actionKeys[Slide]);
        }
    }

    AudioManager::instance().setVolume("music", musicVolume);

    if (player) {
        auto fpc = player->findComponent<FirstPersonControllerComponent>();
        if (fpc) {
            fpc->setMouseXSensivity(mouseXSensivity);
            fpc->setMouseYSensivity(mouseYSensivity);
        }
    }

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Go Back", ImVec2(buttonWidth, 40)) ||
		(!enteredState && !waitingForKey && (
			ImGui::IsKeyPressed(ImGuiKey_Escape)
			))
		) {
		menuType = MainPauseMenu;
		enteredState = true;
	}

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void PauseMenu::quitMenu() {
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
	io.FontGlobalScale = 2.0f;

	if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
		io.KeyMap[ImGuiKey_Space] = KEY_ENTER;
	}

	if (ImGui::IsKeyDown(ImGuiKey_Space)) {
		io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get the display size
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	// Set next window to cover the entire screen
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(displaySize);

	ImGui::Begin("Quit Menu", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	drawList->AddRectFilled(ImVec2(0, 0), displaySize, IM_COL32(0, 0, 0, 96));

	// Center the menu
	ImVec2 center = ImVec2(displaySize.x / 2, displaySize.y / 2);
	ImGui::SetCursorPos(ImVec2(center.x - 200, center.y - 100));

	float windowWidth = ImGui::GetWindowSize().x;
	float textWidth = ImGui::CalcTextSize("Are you sure you want to quit?").x;
	ImGui::Text("Are you sure you want to quit?");

	float buttonWidth = 200.0f;

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Go Back", ImVec2(buttonWidth, 40)) ||
		(!enteredState && (
			ImGui::IsKeyPressed(ImGuiKey_Escape)
			))
	){
		menuType = MainPauseMenu;
		enteredState = true;
	}

	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button("Quit", ImVec2(buttonWidth, 40))) {
		quit = true;
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return;
}

void PauseMenu::run() {

	if (enteredState && !input->isKeyDown(KEY_ESCAPE))
		enteredState = false;

	if (menuType == MainPauseMenu) {
		mainPauseMenu();
	}
	else if (menuType == HowToPlay) {
		howToPlay();
	}
	else if (menuType == Settings) {
		settings();
	}
	else if (menuType == Quit) {
		quitMenu();
	}
	else if (menuType == Credits) {
		credits();
	}
	else {
		frc->clearPhysicsTime();
		isPaused = false;
		input->controlMouse(true);
	}
}
