/******************************************************************************/
/*!
\file       CustomerWalkState.h
\author     Lucas Yee JunJie, l.yee, 2301212 (50%)
			Yan Yu, y.yan, 2301213 (50%)
\par        l.yee@digipen.edu, y.yan@digipen.edu
\date       March 13 2025
\brief      Declares the CustomerWalkState class, which represents the state
			where a customer moves through different waypoints, including the
			queue, waiting area, and seating area. The class handles movement,
			animations, and transitions between states based on order status.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include <iostream>
#include "State.h"
#include <tuple>

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

	bool isOrderTaken = false;
	bool orderCollected = false;
	bool isWalkingToSeat = false;
	bool isWaitingToCollectOrder = false;
	size_t currentTargetIndex = 0;
	std::pair<Entity*, bool>* customerSeat{};

	bool customerAngryLeave = false;

	std::vector<Entity*> seatPoints{};
	Entity* customerSeatEntity = nullptr;

	Entity* currentQueueTarget = nullptr;

	bool orderCollectedNowLeave = false;
	std::vector<Entity*> waitTargets{};
	std::vector<Entity*> queueTargets{};

private:
	Entity* finalPath = nullptr;
	AnimatorComponent* customerAnimator = nullptr;

	// points for customer to move to when leaving
	std::vector<Entity*> leaveTargets{};

	// points for customer to move to after taking order

	// points for customer to move to to the queue area to wait for his order

	//std::vector<std::tuple<Entity*, bool, std::vector<Entity*>>> seatingLocations{};






};