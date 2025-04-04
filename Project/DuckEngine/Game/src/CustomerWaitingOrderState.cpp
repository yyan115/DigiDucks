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
#include "SubmitLogic.h"

SoundComponent* orderComeSFX = nullptr;
SoundComponent* customerSound = nullptr;

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

	if (owner->GetCurrentCustomerEntity())
	{
		customerSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(owner->GetCurrentCustomerEntity()->entityID);
	}
	owner->WaitingSlider->isVisible = true;
	owner->WaitingSlider->maxValue = owner->MaxCashierWaitingTime;
	owner->WaitingSlider->currentValue = owner->WaitingSlider->maxValue;
}


void CustomerWaitingOrderState::Update()
{
	// used to set order sprite to false at the end, because i dont want to refactor code and accidentally introduce more bugs
	bool angryLeft = false;

	if (owner->CurrentWaitingTime <= owner->MaxCashierWaitingTime + 30 * DuckEngine::DeltaTime())
	{
		owner->CurrentWaitingTime += DuckEngine::DeltaTime();

		float sliderValue = owner->MaxCashierWaitingTime - owner->CurrentWaitingTime;
		owner->WaitingSlider->currentValue = (sliderValue < 0.0f) ? 0.0f : sliderValue;

		// change to red if 2/3
		if (owner->CurrentWaitingTime >= owner->MaxCashierWaitingTime / 3 * 2) {
			customerSound->Play(0);
			owner->WaitingSlider->fillColor = { 255.f, 0.f, 0, 255.f };
		}
		// change to yellow if 1/3
		else if (owner->CurrentWaitingTime >= owner->MaxCashierWaitingTime / 3) {
			owner->WaitingSlider->fillColor = { 255.f, 183.f, 0, 255.f };
		}
	}
	// ran out of patience
	else {
		//owner->GetGameLoopLogic()->customerAngryLeave = true;
		owner->WalkState.get()->customerAngryLeave = true;
		customerSound->Stop();	
		customerSound->Play(1);
		//owner->GetGameLoopLogic()->customerCount--;
		
		// Make sure the game loop knows there's no customer waiting for order anymore
		GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
		if (gameLoop) 
		{
			gameLoop->isCustomerWaitingForOrder = false;
			
			// Set the spawn cooldown to prevent immediate spawning
			gameLoop->isSpawningCustomer = true;
			gameLoop->customerSpawnCooldown = 5.0f;  
			gameLoop->timeSinceLastCustomer = 0.0f;  
		}
		
		owner->stateMachine.ChangeState(owner->WalkState);

		// LOGIC TO DECREASE SCORE BY 10
		Entity* submit = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Submit_Station").get();
		if (submit)
		{
			auto submitLogic = GameLogicManager::GetLogicForEntity<SubmitLogic>(submit->entityID);

			submitLogic->decreaseScore(10);
		}

		angryLeft = true;
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

		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_J) || DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
		{
			owner->GetCustomerOrderSpriteRenderer()->isVisible = true;
			customerSound->Stop();
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
			owner->GetGameLoopLogic()->ordersTaken++;

			GameLoopLogic* gameLoop = owner->GetGameLoopLogic();
			if (gameLoop)
			{
				gameLoop->isCustomerWaitingForOrder = false;
				gameLoop->currentActiveOrders++;

				gameLoop->isSpawningCustomer = true;
				gameLoop->customerSpawnCooldown = 6.0f;  // Longer cooldown
				gameLoop->timeSinceLastCustomer = 0.0f;

				gameLoop->ResetCustomerQueue();
			}

			owner->stateMachine.ChangeState(owner->WalkState);
			owner->GetCustomerOrderSpriteRenderer()->isVisible = false;
		}
	}
	else
	{
		owner->GetCustomerOrderSpriteRenderer()->isVisible = false;
	}

	if (angryLeft) {
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