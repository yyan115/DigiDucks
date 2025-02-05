#pragma once

#include "DuckEngine.h"
#include "State.h"

template<typename T>
class StateMachine
{
public:
	std::shared_ptr<State<T>> currentState;

	StateMachine() : currentState(nullptr) {}

	void ChangeState(std::shared_ptr<State<T>> newState)
	{
		if (currentState != nullptr)
		{
			currentState->Exit();
		}

		currentState = std::move(newState);

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