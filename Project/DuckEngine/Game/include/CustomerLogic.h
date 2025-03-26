/******************************************************************************/
/*!
\file       CustomerLogic.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the CustomerLogic class, which defines the behavior of
			customers within the game. The class utilizes a state machine
			to handle customer actions, including idling, walking, and
			waiting for an order. It also interacts with GameLoopLogic and
			manages customer orders and order sprites.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include "DuckEngine.h"
#include "StateMachine.h"
#include "GameLogicComponent.h"

#include "CustomerIdleState.h"
#include "CustomerWalkState.h"
#include "CustomerWaitingOrderState.h"

#include "IngredientType.h"

class GameLoopLogic;
class CustomerLogic : public GameLogic
{
public:
	StateMachine<CustomerLogic> stateMachine;
	std::shared_ptr<CustomerIdleState> IdleState;
	std::shared_ptr<CustomerWalkState> WalkState;
	std::shared_ptr<CustomerWaitingOrderState> WaitingOrderState;
	bool isWaitingToGiveOrder = false;

	CustomerLogic()
		: IdleState(nullptr), WalkState(nullptr) {
	}

	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<CustomerLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	GameLoopLogic* GetGameLoopLogic();
	void OrderCompleted();
	void StartWalking();


	void SetOrder(ItemType order);
	SpriteRendererComponent* GetCustomerOrderSpriteRenderer() { return customerOrderSpriteRenderer; }
	ItemType GetCustomerOrderType() { return customerOrderType; }


	// patience stuff
	RadialSliderComponent* WaitingSlider = nullptr;

	// when waiting for order to be taken
	float CurrentWaitingTime = 0;
	float MaxCashierWaitingTime = 10;


private:
	Entity* customerOrder = nullptr;
	SpriteRendererComponent* customerOrderSpriteRenderer = nullptr;
	ItemType customerOrderType = ItemType::EMPTY;


};
