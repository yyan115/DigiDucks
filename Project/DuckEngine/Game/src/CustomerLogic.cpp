/******************************************************************************/
/*!
\file       CustomerLogic.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Implements the CustomerLogic class, which controls customer
			behavior through a finite state machine. The class manages
			state transitions for customer movement, waiting for orders,
			and order completion. It also updates order-related sprites
			and interacts with the game’s logic systems.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CustomerLogic.h"
#include "GameLogicManager.h"
#include "GameLoopLogic.h"

GameLoopLogic* gameLoopLogic = nullptr;

void CustomerLogic::SetOrder(ItemType order)
{
	customerOrderType = order;

	customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("speech_" + whatType(order));
}

void CustomerLogic::StartWalking()
{
	stateMachine.ChangeState(WalkState);
}

void CustomerLogic::Start()
{
	IdleState = std::make_shared<CustomerIdleState>(this);
	WalkState = std::make_shared<CustomerWalkState>(this);
	WaitingOrderState = std::make_shared<CustomerWaitingOrderState>(this);

	Entity* currentEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
	if (currentEntity->childEntities.size() > 0)
	{
		Entity* customerOrderEntity = currentEntity->childEntities[0].get();
		Entity* radialSliderEntity = currentEntity->childEntities[1].get();

		if (customerOrderEntity)
		{
			customerOrderSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(customerOrderEntity->entityID);
			customerOrderSpriteRenderer->isVisible = false;
		}

		if (radialSliderEntity)
		{
			WaitingSlider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RadialSliderComponent>(radialSliderEntity->entityID);
		}
	}


	Entity* gameLogicManager = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLogicManager->entityID).get();

	stateMachine.ChangeState(IdleState);
}

void CustomerLogic::Update()
{
	if (!gameLoopLogic->IsGameStarted()) return;
	stateMachine.currentState->Update();

}

void CustomerLogic::FixedUpdate()
{
	if (!gameLoopLogic->IsGameStarted()) return;
	stateMachine.currentState->FixedUpdate();
}

void CustomerLogic::OrderCompleted()
{
	WalkState->CustomerOrderCollected();

	GameLoopLogic* gameLoop = GetGameLoopLogic();

	if (inSequence) 
	{
		std::cout << "Sequence mode: Customer order completed, will spawn next after leaving" << std::endl;
	}

	if (gameLoop) 
	{
		gameLoop->isSpawningCustomer = true;
		gameLoop->customerSpawnCooldown = 3.0f;
	}

	// Transition to walk state to leave
	stateMachine.ChangeState(WalkState);
}

GameLoopLogic* CustomerLogic::GetGameLoopLogic()
{
	return gameLoopLogic;
}

float CustomerLogic::GetCustomerMultiplier()
{
	if (CurrentWaitingTime >= MaxTableWaitingTime / 10 * 8)
	{
		return 0.3f; // Angry customer, low reward
	}
	else if (CurrentWaitingTime >= MaxTableWaitingTime / 10 * 5)
	{
		return 0.6f; // Waiting but still okay
	}
	else
	{
		return 1.f; // Fast service, big reward
	}

}

Entity* CustomerLogic::GetCurrentCustomerEntity()
{
	return DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
}

bool CustomerLogic::IsAnyCustomerInSequence(GameLoopLogic* gameLoop)
{
	if (!gameLoop) return false;

	for (auto* customer : gameLoop->customers)
	{
		// Skip customers that haven't been spawned yet or have completed their journey
		if (customer->stateMachine.currentState == customer->IdleState &&
			customer->hasCompletedJourney) {
			continue;
		}

		// If any customer is in sequence mode and active, return true
		if (customer->inSequence &&
			customer->stateMachine.currentState != customer->IdleState) {
			return true;
		}
	}

	return false;
}