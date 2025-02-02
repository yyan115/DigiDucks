#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerWalkState : public State
{
public:
	CustomerWalkState(CustomerLogic* customerLogicOwner) : State(customerLogicOwner) {}

	void Enter() override;
	void Update() override;
	void Exit() override;



};