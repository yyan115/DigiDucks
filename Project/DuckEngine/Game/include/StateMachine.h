#pragma once

#include "DuckEngine.h"
#include "State.h"

template<typename T>
class StateMachine
{
public:
	State<T>* currentState;

	StateMachine() : currentState(nullptr) {}

	void ChangeState(State<T>* newState)
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

	void Update()
	{
		if (currentState != nullptr)
		{
			currentState->Update();
		}
	}

	void FixedUpdate()
	{
		if (currentState != nullptr)
		{
			currentState->FixedUpdate();
		}
	}
};