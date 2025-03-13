/******************************************************************************/
/*!
\file       CustomerIdleState.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the CustomerIdleState class, which represents the idle
			state for a customer entity. In this state, the customer remains
			stationary until transitioning to another state based on game
			logic conditions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerIdleState : public State<CustomerLogic>
{
public:
	explicit CustomerIdleState(CustomerLogic* customerLogicOwner);

	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;
	
};
