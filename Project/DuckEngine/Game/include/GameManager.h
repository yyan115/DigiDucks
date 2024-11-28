#pragma once

#include "DuckEngine.h"
#include "MaxLoadScene.h"
#include "SpriteMovementScene.h"
#include "GameScene.h"
#include "MainMenu.h"
#include "EndGame.h"

class GameManager
{
public:
	static std::string ActiveSceneName;
	static bool ShouldChangeScene;
	static DuckEngine DuckEngine;


	static void InitScenes();
	static void SetActiveScene(std::string sceneName, bool transition = true);
	static void Update();
};

