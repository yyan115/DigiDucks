#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>


enum class IngredientType
{
	EMPTY,
	BUN,
	CHEESE,
	LETTUCE,
	MUSHROOM,
	SHRIMP,
	STEAK,
	TOMATO
};

class StockLogic : public GameLogic
{
private:
	IngredientType type;
	int stock;

public:

	StockLogic() : GameLogic(nullptr), type(IngredientType::BUN), stock(5) {}

	StockLogic(IngredientType type, int stock_) : GameLogic(nullptr), type(type), stock(stock_) {}

	StockLogic(GameLogicComponent* component, IngredientType type, int stock_) : GameLogic(nullptr), type(type), stock(stock_) {}

	void Start() override{}

	void Update() override {}

	void FixedUpdate() override {}

	virtual void Restock() { std::cout << "RESTOCKING "; stock = 5; }

	virtual void useStock() 
	{ 
		if (stock > 0) 
		{ 
			stock--; 
			std::cout << "Left with " << stock << " worth of stock" << std::endl; 
		}
		else 
		{ 
			Restock(); 
		}
	}

	virtual IngredientType getType() { return type; }

};