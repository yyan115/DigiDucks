#include "CustomerLogic.h"

void CustomerLogic::Start()
{
	stateMachine.ChangeState(&WalkState);
}

void CustomerLogic::Update()
{
	stateMachine.currentState->Update();
}

void CustomerLogic::FixedUpdate()
{
	stateMachine.currentState->FixedUpdate();
}
