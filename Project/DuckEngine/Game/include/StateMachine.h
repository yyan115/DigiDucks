#include "DuckEngine.h"
#include "State.h"

class StateMachine
{
public:
	State* currentState;

	StateMachine() : currentState(nullptr) {}

	void ChangeState(State* newState)
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

};