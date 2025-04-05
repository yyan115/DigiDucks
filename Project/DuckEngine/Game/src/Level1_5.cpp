/******************************************************************************/
/*!
\file       Level15.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Implementation of the Level15 class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level1_5.h"
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
void Level1_5::Load()
{
	Scene::Load();

}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level1_5::Start()
{
	Scene::Start();
	GameManager::SetGlobalVariable("LastPlayedScene", "Level1_5");

	ScoreLogic::dayNumber = 2;

	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();


	Entity* tutorialEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("TutorialSprite").get();
	tutorialTexture = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(tutorialEntity->entityID);

	gameLoopLogic->customers[0]->SetOrder(ItemType::BURGER_PLATE);
	gameLoopLogic->customers[0]->MaxCashierWaitingTime = 300;
	gameLoopLogic->customers[0]->MaxTableWaitingTime = 300;

}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level1_5::Update()
{
	if (gameLoopLogic->ordersTaken > 0)
	{
		tutorialTexture->isVisible = true;
	}
}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level1_5::PostUpdate()
{

}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level1_5::Exit()
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level1_5::Unload()
{
	// base unload
	Scene::Unload();

}