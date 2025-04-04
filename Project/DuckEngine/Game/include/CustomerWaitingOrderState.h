/******************************************************************************/
/*!
\file       CustomerWaitingOrderState.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the CustomerWaitingOrderState class, which represents
			the state where a customer waits to give their order. This state
			handles proximity-based interaction with the player, manages
			order tab availability, and transitions the customer to the next
			state after placing an order.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerWaitingOrderState : public State<CustomerLogic>
{
public:
	explicit CustomerWaitingOrderState(CustomerLogic* customerLogicOwner);

	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;

	bool GetIsOrderTaken() { return orderTaken; }

private:
	bool orderTaken = false;
	bool orderCollected = false;

	bool playGreenAnimation = false;
	bool playYellowAnimation = false;
	bool playRedAnimation = false;
};
