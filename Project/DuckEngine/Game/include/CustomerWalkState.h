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
	Entity* currentQueueTarget = nullptr;
	Entity* finalPath = nullptr;
	AnimatorComponent* customerAnimator = nullptr;

	// points for customer to move to when leaving
	std::vector<Entity*> leaveTargets{};

	// points for customer to move to after taking order
	std::vector<Entity*> waitTargets{};

	// points for customer to move to to the queue area to wait for his order
	std::vector<Entity*> queueTargets{};

	std::vector<std::pair<Entity*, bool>> seatingLocations{};

	std::pair<Entity*, bool>* customerSeat{};

	bool isOrderTaken = false;
	bool orderCollected = false;
	bool isWalkingToSeat = false;
	bool isWaitingToCollectOrder = false;
	size_t currentTargetIndex = 0;
	bool customerEnded = false;




};