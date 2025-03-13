/******************************************************************************/
/*!
\file       CustomerIdleState.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Implements the CustomerIdleState class, which defines the
			behavior of a customer in the idle state. The customer remains
			stationary until transitioning to another state, such as
			walking or waiting to give an order.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CustomerIdleState.h"
#include "CustomerLogic.h"
#include "GameScene.h"

CustomerIdleState::CustomerIdleState(CustomerLogic* customerLogicOwner)
	: State<CustomerLogic>(customerLogicOwner) {}

void CustomerIdleState::Enter()
{
	std::cout << "Customer enters Idle State" << std::endl;
}


void CustomerIdleState::Update()
{
}

void CustomerIdleState::FixedUpdate()
{

}

void CustomerIdleState::Exit()
{
	std::cout << "Customer exits Idle State" << std::endl;
}