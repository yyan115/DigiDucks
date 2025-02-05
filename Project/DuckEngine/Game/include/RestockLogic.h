/******************************************************************************/
/*!
\file       RestockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Declaration of the Restock Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"

#define MAX_MAINTENANCE_LEVEL 5;
#define MAX_CART_STOCK 5;

class RestockLogic : public GameLogic
{
private:

	// Restock Menu Sprite
	SpriteRendererComponent* restockMenuSpt = nullptr;

	// Restock Ingredient Menu Sprite
	SpriteRendererComponent* restockIngredientMenuSpt = nullptr;
	// Restock Maintenance Menu Sprite
	SpriteRendererComponent* restockMaintenanceMenuSpt = nullptr;

	// Exit Button
	ButtonComponent* restockExitButton = nullptr;

	// Restock Ingredient Button & Sprite
	ButtonComponent* restockIngredientButton = nullptr;
	SpriteRendererComponent* restockIngredientSpt = nullptr;
	Texture restockIngredientOn{};
	Texture restockIngredientOff{};

	// Restock Maintenance Button & Sprite
	ButtonComponent* restockMaintenanceButton = nullptr;
	SpriteRendererComponent* restockMaintenanceSpt = nullptr;
	Texture restockMaintenanceOn{};
	Texture restockMaintenanceOff{};
	int maintenanceLevel = 0;

	// Maintainence Start Button
	ButtonComponent* maintainenceStartButton = nullptr;

	// Restock All Button
	ButtonComponent* restockAllButton = nullptr;

	std::vector<ItemType> cartStock;
	bool changeCartStock = false;

public:
	bool isRestock = false;

	RestockLogic() : GameLogic(nullptr) {}

	RestockLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}


	/****************************************************************
	* @brief Start function for the Restock Station Logic.
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Restock Station Logic.
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Fixed Update function for the Restock Station Logic.
	* ****************************************************************/
	void FixedUpdate() override;


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<RestockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	/****************************************************************
	* @brief Function that handles the state of the restock menu.
	* ****************************************************************/
	void RestockMenu(bool state);

	/****************************************************************
	* @brief Restock all items in the restock station.
	* ****************************************************************/
	void RestockAll();

	/****************************************************************
	* @brief Restock a specific item in the restock station.
	*
	* @param type - The type of item to restock.
	* ****************************************************************/
	void Restock(ItemType type);

	/****************************************************************
	* @brief Decrease the maintenance level of the Restock station.
	* ****************************************************************/
	void LowerMaintenanceLevel();

	/****************************************************************
	* @brief Increase the maintenance level of the Restock station.
	* ****************************************************************/
	void IncreaseMaintenanceLevel();


	void RefreshCart()
	{

	}
};