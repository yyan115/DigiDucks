#include "CustomerLogic.h"

void CustomerLogic::Start()
{
	IdleState = new CustomerIdleState(this);
	WalkState = new CustomerWalkState(this);
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


CustomerLogic::~CustomerLogic()
{
	delete IdleState;
	delete WalkState;
}