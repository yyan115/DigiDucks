/******************************************************************************/
/*!
\file       StockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 25 2024
\brief      Declartion of base functions of all Stock Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>
#include "TextComponent.h"
#include "IngredientType.h"


class StockLogic : public GameLogic
{
private:
	Entity* entity;
	ItemType type;
	SpriteRendererComponent* spriteRenderer;
	TextComponent* textComponent;
	int stock;
	const int MAX_STOCK = 5;

public:

	StockLogic() : GameLogic(nullptr), entity(nullptr), type(ItemType::EMPTY), spriteRenderer(nullptr), textComponent(nullptr), stock(5) {}

	StockLogic(ItemType type, int stock_) : GameLogic(nullptr), entity(nullptr), type(type), spriteRenderer(nullptr), textComponent(nullptr), stock(stock_) {}

	StockLogic(GameLogicComponent* component, ItemType type, int stock_) : GameLogic(nullptr), entity(nullptr), type(type), spriteRenderer(nullptr), textComponent(nullptr), stock(stock_)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<StockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the StockLogic
	* ***************************************************************/
	void Start() override 
	{
		entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
		if (entity)
		{
			spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entity->entityID);
			textComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(entity->entityID);
			if (textComponent == nullptr)
			{
				if (entity->childEntities.size() > 0)
				{
					textComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(entity->childEntities[0]->entityID);
				}
			}
		}
	}

	/****************************************************************
	* @brief Update function for the StockLogic	*
	* ***************************************************************/
	void Update() override 
	{
		if (textComponent) 
		{
			textComponent->text = std::to_string(stock);
		}
	}

	/****************************************************************
	* @brief FixedUpdate function for the StockLogic
	* ***************************************************************/
	void FixedUpdate() override 
	{
		if (type == ItemType::BIN) return;

		if (stock <= 0)
		{
			if (type == ItemType::STEAK || type == ItemType::SHRIMP || type == ItemType::CHEESE)
			{
				spriteRenderer->texture = AssetManager::GetTextureByName("empty_fridge");
			}
			else {
				spriteRenderer->texture = AssetManager::GetTextureByName("empty_box");
			}
		}
		else {
			switch (type)
			{
			case ItemType::BUN:
				spriteRenderer->texture = AssetManager::GetTextureByName("bun_box");
				break;
			case ItemType::CHEESE:
				spriteRenderer->texture = AssetManager::GetTextureByName("cheese_fridge");
				break;
			case ItemType::LETTUCE:
				spriteRenderer->texture = AssetManager::GetTextureByName("lettuce_box");
				break;
			case ItemType::MUSHROOM:
				spriteRenderer->texture = AssetManager::GetTextureByName("mushroom_box");
				break;
			case ItemType::SHRIMP:
				spriteRenderer->texture = AssetManager::GetTextureByName("shrimp_fridge");
				break;
			case ItemType::STEAK:
				spriteRenderer->texture = AssetManager::GetTextureByName("steak_fridge");
				break;
			case ItemType::TOMATO:
				spriteRenderer->texture = AssetManager::GetTextureByName("tomato_box");
				break;
			};
		}
	}

	/****************************************************************
	* @brief Restock function for the StockLogic
	* ***************************************************************/
	virtual void restock() { stock = 5; }

	/****************************************************************
	* @brief Restock function for the StockLogic
	* ***************************************************************/
	virtual void addStock() { if (stock < MAX_STOCK) stock++; }

	/****************************************************************
	* @brief Use Stock function for the StockLogic
	* ***************************************************************/
	virtual void useStock() { if (stock > 0) { stock--; } }

	/****************************************************************
	* @brief Get Stock function for the StockLogic
	* ***************************************************************/
	bool isEmpty() const { return stock == 0; }

	/****************************************************************
	* @brief Get Stock function for the StockLogic
	* ***************************************************************/
	virtual ItemType getType() { return type; }

	/****************************************************************
	* @brief Get Stock function for the StockLogic
	* 
	* @type_ - ItemType to change to
	* ***************************************************************/
	void changeType(ItemType newType) { type = newType; }

};