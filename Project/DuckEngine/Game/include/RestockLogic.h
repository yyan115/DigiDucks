/******************************************************************************/
/*!
\file       RestockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       Feb 6 2025
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

const int MAX_CART_STOCK = 5;

class RestockLogic : public GameLogic
{
private:

	// Restock Menu Sprite
	SpriteRendererComponent* restockMenuSpt = nullptr;

	// Restock Ingredient Menu Sprite
	SpriteRendererComponent* restockIngredientMenuSpt = nullptr;

	// Exit Button
	ButtonComponent* restockExitButton = nullptr;

	// Ingredient Restocks
	ButtonComponent* restockBunButton = nullptr;
	ButtonComponent* restockCheeseButton = nullptr;
	ButtonComponent* restockLettuceButton = nullptr;
	ButtonComponent* restockMushroomButton = nullptr;
	ButtonComponent* restockShrimpButton = nullptr;
	ButtonComponent* restockSteakButton = nullptr;
	ButtonComponent* restockTomatoButton = nullptr;

	// Cart Stock Max of 5 items
	std::vector<SpriteRendererComponent*> cartStockSprites;
	std::vector<ButtonComponent*> cartStockButtons;

	// Texture for All Items
	std::vector<Texture> itemTextures;

	// Ingredient Stock Buttons
	std::vector<ButtonComponent*> ingredientButtons;
	std::vector<SpriteRendererComponent*> ingredientSprites;
	std::vector<Texture> ingredientBtnTextures;


	ButtonComponent* restockAllButton = nullptr;
	ButtonComponent* restockConfirmButton = nullptr;
	SpriteRendererComponent* restockConfirmSpt = nullptr;
	Texture restockConfirmOn{};
	Texture restockConfirmOff{};
	ButtonComponent* restockClearAllButton = nullptr;

	std::vector<ItemType> cartStock;
	bool changeCartStock = false;

	float delay = 3.5f;
	float delayCounter = 0.f;

public:
	bool isRestock = false;
	bool isDelay = false;

	RestockLogic() : GameLogic(nullptr) {}

	RestockLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<RestockLogic>(*this);
		clone->component = nullptr;
		return clone;
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
	* @brief Add an item to the cart.
	* 
	* @param type - The type of item to add to the cart.
	* ****************************************************************/
	void AddToCart(ItemType type);

	/****************************************************************
	* @brief Remove an item from the cart.
	* 
	* @param type - The type of item to remove from the cart.
	* ****************************************************************/
	void RemoveFromCart(ItemType type);

	/****************************************************************
	* @brief Remove all items from the cart.
	* ****************************************************************/
	void RefreshCart();

	/****************************************************************
	* @brief Update the sprite renderers for the cart stock.
	* ****************************************************************/
	void UpdateCartMenu();

};