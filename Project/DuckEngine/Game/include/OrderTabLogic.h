/******************************************************************************/
/*!
\file       OrderTabLogic.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Declares the OrderTabLogic class and OrderTabData struct, which
			manage the logic and animation for customer order tabs in the game.

			Each order tab represents a customer’s order visually, including
			animation states, linked components (transform/sprite), and the
			corresponding customer. This logic is responsible for adding,
			removing, and updating order tabs dynamically as the game progresses.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include "DuckEngine.h"
#include "IngredientType.h"
#include "State.h"

class CustomerLogic;

struct OrderTabData
{
	Entity* entity = nullptr;
	TransformComponent* transform = nullptr;
	SpriteRendererComponent* spriteRenderer = nullptr;

	float animationTime = 0.0f;
	float elapsedTime = 0.0f;
	bool isAnimating = false;
	Vec2 startPosition;
	Vec2 targetPosition;
	float bounceFactor = 0.2f;

	ItemType tabOrder = ItemType::EMPTY;
	CustomerLogic* tabCustomer = nullptr;

};

class OrderTabLogic : public GameLogic
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	void AddOrder(ItemType order, CustomerLogic* customer);
	void RemoveOrder(CustomerLogic* customer);
	bool HasFreeTab() const;
	int GetUsedTabCount() const;
	int GetMaxTabCount() const;

	const std::vector<OrderTabData>& GetOrderTabs() const { return orderTabs; }


	ItemType GetCurrentOrder()
	{
		return currentOrder;
	}

	CustomerLogic* GetCurrentCustomer()
	{
		return currentCustomer;
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<OrderTabLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

private:
	std::vector<OrderTabData> orderTabs;
	ItemType currentOrder = ItemType::EMPTY;
	CustomerLogic* currentCustomer = nullptr;
};
