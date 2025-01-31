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

	restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
	if (restockMenu)
	{
		restockMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockMenu->entityID);
		if (restockMenuSpt)
		{
			restockMenuSpt->isVisible = false;
		}
	}
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

void RestockLogic::RestockMenu()
{
	restockMenuSpt->isVisible = !restockMenuSpt->isVisible;
}

/****************************************************************
* @brief Restock all items in the restock station.
* ****************************************************************/
void RestockLogic::RestockAll()
{
    std::vector<int> allEntities = GameLogicManager::GetAllEntitiesWithLogic<StockLogic>();

	for (int entityID : allEntities)
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(entityID);
		stockLogic->restock();
	}
}
