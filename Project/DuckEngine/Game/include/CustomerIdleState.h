#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerIdleState : public State
{
public:
	CustomerIdleState(CustomerLogic* customerLogicOwner) : State(customerLogicOwner) {}
	
	void Enter() override;
	void Update() override;
	void Exit() override;



};