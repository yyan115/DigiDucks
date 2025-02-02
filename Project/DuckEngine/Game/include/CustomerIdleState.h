#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerIdleState : public State
{
public:
	CustomerIdleState(CustomerLogic* customerLogicOwner);

	void Enter() override;
	void Update() override;
	void Exit() override;
};
