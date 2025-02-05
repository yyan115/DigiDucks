#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerWaitingOrderState : public State<CustomerLogic>
{
public:
	explicit CustomerWaitingOrderState(CustomerLogic* customerLogicOwner);

	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;

private:
	bool orderTaken = false;

};
