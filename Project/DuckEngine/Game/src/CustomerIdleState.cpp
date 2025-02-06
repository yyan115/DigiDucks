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