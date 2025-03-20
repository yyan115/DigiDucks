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
#include "PotLogic.h"

class TableLogic : public GameLogic
{
private:
	Entity* table = nullptr;
	TransformComponent* tableTransform = nullptr;
	Entity* objectOnTable = nullptr;
	TransformComponent* objectTransform = nullptr;
	ItemType objType = ItemType::EMPTY;

	std::shared_ptr<PotLogic> potLogic = nullptr;

public:
	bool isOccupied = false;

	TableLogic() : GameLogic(nullptr) {}

	TableLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<TableLogic>(*this);
		clone->component = nullptr;
		return clone;
	}


	/****************************************************************
	* @brief Start function for the Table Logic.
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Table Logic.
	* ****************************************************************/
	void Update() override {}

	/****************************************************************
	* @brief FixedUpdate function for the Table Logic.
	* ****************************************************************/
	void FixedUpdate() override {}

	/****************************************************************
	* @brief Shutdown function for the Table Logic.
	* 
	* @param objData - The object data to set.
	* ****************************************************************/
	void setObject(std::pair<int,ItemType> objData);

	/****************************************************************
	* @brief Move the object from the table.
	* 
	* @return The object data to move.
	* ****************************************************************/
	std::pair<int, ItemType> moveObject();

	/****************************************************************
	* @brief Find and Set the Pan on the table.
	* ****************************************************************/
	void setPan();

	/****************************************************************
	* @brief Find and Set the Pot on the table.
	* ****************************************************************/
	void setPot();

	/****************************************************************
	* @brief Get the object type on the table.
	* 
	* @return The object type.
	* ****************************************************************/
	ItemType getType() const { return objType; }

	/****************************************************************
	* @brief Set the object type on the table.
	* 
	* @param type - The object type to set.
	* ****************************************************************/
	void setType(ItemType type) { objType = type; }


	bool isPotFilled();

	ItemType moveSoup();
};