#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class TableLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* objectOnTable;
	TransformComponent* objectTransform;

public:
	bool isOccupied = false;

	TableLogic() : GameLogic(nullptr), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr) {}

	TableLogic(GameLogicComponent* component) : 
		GameLogic(component), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr) {}

	void Start() override;

	void Update() override;

	void FixedUpdate() override;

	void setObject(int EntityID);

	int moveObject();
};