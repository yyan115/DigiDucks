/******************************************************************************/
/*!
\file       Level35.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Implementation of the Level35 class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Level35.h"
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
void Level35::Load()
{
	Scene::Load();

}

/****************************************************************
* @brief Start the scene, initializing game objects and
* setting up the initial state. This function is called when
* the scene begins running.
* ****************************************************************/
void Level35::Start()
{
	Scene::Start();

	ScoreLogic::dayNumber = 1;

	Entity* gameLoopEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	GameLoopLogic* gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLoopEntity->entityID).get();


	for (int i = 0; i < 4; i++)
	{
		// Random number from 0 to 4
		int random = rand() % 4;
		switch (random)
		{
		case 0:
			gameLoopLogic->customers[i]->SetOrder(ItemType::CHEESE_BURGER_PLATE);
			break;
		case 1:
			gameLoopLogic->customers[i]->SetOrder(ItemType::SALAD_PLATE);
			break;
		case 2:
			gameLoopLogic->customers[i]->SetOrder(ItemType::LETTUCE_PLATE);
			break;
		case 3:
			gameLoopLogic->customers[i]->SetOrder(ItemType::BOWL_MUSHROOM);
			break;
		case 4:
			gameLoopLogic->customers[i]->SetOrder(ItemType::BOWL_TOMATO);
			break;
		};
	}

	LevelSelectScreenLogic::currentStage = 6;

	//gameLoopLogic->customers[0]->SetOrder(ItemType::CHEESE_BURGER_PLATE);
	//gameLoopLogic->customers[1]->SetOrder(ItemType::SALAD_PLATE);
	//gameLoopLogic->customers[2]->SetOrder(ItemType::CHEESE_BURGER_PLATE);
	//gameLoopLogic->customers[3]->SetOrder(ItemType::SALAD_PLATE);
}

/****************************************************************
* @brief Update the scene logic each frame, including processing
* input and updating game objects. This function is called
* every frame.
* ****************************************************************/
void Level35::Update()
{


}

/****************************************************************
* @brief PostUpdate function performs any operations after the
* main update logic, such as cleanup or post-processing. This
* function is called every frame, after the Update() method.
* ****************************************************************/
void Level35::PostUpdate()
{

}

/****************************************************************
* @brief Exit the scene, performing any cleanup necessary before
* the scene is unloaded.
* ****************************************************************/
void Level35::Exit()
{

}

/****************************************************************
* @brief Unload the scene and free any resources that were loaded
* during the Load() phase.
* ****************************************************************/
void Level35::Unload()
{
	// base unload
	Scene::Unload();

}