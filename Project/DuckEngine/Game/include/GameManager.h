#pragma once

#include "DuckEngine.h"
#include "MaxLoadScene.h"
#include "SpriteMovementScene.h"

class GameManager
{
public:
	static std::string ActiveSceneName;
	static DuckEngine DuckEngine;
	static void InitScenes();
	static void SetActiveScene(std::string sceneName);
	static std::string GetActiveScene();

};

