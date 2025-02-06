#pragma once

#include "DuckEngine.h"
#include "StateMachine.h"
#include "GameLogicComponent.h"

#include "CustomerIdleState.h"
#include "CustomerWalkState.h"
#include "CustomerWaitingOrderState.h"

#include "IngredientType.h"


class GameScene;

class CustomerLogic : public GameLogic
{
public:
	StateMachine<CustomerLogic> stateMachine;
	std::shared_ptr<CustomerIdleState> IdleState;
	std::shared_ptr<CustomerWalkState> WalkState;
	std::shared_ptr<CustomerWaitingOrderState> WaitingOrderState;

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

	void SetOrder(ItemType order);
	SpriteRendererComponent* GetCustomerOrderSpriteRenderer() { return customerOrderSpriteRenderer; }
	GameScene* GetGameScene() { return gameScene; }
	ItemType GetCustomerOrderType() { return customerOrderType; }
	void OrderCompleted();

private:
	GameScene* gameScene = nullptr;
	Entity* customerOrder = nullptr;
	SpriteRendererComponent* customerOrderSpriteRenderer = nullptr;
	ItemType customerOrderType = ItemType::EMPTY;

};
