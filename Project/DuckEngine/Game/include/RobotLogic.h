/******************************************************************************/
/*!
\file       RobotLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       Feb 04 2025
\brief      Declaration of Robot Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "RestockLogic.h"

class RobotLogic : public GameLogic
{
private:
	Entity* robot;
	AnimatorComponent* robotAni;

public:

	RobotLogic() :
		GameLogic(nullptr), robot(nullptr), robotAni(nullptr) {}

	RobotLogic(GameLogicComponent* component) :
		GameLogic(nullptr), robot(nullptr), robotAni(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<RobotLogic>(*this);
		clone->component = nullptr;
		return clone;
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

};