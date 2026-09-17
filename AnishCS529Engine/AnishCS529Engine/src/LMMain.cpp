/*!****************************************************************************
 * \file   LMMain.cpp
 * \author Nicholas Shaw (nick.shaw@digipen.edu) (Level Management)
 * 
 * Copyright © 2025 DIGIPEN Institute of Technology. All rights reserved.
 * 
 *****************************************************************************/
#include "precompiled.h"

#include "LevelManager.h"

// extern "C"
// {
//     __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//     __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
// }

#ifndef _DEBUG

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif

int main()
{

    LevelManager::Instance().initialize();
    //LevelManager::Instance().DisplayLogos();

    //Main Loop
    while (!LevelManager::Instance().GameComplete()) {
        LevelManager::Instance().RunLevels();
    }
    LevelManager::Instance().shutdown();

    return 0;
}
