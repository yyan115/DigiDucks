/******************************************************************************/
/*!
\file       GetPanLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 4 2025
\brief      Declartion of all GetPan Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "TableLogic.h"


// This Logic is Only for Entity with TableLogic
// This Logic is used to find the Pan and set it to TableLogic Item
class GetPanLogic : public GameLogic
{
private:

public:

	GetPanLogic() : GameLogic(nullptr) {}

	GetPanLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}


	/****************************************************************
	* @brief Start function for the Score Logic.
	* ****************************************************************/
	void Start() override
	{
		auto tableLogic = GameLogicManager::GetLogicForEntity<TableLogic>(component->GetEntityID());

		if (tableLogic)
		{
			tableLogic->setPan();
		}
		else
		{
			std::cout << "Table Logic Not Found" << std::endl;
		}
	}

	/****************************************************************
	* @brief Update function for the Score Logic.
	* ****************************************************************/
	void Update() override {}

	/****************************************************************
	* @brief Fixed Update function for the Score Logic.
	* ****************************************************************/
	void FixedUpdate() override {}


	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<GetPanLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};