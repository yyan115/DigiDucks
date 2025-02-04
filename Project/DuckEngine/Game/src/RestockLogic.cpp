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

/****************************************************************
* @brief Start function for the Restock Station Logic.
* ****************************************************************/
void RestockLogic::Start()
{
	std::cout << "RestockLogic::Start" << std::endl;
	restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
	if (restockMenu)
	{
		restockMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockMenu->entityID);
		if (restockMenuSpt)
		{
			restockMenuSpt->isVisible = false;
		}
	}

	maintainenceBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Maintainence_Btn").get();
	if (maintainenceBtn)
	{
		maintainenceButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(maintainenceBtn->entityID);
		if (maintainenceButton)
		{
			maintainenceButton->onClick = [this]() { /*Mainatinence Func*/ };
		}
}


/****************************************************************
* @brief Update function for the Restock Station Logic.
* ****************************************************************/
void RestockLogic::Update()
{

}

void RestockLogic::FixedUpdate()
{
}

void RestockLogic::RestockMenu(bool state)
{
	// Disable/Enable Restock Menu
	if (restockMenuSpt)
	{
		std::cout << "Restock Menu" << std::endl;
		restockMenuSpt->isVisible = state;
	}
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

void RestockLogic::Restock(ItemType type) 
{
	std::vector<int> allEntities = GameLogicManager::GetAllEntitiesWithLogic<StockLogic>();
	for (int entityID : allEntities)
	{
		auto stockLogic = GameLogicManager::GetLogicForEntity<StockLogic>(entityID);
		if (stockLogic->getType() == type)
		{
			stockLogic->restock();
			return;
		}
	}

	// Error message
	std::cout << "RestockLogic::Restock: No stock of type " << static_cast<int>(type) << " found." << std::endl;
}