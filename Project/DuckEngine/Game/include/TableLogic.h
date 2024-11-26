#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class TableLogic : public GameLogic
{
private:
	TransformComponent* tableTransform;
	Entity* objectOnTable;
	TransformComponent* objectTransform;

public:
	Entity* table;
	bool isOccupied = false;

	TableLogic() : GameLogic(nullptr), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr) {}

	TableLogic(GameLogicComponent* component) : 
		GameLogic(component), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr) {}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<TableLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;

	void Update() override;

	void FixedUpdate() override;

	void setObject(int EntityID);

	int moveObject();
};