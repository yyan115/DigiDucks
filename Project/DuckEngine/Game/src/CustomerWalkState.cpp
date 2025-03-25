/******************************************************************************/
/*!
\file       CustomerWalkState.cpp
\author     Lucas Yee JunJie, l.yee, 2301212 (50%)
			Yan Yu, y.yan, 2301213 (50%)
\par        l.yee@digipen.edu, y.yan@digipen.edu
\date       March 13 2025
\brief      Implements the CustomerWalkState class, handling customer movement
			through predefined waypoints such as queue positions, waiting
			areas, and seating locations. The state determines movement speed,
			manages animations, and transitions based on order collection and
			completion.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include <iostream>
#include <algorithm>
#include "CustomerWalkState.h"
#include "CustomerLogic.h"
#include "GameManager.h"
#include "GameLoopLogic.h"

Entity* customerSeatEntity = nullptr;

// Constructor
CustomerWalkState::CustomerWalkState(CustomerLogic* customerLogicOwner)
	: State<CustomerLogic>(customerLogicOwner)
	, currentQueueTarget(nullptr)
{
}

// Called when the customer enters the Walk State
void CustomerWalkState::Enter()
{
	std::cout << "Customer enters Walk State" << std::endl;

	// Get all entities
	auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

	// Collect all "Queue", "Wait" "Seat" and "Leave" entities
	for (auto& entity : entities)
	{
		if (entity->name.find("QueueUpPoint") != std::string::npos) {
			queueTargets.push_back(entity.get());
			//std::cout << "Added queue target: " << entity->name << "\n";
		}
		else if (entity->name.find("WaitPoint") != std::string::npos)
		{
			waitTargets.push_back(entity.get());
			//std::cout << "Added wait target: " << entity->name << "\n";
		}
		else if (entity->name.find("LeavePoint") != std::string::npos)
		{
			leaveTargets.push_back(entity.get());
			//std::cout << "Added leave target: " << entity->name << "\n";
		}
		else if (entity->name.find("Seat") != std::string::npos) {
			seatingLocations.emplace_back(entity.get(), false);
			//std::cout << "emplaced " << entity->name << "\n";
		}
	}

	// Sort them based on the suffix number (e.g., QueueUp_1, Wait_2, etc.)
	auto sortEntities = [](Entity* a, Entity* b) {
		int numA = std::stoi(a->name.substr(a->name.find_last_of('_') + 1));
		int numB = std::stoi(b->name.substr(b->name.find_last_of('_') + 1));
		return numA < numB;
		};

	// Different sort as this uses a pair
	auto sortEntitiesSeats = [](const std::pair<Entity*, bool>& a, const std::pair<Entity*, bool>& b) {
		std::string nameA = a.first->name;
		std::string nameB = b.first->name;

		// Find the underscore position
		size_t posA = nameA.find_last_of('_');
		size_t posB = nameB.find_last_of('_');

		// Ensure that there is a number after the underscore
		if (posA == std::string::npos || posB == std::string::npos) return false;

		std::string numStrA = nameA.substr(posA + 1);
		std::string numStrB = nameB.substr(posB + 1);

		// Validate if the extracted part is actually a number
		if (!std::all_of(numStrA.begin(), numStrA.end(), ::isdigit) ||
			!std::all_of(numStrB.begin(), numStrB.end(), ::isdigit)) {
			std::cerr << "Warning: Invalid entity name format -> " << nameA << " or " << nameB << "\n";
			return false; // Keep original order if invalid
		}

		int numA = std::stoi(numStrA);
		int numB = std::stoi(numStrB);
		return numA < numB;
		};


	std::sort(queueTargets.begin(), queueTargets.end(), sortEntities);
	std::sort(waitTargets.begin(), waitTargets.end(), sortEntities);
	std::sort(leaveTargets.begin(), leaveTargets.end(), sortEntities);
	std::sort(seatingLocations.begin(), seatingLocations.end(), sortEntitiesSeats);

	// Grab animator
	customerAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(owner->GetComponentID());

	// The logic says: Start by walking through the queue
	currentTargetIndex = 0;
	isOrderTaken = owner->WaitingOrderState->GetIsOrderTaken();

	if (!queueTargets.empty())
	{
		currentQueueTarget = queueTargets[currentTargetIndex];  // Start with queue
	}
	else
	{
		std::cerr << "No valid queue targets found!" << std::endl;
	}

	if (isOrderTaken && orderCollected && !leaveTargets.empty()) {
		currentQueueTarget = leaveTargets[0];
	}
	else if (isOrderTaken && !orderCollected && !waitTargets.empty()) {
		currentQueueTarget = waitTargets[0];
	}

	//std::cout << "Queue size = " << queueTargets.size()
	//    << ", Wait size = " << waitTargets.size()
	//    << ", Leave size = " << leaveTargets.size() << std::endl;

	GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
	if (gameLoop) 
	{
		seatingLocations = gameLoop->seatingLocations;
	}
}

// Update (not used, but kept for completeness)
void CustomerWalkState::Update()
{
}

// FixedUpdate: Movement, transitions, animations
void CustomerWalkState::FixedUpdate()
{
	TransformComponent* customerTransform =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->GetComponentID());

	if (!currentQueueTarget || !customerTransform)
	{
		return;
	}

	// Get the transform of the current target
	TransformComponent* targetTransform =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(currentQueueTarget->entityID);
	if (!targetTransform)
	{
		return;
	}

	// Calculate direction & distance
	Vec2 direction = targetTransform->GetPosition() - customerTransform->GetPosition();
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance > 0.0f)
	{
		direction.x /= distance;
		direction.y /= distance;
	}

	// Movement speed
	float moveSpeed = 5.0f;

	// Get rigidbody
	RigidbodyComponent* rigidbody =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(owner->GetComponentID());
	if (!rigidbody)
	{
		return;
	}

	if (currentQueueTarget == leaveTargets.back() && distance <= 0.1f)
	{
		owner->stateMachine.ChangeState(owner->IdleState);

		GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
		if (gameLoop)
		{
			gameLoop->customersFinished++;

			std::cout << "Customer finished: " << gameLoop->customersFinished
				<< " out of " << gameLoop->customerCount << std::endl;

			// Check if all customers have been served and have left
			if (gameLoop->customersFinished >= gameLoop->customerCount)
			{
				std::cout << "All customers have completed their journey. Transitioning to EndScene." << std::endl;
				GameManager::SetActiveScene("EndScene");
				return; // Add a return here to ensure we exit the function
			}
		}

		if (owner->GetGameLoopLogic()->currentCustomerIndex < owner->GetGameLoopLogic()->customerCount - 1)
		{
			std::cout << "NEXT CUSTOMER" << std::endl;

			if (gameLoop && !gameLoop->isSpawningCustomer)
			{
				// Check if any customer is currently waiting to place an order
				bool anyCustomerWaiting = false;
				for (auto* customer : gameLoop->customers)
				{
					if (customer->stateMachine.currentState == customer->WaitingOrderState)
					{
						anyCustomerWaiting = true;
						break;
					}
				}

				// Only spawn next customer if no one is waiting at the counter
				if (!anyCustomerWaiting)
				{
					gameLoop->isSpawningCustomer = true;
					gameLoop->timeSinceLastCustomer = 0.0f;
					gameLoop->currentCustomerIndex++;
					CustomerLogic* nextCustomer = gameLoop->customers[gameLoop->currentCustomerIndex];
					nextCustomer->StartWalking();
				}
				else
				{
					std::cout << "Not spawning next customer - someone is already at the counter!" << std::endl;
				}
			}
		}
	}

	if (distance <= 0.1f)
	{
		rigidbody->velocity = Vec2(0.0f, 0.0f);

		if (!isOrderTaken && currentTargetIndex < queueTargets.size() - 1)
		{
			std::cout << "Customer walking to queue.\n";

			currentTargetIndex++;
			currentQueueTarget = queueTargets[currentTargetIndex];
		}
		else if (!isOrderTaken && currentTargetIndex == queueTargets.size() - 1)
		{
			std::cout << "Customer waiting to order.\n";

			owner->stateMachine.ChangeState(owner->WaitingOrderState);

			if (customerAnimator)
			{
				customerAnimator->PlayAnimation("BACK_IDLE");
			}
			return;
		}
		else if (isOrderTaken && !orderCollected && currentTargetIndex < waitTargets.size() - 1)
		{
			std::cout << "Customer order taken. Moving in to dining area.";

			currentTargetIndex++;
			currentQueueTarget = waitTargets[currentTargetIndex];
		}
		else if (isOrderTaken && !orderCollected && isWalkingToSeat) 
		{
			if (customerAnimator)
			{
				// WAIT FOR ORDER IN SEAT
				customerAnimator->PlayAnimation("BACK_IDLE");
				isWaitingToCollectOrder = true;
			}
		}
		else if (isOrderTaken && !orderCollected && currentTargetIndex == waitTargets.size() - 1)
		{
			GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
			bool foundSeat = false;

			if (gameLoop)
			{
				for (auto& pairSeat : gameLoop->seatingLocations)
				{
					if (!pairSeat.second)
					{
						currentQueueTarget = pairSeat.first;
						isWalkingToSeat = true;

						pairSeat.second = true;
						customerSeatEntity = pairSeat.first;

						foundSeat = true;
						std::cout << "Customer assigned to seat: " << pairSeat.first->name << "\n";
						break;
					}
				}
			}

			if (foundSeat)
			{
				std::cout << "Customer found a seat. Moving now.\n";
			}
			else
			{
				std::cout << "No available seats found!\n";
			}
		}

		// 3) If the order is collected, move through leaveTargets
		else if (isOrderTaken && orderCollected && currentTargetIndex < leaveTargets.size() - 1)
		{
			// not sure if needed, not removing cause dont want to find out
			if (isWaitingToCollectOrder) 
			{
				currentTargetIndex = 0;
				isWaitingToCollectOrder = false;
			}

			std::cout << "Customer is now leaving.\n";

			currentTargetIndex++;
			currentQueueTarget = leaveTargets[currentTargetIndex];
		}

		else if (isOrderTaken && orderCollected && currentTargetIndex == leaveTargets.size() - 1)
		{
			std::cout << "Customer has left.\n";

		}

		else
		{
			std::cout << "ERROR, should never be here\n";
			if (customerAnimator)
			{
				customerAnimator->PlayAnimation("BACK_IDLE");
			}
		}
	}
	else
	{
		//std::cout << "walking...\n";

		// We're still walking
		rigidbody->velocity = direction * moveSpeed;

		SoundComponent* Footstep = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CustomerFootstepSFX").get()->entityID);

		// Play walk animations
		if (customerAnimator)
		{
			if (std::abs(direction.x) > std::abs(direction.y))
			{
				if (direction.x > 0)
				{
					customerAnimator->PlayAnimation("RIGHT_WALK");

				}
				else
				{
					customerAnimator->PlayAnimation("LEFT_WALK");
				}
			}
			else
			{
				if (direction.y > 0)
				{
					customerAnimator->PlayAnimation("BACK_WALK");
				}
				else
				{
					customerAnimator->PlayAnimation("FRONT_WALK");
				}
			}

			Footstep->Play();
		}
	}
}

// Exit: Called when leaving this state
void CustomerWalkState::Exit()
{
	std::cout << "Customer exits Walk State" << std::endl;

}