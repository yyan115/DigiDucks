#include <iostream>
#include "CustomerWalkState.h"
#include "CustomerLogic.h"

CustomerWalkState::CustomerWalkState(CustomerLogic* customerLogicOwner)
	: State(static_cast<GameLogic*>(customerLogicOwner)) {}

void CustomerWalkState::Enter()
{
	std::cout << "Customer enters Walk State" << std::endl;
}


void CustomerWalkState::Update()
{

}

void CustomerWalkState::Exit()
{
	std::cout << "Customer exits Walk State" << std::endl;
}