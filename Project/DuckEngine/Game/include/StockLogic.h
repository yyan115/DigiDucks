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
	int stock;

public:

	StockLogic() : GameLogic(nullptr), type(ItemType::EMPTY), stock(5) {}

	StockLogic(ItemType type, int stock_) : GameLogic(nullptr), type(type), stock(stock_) {}

	StockLogic(GameLogicComponent* component, ItemType type, int stock_) : GameLogic(nullptr), type(type), stock(stock_) {}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<StockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override {}

	void Update() override {}

	void FixedUpdate() override {}

	virtual void restock() { std::cout << "RESTOCKING "; stock = 5; }

	virtual void useStock() 
	{ 
		if (stock > 0) 
		{ 
			stock--; 
			std::cout << "Left with " << stock << " worth of stock" << std::endl; 
		}
	}

	bool isEmpty() const { return stock == 0; }

	virtual ItemType getType() { return type; }

	void changeType(ItemType type_) { type = type_; }

};