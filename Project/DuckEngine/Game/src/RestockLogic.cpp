/******************************************************************************/
/*!
\file       RestockLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
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

		if (restockMenu->childEntities.size() > 0)
		{
			for (int i = 0; i < restockMenu->childEntities.size(); i++)
			{
				sliderLogic = GameLogicManager::GetLogicForEntity<SliderLogic>(restockMenu->childEntities[i]->entityID);
				if (sliderLogic)
					break;
			}
		}
	}

	auto restockIngredientMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Ingredient_Menu").get();
	if (restockIngredientMenu)
	{
		restockIngredientMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockIngredientMenu->entityID);
		if (restockIngredientMenuSpt)
			std::cout << "Restock Ingredient Menu Found" << std::endl;
	}

	auto restockExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Exit_Btn").get();
	if (restockExitBtn)
	{
		restockExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockExitBtn->entityID);
		if (restockExitButton)
		{

			restockExitButton->onClick = [this, restockExitBtn]() 
				{
					DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(restockExitBtn->entityID)->Play();
					RestockMenu(false);
				};

		}
	}


	auto restockAllBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_All_Btn").get();
	if (restockAllBtn)
	{
		restockAllButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockAllBtn->entityID);
		if (restockAllButton)
		{
			restockAllButton->onClick = [this, restockAllBtn]() 
				{
					DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(restockAllBtn->entityID)->Play();
					RestockAll(); 
				};
		}
	}

	// Cart Stock Sprites
	std::string spriteName = "Cart_Stock_";
	for (int i = 0; i < MAX_CART_STOCK; i++)
	{
		auto cartStockSprite = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(spriteName + std::to_string(i)).get();
		if (cartStockSprite)
		{
			auto cartStockSpriteSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(cartStockSprite->entityID);
			if (cartStockSpriteSpt)
			{
				cartStockSprites.push_back(cartStockSpriteSpt);
				cartStockSprites[i]->isVisible = false;
			}
			auto cartStockButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(cartStockSprite->entityID);
			if (cartStockButton)
			{
				cartStockButtons.push_back(cartStockButton);
				cartStockButtons[i]->onClick = [this, i, cartStockSprite]()
					{
						DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(cartStockSprite->entityID)->Play();
						if (cartStock.size() > i)
						{
							RemoveFromCart(cartStock[i]);
						}
					};
			}
		}
	}

	// Add Images Of Ingredients Buttons
	for (int i = 0; i < 7; i++)
	{
		std::string ingredient = whatType(static_cast<ItemType>(i));
		ingredientBtnTextures.push_back(AssetManager::GetTextureByName(ingredient + "_unselected"));
		ingredientBtnTextures.push_back(AssetManager::GetTextureByName(ingredient + "_selected"));

		itemTextures.push_back(AssetManager::GetTextureByName(ingredient));
	}


	// Ingredient Stock Buttons
	std::string ingredientName = "Restock_Ingredient_";
	for (int i = 0; i < 7; i++)
	{
		auto ingredientBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(ingredientName + std::to_string(i)).get();
		if (ingredientBtn)
		{
			auto ingredientSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(ingredientBtn->entityID);
			if (ingredientSprite)
			{
				ingredientSprites.push_back(ingredientSprite);
				ingredientSprites[i]->texture = ingredientBtnTextures[i*2];
			}
			auto ingredientButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(ingredientBtn->entityID);
			if (ingredientButton)
			{
				ingredientButtons.push_back(ingredientButton);
				ingredientButtons[i]->onClick = [this, i, ingredientBtn]()
					{
						DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(ingredientBtn->entityID)->Play();
						AddToCart(static_cast<ItemType>(i));
					};

				// Change Texture when Hovering
				ingredientButtons[i]->onHover = [this, i]()
					{
						ingredientSprites[i]->texture = ingredientBtnTextures[i * 2 + 1];
					};

				// Change Texture when not Hovering
				ingredientButtons[i]->onFinishHover = [this, i]()
					{
						ingredientSprites[i]->texture = ingredientBtnTextures[i * 2];
					};
			}
		}
	}

	// Find Restock Confirm Button
	auto restockConfirmBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Confirm_Btn").get();
	if (restockConfirmBtn)
	{
		// Set Button Functionality
		restockConfirmButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockConfirmBtn->entityID);
		if (restockConfirmButton)
		{
			restockConfirmButton->onClick = [this, restockConfirmBtn]()
				{
					DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(restockConfirmBtn->entityID)->Play();
					for (ItemType type : cartStock)
					{
						Restock(type);
					}
					RefreshCart();
				};
			restockConfirmButton->isEnabled = false;
		}

		// Set Button Texture
		restockConfirmSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockConfirmBtn->entityID);
		if (restockConfirmSpt)
		{
			restockConfirmOn = AssetManager::GetTextureByName("confirm_on");
			restockConfirmOff = AssetManager::GetTextureByName("confirm_off");
			restockConfirmSpt->texture = restockConfirmOff;
		}
	}

	// Find Clear Cart Button
	auto restockClearAllBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Clear_Btn").get();
	if (restockClearAllBtn)
	{
		// Set Button Functionality
		restockClearAllButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockClearAllBtn->entityID);
		if (restockClearAllButton)
		{
			restockClearAllButton->onClick = [this, restockClearAllBtn]()
				{
					DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(restockClearAllBtn->entityID)->Play();
					RefreshCart();
				};
		}
	}
}


/****************************************************************
* @brief Update function for the Restock Station Logic.
* ****************************************************************/
void RestockLogic::Update()
{
	// If cart stock has changed
	if (changeCartStock)
	{
		UpdateCartMenu();
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
			stockLogic->addStock();
			return;
		}
	}

	// Error message
	std::cout << "RestockLogic::Restock: No stock of type " << static_cast<int>(type) << " found." << std::endl;
}

void RestockLogic::RefreshCart()
{
	cartStock.clear();
	changeCartStock = true;
}

void RestockLogic::UpdateCartMenu()
{
	for (int i = 0; i < 5; i++)
	{
		if (cartStockSprites[i])
		{
			if (i < cartStock.size())
			{
				cartStockSprites[i]->texture = itemTextures[static_cast<int>(cartStock[i])];
				cartStockSprites[i]->isVisible = true;

			}
			else
			{
				cartStockSprites[i]->isVisible = false;
			}
		}
	}
	if (cartStock.size() > 0)
	{
		restockConfirmButton->isEnabled = true;
		restockConfirmSpt->texture = restockConfirmOn;
	}
	else
	{
		restockConfirmButton->isEnabled = false;
		restockConfirmSpt->texture = restockConfirmOff;
	}
}

void RestockLogic::AddToCart(ItemType type)
{
	if (cartStock.size() >= MAX_CART_STOCK)
		return;
	cartStock.push_back(type);
	changeCartStock = true;
}

void RestockLogic::RemoveFromCart(ItemType type)
{
	for (int i = 0; i < cartStock.size(); i++)
	{
		if (cartStock[i] == type)
		{
			cartStock.erase(cartStock.begin() + i);
			changeCartStock = true;
			return;
		}
	}
	std::cout << "No item of type " << static_cast<int>(type) << " found in cart." << std::endl;
}