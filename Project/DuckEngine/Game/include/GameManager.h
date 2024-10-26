#pragma once

#include "DuckEngine.h"
#include "MaxLoadScene.h"
#include "SpriteMovementScene.h"

class GameManager
{
public:
	static DuckEngine DuckEngine;
	static void InitScenes();
	static void SetActiveScene(std::string sceneName);

};

