#include "CustomerLogic.h"

void CustomerLogic::Start()
{
	IdleState = std::make_shared<CustomerIdleState>(this);
	WalkState = std::make_shared<CustomerWalkState>(this);
	stateMachine.ChangeState(WalkState);
}

void CustomerLogic::Update()
{
	stateMachine.currentState->Update();
}

void CustomerLogic::FixedUpdate()
{
	stateMachine.currentState->FixedUpdate();
}
