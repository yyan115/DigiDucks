#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerWalkState : public State
{
public:
	CustomerWalkState(CustomerLogic* customerLogicOwner);

	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;

private:
	CustomerLogic* customer;
	Entity* queueTarget;


};