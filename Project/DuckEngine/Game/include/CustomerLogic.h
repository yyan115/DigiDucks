#pragma once

#include "DuckEngine.h"
#include "StateMachine.h"
#include "CustomerIdleState.h"
#include "CustomerWalkState.h"
#include "GameLogicComponent.h"

class CustomerLogic : public GameLogic
{
public:
	StateMachine<CustomerLogic> stateMachine;
	CustomerIdleState* IdleState;
	CustomerWalkState* WalkState;

	CustomerLogic()
		: IdleState(nullptr), WalkState(nullptr) {
	}

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	~CustomerLogic();

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<CustomerLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
};
