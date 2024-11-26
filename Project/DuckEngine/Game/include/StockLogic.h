#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>


enum class IngredientType
{
	EMPTY,
	BIN,
	BUN,
	CHEESE,
	C_CHEESE,	// Cut Cheese
	LETTUCE,
	C_LETTUCE,	// Cut Lettuce
	MUSHROOM,
	C_MUSHROOM,	// Cut Mushroom
	SHRIMP,
	C_SHRIMP,	// Cut Shrimp
	STEAK,
	R_PATTY,	// Raw Patty
	C_PATTY,	// Cooked Patty
	TOMATO,
	C_TOMATO	// Cut Tomato
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

	bool isEmpty() const { return stock == 0; }

	virtual IngredientType getType() { return type; }

	void changeType(IngredientType type_) { type = type_; }

};