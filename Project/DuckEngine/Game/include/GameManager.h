/******************************************************************************/
/*!
\file       GameManager.h
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Definition of the GameManager class, it handles the changing of scenes whenever it is called.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "DuckEngine.h"

class GameManager
{
public:
	static std::string ActiveSceneName;
	static bool ShouldChangeScene;
	static DuckEngine DuckEngine;
	static std::unordered_map<std::string, std::string> GlobalVariables;
	static bool GamepadDown;

	static void InitScenes();
/****************************************************************
* @brief Initializes all the scenes and gameplay logic for the game and is responsible for setting up all the scenes and game logic components that
* will be used throughout the game.
* ****************************************************************/
	static void SetActiveScene(std::string sceneName, bool transition = true);
/****************************************************************
* @brief Sets the active scene for the game, optionally triggering a scene transition.
* ****************************************************************/
	static void Update();
/****************************************************************
* @brief Updates the game state and handles scene transitions.
* ****************************************************************/

	static void SetGlobalVariable(const std::string& key, const std::string& value);
	static std::string GetGlobalVariable(const std::string& key);
};

