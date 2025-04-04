/******************************************************************************/
/*!
\file       GameScene.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Implementation of the GameScene class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level1.h"
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
void Level1::Load() 
{
	Scene::Load();
}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level1::Start() 
{
	Scene::Start();
	CameraManager::SetHeight(24);
	ScoreLogic::dayNumber = 1;

	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	GameLoopLogic* gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();


	// Level 1 Salad Only
	// 2 Customers
	for (int i = 0; i < gameLoopLogic->customers.size(); i++)
	{
		//gameLoopLogic->customers[0]->MaxCashierWaitingTime = 20.0f;
		//gameLoopLogic->customers[0]->MaxTableWaitingTime = 40.0f;
		int random = rand() % 4;
		switch (random)
		{
		case 0:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_PLATE);
			std::cout << whatType(ItemType::LETTUCE_PLATE) << std::endl;
			break;
		case 1:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_SHRIMP_PLATE);
			std::cout << whatType(ItemType::LETTUCE_SHRIMP_PLATE) << std::endl;
			break;
		case 2:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_TOMATO_PLATE);
			std::cout << whatType(ItemType::LETTUCE_TOMATO_PLATE) << std::endl;
			break;
		case 3:
			gameLoopLogic->customers[i]->SetOrder(ItemType::SALAD_PLATE);
			std::cout << whatType(ItemType::SALAD_PLATE) << std::endl;
			break;
		};
	}

	// Set Timer
	gameLoopLogic->timeLeft = 120.0f;
}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level1::Update() 
{	


}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level1::PostUpdate()
{
	
}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level1::Exit() 
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level1::Unload() 
{
	// base unload
	Scene::Unload();

}