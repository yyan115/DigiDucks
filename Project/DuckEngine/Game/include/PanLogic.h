/******************************************************************************/
/*!
\file       PanLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Pan Logic functions

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


class PanLogic : public GameLogic
{
private:
	Entity* table;
	TransformComponent* tableTransform;
	Entity* object;
	TransformComponent* objectTransform;
	SpriteRendererComponent* objectSprite;
	ItemType type;
	float cookTime;

public:
	bool isCooked = false;
	bool isOccupied = false;

	PanLogic() :
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(ItemType::EMPTY), cookTime(1.f) {}

	PanLogic(GameLogicComponent* component) :
		GameLogic(nullptr), table(nullptr), tableTransform(nullptr), object(nullptr), objectTransform(nullptr), objectSprite(nullptr), type(ItemType::EMPTY), cookTime(1.f)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<PanLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the Pan Logic
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Pan Logic
	* ****************************************************************/
	void Update() override;
	
	/****************************************************************
	* @brief FixedUpdate function for the Pan Logic
	* ****************************************************************/
	void FixedUpdate() override;

	/****************************************************************
	* @brief Set the object in the pan
	* 
	* @param objData - the object to be set in the pan
	* ****************************************************************/
	void setObject(std::pair<int, ItemType> objData);

	/****************************************************************
	* @brief Get the object in the pan
	* 
	* @return - the object in the pan
	* ****************************************************************/
	std::pair<int, ItemType> moveObject();

	/****************************************************************
	* @brief Cook the object in the pan
	* ****************************************************************/
	void cookObject();

	/****************************************************************
	* @brief Get the type of the object in the pan
	* 
	* @return - the type of the object in the pan
	* ****************************************************************/
	ItemType getType() const { return type; }

	/****************************************************************
	* @brief Make the pan empty
	* ****************************************************************/
	void makeEmptyPan();
};