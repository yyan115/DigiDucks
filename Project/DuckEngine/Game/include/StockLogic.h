/******************************************************************************/
/*!
\file       StockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 25 2024
\brief      Declartion of all Chopping Board Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>

#include "IngredientType.h"


class StockLogic : public GameLogic
{
private:
	ItemType type;
	SpriteRendererComponent* spriteRenderer;
	int stock;

public:

	StockLogic() : GameLogic(nullptr), type(ItemType::EMPTY), spriteRenderer(nullptr), stock(5) {}

	StockLogic(ItemType type, int stock_) : GameLogic(nullptr), type(type), spriteRenderer(nullptr), stock(stock_) {}

	StockLogic(GameLogicComponent* component, ItemType type, int stock_) : GameLogic(nullptr), type(type), spriteRenderer(nullptr), stock(stock_) {}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<StockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override 
	{
		spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());
	}

	void Update() override 
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

	void FixedUpdate() override {}

	virtual void restock() { stock = 5; }

	virtual void useStock() 
	{ 
		if (stock > 0) 
		{ 
			stock--; 
		}
	}

	bool isEmpty() const { return stock == 0; }

	virtual ItemType getType() { return type; }

	void changeType(ItemType type_) { type = type_; }

};