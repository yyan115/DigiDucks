#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerWalkState : public State<CustomerLogic>
{
public:
	explicit CustomerWalkState(CustomerLogic* customerLogicOwner);

	void Initialize(CustomerLogic* customerLogicOwner);
	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;

private:
	Entity* queueTarget;


};