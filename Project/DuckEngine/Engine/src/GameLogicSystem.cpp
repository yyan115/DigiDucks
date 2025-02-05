/******************************************************************************/
/*!
\file       GameLogicSystem.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Implements the GameLogicSystem class, which coordinates the
			initialization, updating, and fixed updating of game logic
			components for entities in the game engine. This system interacts
			with the DuckEngine Component Manager and the GameLogicManager
			to handle logic execution.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameLogicSystem.h"
#include "DuckEngine.h"
#include "GameLogicManager.h"
#include "GameLogicComponent.h"

/**************************************************************************
* @brief Starts all game logic components by initializing them.
* @details Iterates through all GameLogicComponents in the DuckEngine
*          Component Manager, retrieves their logic names, and clones
*          the corresponding base logic from the GameLogicManager. The
*          cloned logic is set up and started before being added to
*          the entity's logic map in the GameLogicManager.
**************************************************************************/
void GameLogicSystem::Start()
{
	auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

	for (auto& [entityID, component] : components)
	{
		auto* logicComponent = static_cast<GameLogicComponent*>(component.get());

		for (const auto& logicName : logicComponent->logicNames)
		{
			auto baseLogic = GameLogicManager::GetLogic(logicName);
			if (baseLogic)
			{
				auto logic = baseLogic->Clone();
				logic->SetComponent(logicComponent);
				GameLogicManager::AddLogicToEntity(entityID, logic);
			}
		}

	}

	for (auto& [entityID, component] : components)
	{
		auto& logics = GameLogicManager::GetLogicsForEntity(entityID);
		for (auto& logic : logics)
		{
			logic->Start();
		}
	}
}

/**************************************************************************
* @brief Updates all game logic components associated with entities.
* @details Iterates through all entities and retrieves their associated
*          logic objects from the GameLogicManager. Calls the Update
*          method for each logic object.
**************************************************************************/
void GameLogicSystem::Update()
{
	auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();
	for (auto& [entityID, component] : components)
	{
		auto logics = GameLogicManager::GetAllLogicsForEntity(entityID);
		for (auto& logic : logics)
		{
			if (logic)
			{
				logic->Update();
			}
		}
	}
}

/**************************************************************************
* @brief Performs fixed updates on all game logic components.
* @details Iterates through all entities and retrieves their associated
*          logic objects from the GameLogicManager. Calls the FixedUpdate
*          method for each logic object. This is typically used for tasks
*          that require consistent update intervals, such as physics updates.
**************************************************************************/
void GameLogicSystem::FixedUpdate()
{
	auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

	for (auto& [entityID, component] : components)
	{
		auto logics = GameLogicManager::GetAllLogicsForEntity(entityID);
		for (auto& logic : logics)
		{
			if (logic)
			{
				logic->FixedUpdate();
			}
		}
	}
}
