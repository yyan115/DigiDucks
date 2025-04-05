/******************************************************************************/
/*!
\file       Level0.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 18 2024
\brief      Implementation of the GameScene class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level0.h"
#include "Scene.h"
#include "CutSceneLogic.h"
#include "ScoreLogic.h"
#include "GameLogicManager.h"
#include "LevelSelectScreenLogic.h"
#include "GameManager.h"



/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/
void Level0::Load()
{
	Scene::Load();

}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level0::Start()
{
	Scene::Start();
	GameManager::SetGlobalVariable("LastPlayedScene", "Level0");
	ScoreLogic::dayNumber = 0;

	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();

	Entity* tutorialEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("TutorialSprite").get();
	tutorialTexture = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(tutorialEntity->entityID);
	tutorialTexture->isVisible = false;

	Entity* tutorialEntity2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("TutorialSprite2").get();
	tutorialTexture2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(tutorialEntity2->entityID);
	tutorialTexture2->isVisible = false;
	
	gameLoopLogic->customers[0]->SetOrder(ItemType::LETTUCE_TOMATO_PLATE);
	gameLoopLogic->customers[0]->MaxCashierWaitingTime = 300;
	gameLoopLogic->customers[0]->MaxTableWaitingTime = 300;


}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level0::Update()
{
	if (gameLoopLogic->IsGameStarted() && gameLoopLogic->ordersTaken < 1)
	{
		tutorialTexture->isVisible = true;
	}

	if (gameLoopLogic->ordersTaken > 0)
	{
		tutorialTexture->isVisible = false;
		tutorialTexture2->isVisible = true;
	}

	gameLoopLogic->timeLeft = 180.0f;
}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level0::PostUpdate()
{

}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level0::Exit()
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level0::Unload()
{
	// base unload
	Scene::Unload();

}