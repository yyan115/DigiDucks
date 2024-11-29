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

class RestockLogic : public GameLogic
{
private:
	Entity* restockStation;
	TransformComponent* restockTransform;

public:

	RestockLogic() :
		GameLogic(nullptr), restockStation(nullptr), restockTransform(nullptr) {}

	RestockLogic(GameLogicComponent* component) :
		GameLogic(nullptr), restockStation(nullptr), restockTransform(nullptr)
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
	void Update() override {}

	/****************************************************************
	* @brief Fixed Update function for the Restock Station Logic.
	* ****************************************************************/
	void FixedUpdate() override {}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<RestockLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	/****************************************************************
	* @brief Restock all items in the restock station.
	* ****************************************************************/
	void restockAll();

};