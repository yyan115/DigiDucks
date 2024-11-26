#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"

class TableLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* objectOnTable;
	TransformComponent* objectTransform;
	IngredientType type;

public:
	bool isOccupied = false;

	TableLogic() : 
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr), type(IngredientType::EMPTY) {}

	TableLogic(GameLogicComponent* component) : 
		GameLogic(component), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr), type(IngredientType::EMPTY) {}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<TableLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;

	void Update() override;

	void FixedUpdate() override;

	void setObject(std::pair<int,IngredientType> objData);

	std::pair<int, IngredientType> moveObject();
};