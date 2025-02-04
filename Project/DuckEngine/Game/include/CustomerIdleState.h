#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerIdleState : public State<CustomerLogic>
{
public:
	explicit CustomerIdleState(CustomerLogic* customerLogicOwner);

	void Initialize(CustomerLogic* customerLogicOwner);
	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;
	
};
