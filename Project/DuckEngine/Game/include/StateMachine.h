#pragma once

#include "DuckEngine.h"
#include "State.h"

class StateMachine
{
public:
	State* currentState;

	StateMachine() : currentState(nullptr) {}

	void ChangeState(State* newState);
	void Update();
};