#pragma once

#include "DuckEngine.h"
#include "IngredientType.h"

class CustomerLogic;

class OrderTabLogic : public GameLogic
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void AddOrder(ItemType order, CustomerLogic* customer);
	ItemType GetCurrentOrder() { return currentOrder; }

	CustomerLogic* GetCurrentCustomer() { return currentCustomer; }

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<OrderTabLogic>(*this);
		clone->component = nullptr;
		return clone;
	}
private:
	Entity* orderTabEntity = nullptr;
	TransformComponent* orderTabTransform = nullptr;
	SpriteRendererComponent* orderTabSpriteRenderer = nullptr;
	ItemType currentOrder = ItemType::EMPTY;

	CustomerLogic* currentCustomer = nullptr;

	float animationTime = 0.0f;
	float elapsedTime = 0.0f;   // Time elapsed for animation
	bool isAnimating = false;  // Whether the animation is active
	Vec2 startPosition;        // Start position for animation
	Vec2 targetPosition;       // Target position for animation
	float bounceFactor = 0.2f; // Bounce factor for overshoot
};
