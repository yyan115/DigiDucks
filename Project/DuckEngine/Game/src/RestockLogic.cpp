/******************************************************************************/
/*!
\file       RestockLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       Feb 6 2025
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
		{
			maintenanceTextures.push_back(AssetManager::GetTextureByName("maintenance1"));
			maintenanceTextures.push_back(AssetManager::GetTextureByName("maintenance2"));
			maintenanceTextures.push_back(AssetManager::GetTextureByName("maintenance3"));
			maintenanceTextures.push_back(AssetManager::GetTextureByName("maintenance4"));

			UpdateMaintenanceMenu();
		}
	}

	auto restockExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Exit_Btn").get();
	if (restockExitBtn)
	{
		restockExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockExitBtn->entityID);
		if (restockExitButton)
		{
			
			restockExitButton->onClick = [this, restockExitBtn]() {
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(restockExitBtn->entityID)->Play();
				RestockMenu(false);
				};

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
			maintainenceStartButton->onClick = [this]() { MiniGame_1(true); };
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

	// Cart Stock Sprites
	std::string spriteName = "Cart_Stock_";
	for (int i = 0; i < MAX_CART_STOCK; i++)
	{
		std::cout << "Cart_Stock_" << i << std::endl;
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
				cartStockButtons[i]->onClick = [this, i]()
					{
						if (cartStock.size() > i)
						{
							RemoveFromCart(cartStock[i]);
						}
					};
			}
		}
	}

	ingredientBtnTextures.push_back(AssetManager::GetTextureByName("lettucebutton"));
	ingredientBtnTextures.push_back(AssetManager::GetTextureByName("lettucebutton_active"));
	ingredientBtnTextures.push_back(AssetManager::GetTextureByName("shrimpbutton"));
	ingredientBtnTextures.push_back(AssetManager::GetTextureByName("shrimpbutton_active"));
	ingredientBtnTextures.push_back(AssetManager::GetTextureByName("tomatobutton"));
	ingredientBtnTextures.push_back(AssetManager::GetTextureByName("tomatobutton_active"));


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
				ingredientButtons[i]->onClick = [this, i]()
					{
						AddToCart(static_cast<ItemType>(i));
					};
				ingredientButtons[i]->onHover = [this, i]()
					{
						ingredientSprites[i]->texture = ingredientBtnTextures[i * 2 + 1];
					};
				ingredientButtons[i]->onFinishHover = [this, i]()
					{
						ingredientSprites[i]->texture = ingredientBtnTextures[i * 2];
					};
			}
		}
	}

	// Texture for all items
	//itemTextures.push_back(AssetManager::GetTextureByName("bun"));
	//itemTextures.push_back(AssetManager::GetTextureByName("cheese"));
	itemTextures.push_back(AssetManager::GetTextureByName("lettuce"));
	//itemTextures.push_back(AssetManager::GetTextureByName("mushroom"));
	itemTextures.push_back(AssetManager::GetTextureByName("shrimp"));
	//itemTextures.push_back(AssetManager::GetTextureByName("steak"));
	itemTextures.push_back(AssetManager::GetTextureByName("tomato"));


	auto restockConfirmBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Confirm_Btn").get();
	if (restockConfirmBtn)
	{
		restockConfirmButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockConfirmBtn->entityID);
		if (restockConfirmButton)
		{
			restockConfirmButton->onClick = [this]()
				{
					for (ItemType type : cartStock)
					{
						Restock(type);
					}
					RefreshCart();
				};
			restockConfirmButton->isEnabled = false;
		}
		restockConfirmSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(restockConfirmBtn->entityID);
		if (restockConfirmSpt)
		{
			restockConfirmOn = AssetManager::GetTextureByName("confirm_on");
			restockConfirmOff = AssetManager::GetTextureByName("confirm_off");
			restockConfirmSpt->texture = restockConfirmOff;
		}
	}

	auto restockClearAllBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Clear_Btn").get();
	if (restockClearAllBtn)
	{
		restockClearAllButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(restockClearAllBtn->entityID);
		if (restockClearAllButton)
		{
			restockClearAllButton->onClick = [this]()
				{
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
			IncreaseMaintenanceLevel();
			return;
		}
	}

	// Error message
	std::cout << "RestockLogic::Restock: No stock of type " << static_cast<int>(type) << " found." << std::endl;
}


void RestockLogic::UpdateMaintenanceMenu()
{
	if(restockMaintenanceSpt)
		restockMaintenanceMenuSpt->texture = maintenanceTextures[maintenanceLevel];
}

void RestockLogic::LowerMaintenanceLevel()
{
	if (maintenanceLevel <= 0)
		return;
	maintenanceLevel--;
	UpdateMaintenanceMenu();
}

void RestockLogic::IncreaseMaintenanceLevel()
{
	if (maintenanceLevel >= 3)
		return;
	maintenanceLevel++;
	UpdateMaintenanceMenu();
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

void RestockLogic::MiniGame_1(bool state)
{
	/*
	*  Minigame Logic here.
	*/
	isMiniGame = state;
}