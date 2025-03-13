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
	bool OrderTabLogic::HasFreeTab() const;
	int OrderTabLogic::GetUsedTabCount() const;
	void OrderTabLogic::ReorderTabs();

	int OrderTabLogic::GetMaxTabCount() const;

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
