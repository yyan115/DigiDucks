/******************************************************************************/
/*!
\file       Level2.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Implementation of the Level2 class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level2.h"
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
void Level2::Load()
{
	Scene::Load();

}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level2::Start()
{
	Scene::Start();

	ScoreLogic::dayNumber = 2;
	GameManager::SetGlobalVariable("LastPlayedScene", "Level2");


	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	GameLoopLogic* gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();
	// Level 2 Burger and Cheese Burger
	// 2 Customers
	for (int i = 0; i < gameLoopLogic->customers.size(); i++)
	{
		int random = rand() % 8;
		switch (random)
		{
		case 0:
			gameLoopLogic->customers[i]->SetOrder(ItemType::BURGER_PLATE);
			break;
		case 1:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_PLATE);
			break;
		case 2:
			gameLoopLogic->customers[i]->SetOrder(ItemType::CHEESE_BURGER_PLATE);
			break;
		case 3:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_TOMATO_PLATE);
			break;
		case 4:
			gameLoopLogic->customers[i]->SetOrder(ItemType::BURGER_PLATE);
			break;
		case 5:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_SHRIMP_PLATE);
			break;
		case 6:
			gameLoopLogic->customers[i]->SetOrder(ItemType::CHEESE_BURGER_PLATE);
			break;
		case 7:
			gameLoopLogic->customers[i]->SetOrder(ItemType::SALAD_PLATE);
			break;
		};
	}

	// Set Timer
	gameLoopLogic->timeLeft = 180.0f;
}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level2::Update()
{

}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level2::PostUpdate()
{

}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level2::Exit()
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level2::Unload()
{
	// base unload
	Scene::Unload();

}