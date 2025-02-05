#pragma once

#include "DuckEngine.h"
#include "IngredientType.h"

class OrderTabLogic : public GameLogic
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void AddOrder(ItemType order);
	ItemType GetCurrentOrder() { return currentOrder; }

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

	float animationTime = 0.0f;
	float elapsedTime = 0.0f;   // Time elapsed for animation
	bool isAnimating = false;  // Whether the animation is active
	Vec2 startPosition;        // Start position for animation
	Vec2 targetPosition;       // Target position for animation
	float bounceFactor = 0.2f; // Bounce factor for overshoot
};
