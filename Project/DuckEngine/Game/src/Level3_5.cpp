/******************************************************************************/
/*!
\file       Level3_5.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Implementation of the Level3_5 class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level3_5.h"
#include "Scene.h"
#include "ScoreLogic.h"
#include "GameLoopLogic.h"
#include "LevelSelectScreenLogic.h"

// state manger for this level
//CustomerStateManager stateManager;

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/
void Level3_5::Load()
{
	Scene::Load();

}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level3_5::Start()
{
	Scene::Start();

	ScoreLogic::dayNumber = 3;

	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	GameLoopLogic* gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();

	// Level 3.5 Burger, Salad and Soup
	// 4 Customers
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
			gameLoopLogic->customers[i]->SetOrder(ItemType::BOWL_MUSHROOM);
			break;
		case 5:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_SHRIMP_PLATE);
			break;
		case 6:
			gameLoopLogic->customers[i]->SetOrder(ItemType::BOWL_TOMATO);
			break;
		case 7:
			gameLoopLogic->customers[i]->SetOrder(ItemType::SALAD_PLATE);
			break;
		};
	}
}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level3_5::Update()
{


}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level3_5::PostUpdate()
{

}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level3_5::Exit()
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level3_5::Unload()
{
	// base unload
	Scene::Unload();

}