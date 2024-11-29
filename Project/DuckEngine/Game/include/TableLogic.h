/******************************************************************************/
/*!
\file       TableLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 26 2024
\brief      Declaration of all Table logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"

class TableLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* objectOnTable;
	TransformComponent* objectTransform;
	ItemType type;

public:
	bool isOccupied = false;

	TableLogic() : 
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr), type(ItemType::EMPTY) {}

	TableLogic(GameLogicComponent* component) : 
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), objectOnTable(nullptr), objectTransform(nullptr), type(ItemType::EMPTY) 
	{
		UNREFERENCED_PARAMETER(component);
	}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<TableLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;

	void Update() override;

	void FixedUpdate() override;

	void setObject(std::pair<int,ItemType> objData);

	std::pair<int, ItemType> moveObject();

	ItemType getType() { return type; }
};