#include "StateMachine.h"

void StateMachine::ChangeState(State* newState)
{
	if (currentState != nullptr)
	{
		currentState->Exit();
	}

	currentState = newState;

	if (currentState != nullptr)
	{
		currentState->Enter();
	}
}

void StateMachine::Update()
{
	if (currentState != nullptr)
	{
		currentState->Update();
	}
}

void StateMachine::FixedUpdate()
{
	if (currentState != nullptr)
	{
		currentState->FixedUpdate();
	}
}
