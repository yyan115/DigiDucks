/******************************************************************************/
/*!
\file       Level2_5.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Implementation of the Level2_5 class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level2_5.h"
#include "Scene.h"
#include "ScoreLogic.h"
#include "GameLoopLogic.h"
#include "LevelSelectScreenLogic.h"
#include "GameManager.h"

// state manger for this level
//CustomerStateManager stateManager;

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/
void Level2_5::Load()
{
	Scene::Load();

}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level2_5::Start()
{
	Scene::Start();

	ScoreLogic::dayNumber = 4;
	GameManager::SetGlobalVariable("LastPlayedScene", "Level2_5");


	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();

	Entity* tutorialEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("TutorialSprite").get();
	tutorialTexture = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(tutorialEntity->entityID);


	gameLoopLogic->customers[0]->SetOrder(ItemType::BOWL_MUSHROOM);
	gameLoopLogic->customers[0]->MaxCashierWaitingTime = 300;
	gameLoopLogic->customers[0]->MaxTableWaitingTime = 300;
	
}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level2_5::Update()
{
	if (gameLoopLogic->ordersTaken > 0)
	{
		tutorialTexture->isVisible = true;
	}
	gameLoopLogic->timeLeft = 180.0f;
}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level2_5::PostUpdate()
{

}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level2_5::Exit()
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level2_5::Unload()
{
	// base unload
	Scene::Unload();

}