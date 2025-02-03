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

	CustomerLogic()
		: GameLogic(nullptr), stateMachine(), IdleState(this), WalkState(this)
	{
	}

	CustomerLogic(GameLogicComponent* component)
		: GameLogic(component), IdleState(this), WalkState(this)
	{
	}

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<CustomerLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

private:
	Entity* queueTarget;

};
