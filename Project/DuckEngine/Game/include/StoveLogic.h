/******************************************************************************/
/*!
\file       StoveLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Stove Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"
#include "SliderLogic.h"
#include <iostream>


class StoveLogic : public GameLogic
{
private:
	Entity* table = nullptr;
	TransformComponent* tableTransform = nullptr;
	SoundComponent* tableSFX = nullptr;
	Entity* object = nullptr;
	TransformComponent* objectTransform = nullptr;
	SpriteRendererComponent* objectSprite = nullptr;
	ItemType type = ItemType::EMPTY;
	float currCookTime = 0.0f;
	const float cookTime = 3.5f;

	std::shared_ptr<SliderLogic> sliderLogic = nullptr;

	bool isCooked = false;

public:
	bool isPot = false;
	bool isPan = false;
	bool isOccupied = false;

	StoveLogic() : GameLogic(nullptr) {}

	StoveLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<StoveLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the Stove Logic
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Stove Logic
	* ****************************************************************/
	void Update() override;
	
	/****************************************************************
	* @brief FixedUpdate function for the Stove Logic
	* ****************************************************************/
	void FixedUpdate() override;

	/****************************************************************
	* @brief Set the object on the stove.
	* 
	* @param objData - the object to be set on the stove
	* ****************************************************************/
	void setObject(std::pair<int, ItemType> objData);

	/****************************************************************
	* @brief Get the object on the stove
	* 
	* @return - Id and type of the object on the Stove
	* ****************************************************************/
	std::pair<int, ItemType> moveObject();

	/****************************************************************
	* @brief Clear the Pot and return type of Soup
	* 
	* @return - the type of soup
	* ****************************************************************/
	ItemType moveSoup();

	/****************************************************************
	* @brief Clear the Pan and return type of Patty
	* 
	* @return - the type of patty
	* ****************************************************************/
	ItemType movePatty();

	/****************************************************************
	* @brief Cook the object on the stove
	* ****************************************************************/
	void cookObject();

	/****************************************************************
	* @brief Get the type of the object on the Stove
	* 
	* @return - the type of the object on the stove
	* ****************************************************************/
	ItemType getType() const { return type; }

	/****************************************************************
	* @brief Set the Object on the Stove to the provided Item Type.
	* 
	* @param objType - the type of object to be set
	* 
	* @return - true if the object is set
	* ****************************************************************/
	bool setObjectSprite(ItemType objType);

	/****************************************************************
	* @brief Set the cooking type of the stove
	* 
	* @param objType - Either Pan or Pot
	* ****************************************************************/
	void setCookingType(std::pair<int, ItemType> objData);

	void EmitSparks();
};