/******************************************************************************/
/*!
\file       ChopBoardLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Chopping Board Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"
#include <iostream>


class ChopBoardLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* object;
	TransformComponent* objectTransform;
	SpriteRendererComponent* objectSprite;
	ItemType type;
	float chopTime;

public:
	bool isChopped = false;
	bool isOccupied = false;

	ChopBoardLogic() : 
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(ItemType::EMPTY), chopTime(2.f) {}

	ChopBoardLogic(GameLogicComponent* component) :
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(ItemType::EMPTY), chopTime(2.f) {}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<ChopBoardLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void setObject(std::pair<int, ItemType> objData);

	std::pair<int, ItemType> moveObject();

	void chopObject();

	ItemType getType() const { return type; }
};