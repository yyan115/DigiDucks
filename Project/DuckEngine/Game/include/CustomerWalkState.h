#pragma once

#include <iostream>
#include "State.h"

class CustomerLogic;

class CustomerWalkState : public State<CustomerLogic>
{
public:
	explicit CustomerWalkState(CustomerLogic* customerLogicOwner);
	void Enter() override;
	void Update() override;
	void FixedUpdate() override;
	void Exit() override;

	void CustomerOrderCollected() { orderCollected = true; }
	bool GetIsWaitingToCollectOrder() { return isWaitingToCollectOrder; }


private:
	std::vector<Entity*> queueTargets{};
	Entity* currentQueueTarget = nullptr;
	Entity* finalPath = nullptr;
	AnimatorComponent* customerAnimator = nullptr;


	bool isOrderTaken = false;
	bool orderCollected = false;
	bool isWaitingToCollectOrder = false;
	size_t currentTargetIndex = 0;
	bool customerEnded = false;




};