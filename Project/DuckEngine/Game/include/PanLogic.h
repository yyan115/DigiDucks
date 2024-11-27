#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>


class PanLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* object;
	TransformComponent* objectTransform;
	SpriteRendererComponent* objectSprite;
	IngredientType type;
	float cookTime;

public:
	bool isCooked = false;
	bool isOccupied = false;

	PanLogic() :
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(IngredientType::EMPTY), cookTime(3.f) {}

	PanLogic(GameLogicComponent* component) :
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(IngredientType::EMPTY), cookTime(3.f) {}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<PanLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void setObject(std::pair<int, IngredientType> objData);

	std::pair<int, IngredientType> moveObject();

	void cookObject();

	IngredientType getType() const { return type; }

	void makeEmptyPan();
};