#pragma once
#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>


class ChopBoardLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* object;
	TransformComponent* objectTransform;
	SpriteRendererComponent* objectSprite;
	IngredientType type;
	float chopTime;

public:
	bool isChopped = false;
	bool isOccupied = false;

	ChopBoardLogic() : 
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(IngredientType::EMPTY), chopTime(2.f) {}

	ChopBoardLogic(GameLogicComponent* component) :
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(IngredientType::EMPTY), chopTime(2.f) {}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<ChopBoardLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void setObject(std::pair<int, IngredientType> objData);

	std::pair<int, IngredientType> moveObject();

	void chopObject();

	IngredientType getType() const { return type; }
};