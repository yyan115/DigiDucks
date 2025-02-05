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
	auto restockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();;
	if (restockMenu)
	{
		restockMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());
		if (restockMenuSpt)
		{
			restockMenuSpt->isVisible = false;
		}
	}

	auto restockIngredientMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Ingredient_Menu").get();
	if (restockIngredientMenu)
	{
		restockIngredientMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockIngredientMenu->entityID);
		if(restockIngredientMenuSpt)
			std::cout << "Restock Ingredient Menu Found" << std::endl;
	}

	auto restockMaintenanceMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Maintenance_Menu").get();
	if (restockMaintenanceMenu)
	{
		restockMaintenanceMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockMaintenanceMenu->entityID);
		if (restockMaintenanceMenuSpt)
			std::cout << "Restock Maintenance Menu Found" << std::endl;
	}

	auto restockExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Exit_Btn").get();
	if (restockExitBtn)
	{
		restockExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockExitBtn->entityID);
		if (restockExitButton)
		{
			restockExitButton->onClick = [this]() { RestockMenu(false); };
		}
	}


	auto restockIngredientBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Ingredient_Btn").get();
	if (restockIngredientBtn)
	{
		restockIngredientOn = AssetManager::GetTextureByName("restock_ingredient_on");
		restockIngredientOff = AssetManager::GetTextureByName("restock_ingredient_off");
		restockIngredientSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockIngredientBtn->entityID);
		if (restockIngredientSpt)
		{
			restockIngredientSpt->texture = restockIngredientOn;
			restockIngredientButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockIngredientBtn->entityID);
			if (restockIngredientButton) {
				restockIngredientButton->onClick = [this]()
					{
						// Swap Texture
						restockIngredientSpt->texture = restockIngredientOn;
						restockMaintenanceSpt->texture = restockMaintenanceOff;

						// Enable Ingredient Menu, Disable Maintenance Menu
						restockIngredientMenuSpt->isVisible = true;
						restockMaintenanceMenuSpt->isVisible = false;
					};
			}
		}
	}

	auto restockMaintenanceBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Maintenance_Btn").get();
	if (restockMaintenanceBtn)
	{		
		restockMaintenanceOn = AssetManager::GetTextureByName("restock_maintenance_on");
		restockMaintenanceOff = AssetManager::GetTextureByName("restock_maintenance_off");
		restockMaintenanceSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockMaintenanceBtn->entityID);
		if (restockMaintenanceSpt)
		{
			restockMaintenanceSpt->texture = restockMaintenanceOff;
			restockMaintenanceButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockMaintenanceBtn->entityID);
			if (restockMaintenanceButton)
			{
				restockMaintenanceButton->onClick = [this]()
					{
						// Swap Texture
						restockIngredientSpt->texture = restockIngredientOff;
						restockMaintenanceSpt->texture = restockMaintenanceOn;

						// Enable Maintenance Menu, Disable Ingredient Menu
						restockIngredientMenuSpt->isVisible = false;
						restockMaintenanceMenuSpt->isVisible = true;
					};
			}
		}
	}


	auto maintainenceStartBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Maintainence_Start_Btn").get();
	if (maintainenceStartBtn)
	{
		maintainenceStartButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(maintainenceStartBtn->entityID);
		if (maintainenceStartButton)
		{
			maintainenceStartButton->onClick = [this]() { std::cout << "Start Mini Game" << std::endl;/*Mini Game Code goes HERE*/ };
		}
	}

	auto restockAllBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_All_Btn").get();
	if (restockAllBtn)
	{
		restockAllButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockAllBtn->entityID);
		if (restockAllButton)
		{
			restockAllButton->onClick = [this]() { RestockAll(); };
		}
	}
}


/****************************************************************
* @brief Update function for the Restock Station Logic.
* ****************************************************************/
void RestockLogic::Update()
{
	if (maintenanceLevel >= 5)
	{
		// Disable restock button
		if (restockAllButton)
		{
			restockAllButton->isEnabled = false;
		}
	}

	// If cart stock has changed
	if (changeCartStock)
	{


		changeCartStock = false;
	}
}

void RestockLogic::FixedUpdate()
{
}

void RestockLogic::RestockMenu(bool state)
{
	// Disable/Enable Restock Menu
	if (restockMenuSpt)
	{
		restockMenuSpt->isVisible = state;
	}

	isRestock = state;
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

void RestockLogic::LowerMaintenanceLevel()
{
	maintenanceLevel--;
}

void RestockLogic::IncreaseMaintenanceLevel()
{
	maintenanceLevel++;
}