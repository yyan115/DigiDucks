#include <iostream>
#include "CustomerIdleState.h"
#include "CustomerLogic.h"

CustomerIdleState::CustomerIdleState(CustomerLogic* customerLogicOwner)
	: State(static_cast<GameLogic*>(customerLogicOwner)) {}

void CustomerIdleState::Enter()
{
	std::cout << "Customer enters Idle State" << std::endl;
}


void CustomerIdleState::Update()
{

}

void CustomerIdleState::Exit()
{
	std::cout << "Customer exits Idle State" << std::endl;
}