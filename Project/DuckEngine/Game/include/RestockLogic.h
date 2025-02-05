/******************************************************************************/
/*!
\file       RestockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 29 2024
\brief      Declaration of the Restock Station Logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"

class RestockLogic : public GameLogic
{
private:
	Entity* restockMenu = nullptr;
	SpriteRendererComponent* restockMenuSpt = nullptr;



	ButtonComponent* maintainenceButton = nullptr;

	ButtonComponent* restockAllButton = nullptr;

	ButtonComponent* restockExitButton = nullptr;

public:
	bool isRestock = false;

	RestockLogic() : GameLogic(nullptr) {}

	RestockLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}


	/****************************************************************
	* @brief Start function for the Restock Station Logic.
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Restock Station Logic.
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Fixed Update function for the Restock Station Logic.
	* ****************************************************************/
	void FixedUpdate() override;


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<RestockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void RestockMenu(bool state);

	/****************************************************************
	* @brief Restock all items in the restock station.
	* ****************************************************************/
	void RestockAll();


	/****************************************************************
	* @brief Restock a specific item in the restock station.
	*
	* @param type - The type of item to restock.
	* ****************************************************************/
	void Restock(ItemType type);

};