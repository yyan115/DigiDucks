/******************************************************************************/
/*!
\file       CustomerWaitingOrderState.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Implements the CustomerWaitingOrderState class, managing the
			logic for customers waiting to place an order. This includes
			detecting player proximity, playing sound effects, interacting
			with the order tab system, and transitioning the customer to the
			next state once the order is taken.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CustomerWaitingOrderState.h"
#include "CustomerLogic.h"
#include "OrderTabLogic.h"
#include "DuckEngine_Input.h"
#include "GameLoopLogic.h"

SoundComponent* orderComeSFX = nullptr;

CustomerWaitingOrderState::CustomerWaitingOrderState(CustomerLogic* customerLogicOwner)
	: State<CustomerLogic>(customerLogicOwner) {}

void CustomerWaitingOrderState::Enter()
{
	std::cout << "Customer enters Waiting Order State" << std::endl;
	owner->isWaitingToGiveOrder = true;

	GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
	if (gameLoop) 
	{
		gameLoop->isCustomerWaitingForOrder = true;
	}

	Entity* SFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("OrderComeSFX").get();
	if (SFX)
	{
		orderComeSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(SFX->entityID);
	}

	orderComeSFX->Play(-1);

	owner->WaitingSlider->isVisible = true;
	owner->WaitingSlider->maxValue = owner->MaxCashierWaitingTime;
	owner->WaitingSlider->currentValue = owner->WaitingSlider->maxValue;
}


void CustomerWaitingOrderState::Update()
{
	if (owner->CurrentWaitingTime <= owner->MaxCashierWaitingTime)
	{
		owner->CurrentWaitingTime += DuckEngine::DeltaTime();
		owner->WaitingSlider->currentValue = owner->MaxCashierWaitingTime - owner->CurrentWaitingTime;
	}

	TransformComponent* customerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->GetComponentID());

	Entity* playerEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
	TransformComponent* playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(playerEntity->entityID);

	Vec2 customerPosition = customerTransform->GetPosition();
	Vec2 playerPosition = playerTransform->GetPosition();
	float distance = std::sqrt(
		(playerPosition.x - customerPosition.x) * (playerPosition.x - customerPosition.x) +
		(playerPosition.y - customerPosition.y) * (playerPosition.y - customerPosition.y)
	);

	float proximityThreshold = 5.1f;
	if (distance < proximityThreshold && !orderTaken)
	{
		owner->GetCustomerOrderSpriteRenderer()->isVisible = true;

		Entity* orderTab = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tabs").get();
		OrderTabLogic* orderTabLogic = GameLogicManager::GetLogicForEntity<OrderTabLogic>(orderTab->entityID).get();

		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_J))
		{
			owner->GetCustomerOrderSpriteRenderer()->isVisible = true;

			if (!orderTabLogic->HasFreeTab())
			{
				std::cout << "No free Order Tabs! Max order limit reached.\n";
				return;
			}

			SoundComponent* quackSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QuackSound").get()->entityID);
			quackSound->Play(-1);
			orderTabLogic->AddOrder(owner->GetCustomerOrderType(), owner);
			orderTaken = true;
			owner->isWaitingToGiveOrder = false;
			owner->stateMachine.ChangeState(owner->WalkState);
			owner->GetCustomerOrderSpriteRenderer()->isVisible = false;

			GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
			if (gameLoop)
			{
				gameLoop->isCustomerWaitingForOrder = false;
				gameLoop->currentActiveOrders++;
			}


		}
	}
	else
	{
		owner->GetCustomerOrderSpriteRenderer()->isVisible = false;
	}
}
void CustomerWaitingOrderState::FixedUpdate()
{

}

void CustomerWaitingOrderState::Exit()
{
	std::cout << "Customer exits Waiting Order State" << std::endl;
	GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
	if (gameLoop) 
	{
		gameLoop->isCustomerWaitingForOrder = false;
	}

	owner->WaitingSlider->isVisible = false;
}