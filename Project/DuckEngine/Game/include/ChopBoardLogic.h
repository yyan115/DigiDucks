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
#include <SliderLogic.h>
#include <iostream>


class ChopBoardLogic : public GameLogic
{
private:
	Entity* table = nullptr;
	TransformComponent* tableTransform = nullptr;
	Entity* object = nullptr;
	TransformComponent* objectTransform = nullptr;
	SpriteRendererComponent* objectSprite = nullptr;
	ItemType type = ItemType::EMPTY;
	float chopTime = 1.0f;


	std::shared_ptr<SliderLogic> sliderLogic = nullptr;

public:
	bool isChopped = false;
	bool isOccupied = false;

	ChopBoardLogic() : GameLogic(nullptr) {}

	ChopBoardLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<ChopBoardLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the ChopBoardLogic
	* ***************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the ChopBoardLogic
	* ***************************************************************/
	void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for the ChopBoardLogic
	* ***************************************************************/
	void FixedUpdate() override;

	/****************************************************************
	* @brief Set the object on the chopping board
	* 
	* @param objData - the object data to be set
	* ***************************************************************/
	void setObject(std::pair<int, ItemType> objData);

	/****************************************************************
	* @brief Move the object on the chopping board
	* 
	* @return std::pair<int, ItemType> - the object data to be moved
	* ***************************************************************/
	std::pair<int, ItemType> moveObject();

	/****************************************************************
	* @brief Chop the object on the chopping board
	* ***************************************************************/
	void chopObject();

	/****************************************************************
	* @brief Get the type of the object on the chopping board
	*	
	* @return ItemType - the type of the object
	* ***************************************************************/
	ItemType getType() const { return type; }


	/****************************************************************
	* @brief Check if the object can be put on the chopping board
	* 
	* @param type - the type of the object
	* 
	* @return true if the object can be put on the chopping board, false otherwise
	* ***************************************************************/
	bool checkIngredient(ItemType ingredientType);
};