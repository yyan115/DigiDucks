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
	TOMATO,
	BIN
};

class StockLogic : public GameLogic
{
private:
	IngredientType type;
	int stock;

public:

	StockLogic() : GameLogic(nullptr), type(IngredientType::EMPTY), stock(5) {}

	StockLogic(IngredientType type, int stock_) : GameLogic(nullptr), type(type), stock(stock_) {}

	StockLogic(GameLogicComponent* component, IngredientType type, int stock_) : GameLogic(nullptr), type(type), stock(stock_) {}

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

	virtual void returnStock() { stock++; }

	virtual IngredientType getType() { return type; }

	void changeType(IngredientType type_) { type = type_; }

};