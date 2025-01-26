/******************************************************************************/
/*!
\file       RestockLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Definition of the Restock Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "RestockLogic.h"
#include "StockLogic.h"
#include "GameLogicManager.h"

/****************************************************************
* @brief Start function for the Restock Station Logic.
* ****************************************************************/
void RestockLogic::Start()
{
	restockStation = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	restockTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
}


/****************************************************************
* @brief Update function for the Restock Station Logic.
* ****************************************************************/
void RestockLogic::Update()
{
	if (animator)
	{
		animator->PlayAnimation("Idle");
	}
}

/****************************************************************
* @brief Restock all items in the restock station.
* ****************************************************************/
void RestockLogic::restockAll()
{
    std::vector<int> allEntities = GameLogicManager::GetAllEntitiesWithLogic<StockLogic>();

	for (int entityID : allEntities)
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(entityID);
		stockLogic->restock();
	}
}
