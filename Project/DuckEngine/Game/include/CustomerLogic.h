#pragma once

#include "DuckEngine.h"
#include "StateMachine.h"
#include "CustomerIdleState.h"
#include "CustomerWalkState.h"
#include "GameLogicComponent.h"

class CustomerLogic : public GameLogic
{
public:
	StateMachine stateMachine;
	CustomerIdleState IdleState;
	CustomerWalkState WalkState;

	CustomerLogic(GameLogicComponent* component)
		: GameLogic(component), IdleState(this), WalkState(this)
	{
	}
};
