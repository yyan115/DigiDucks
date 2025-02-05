#include <cmath>
#include "OrderTabLogic.h"

void OrderTabLogic::Start()
{
	orderTabEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tab").get();
	orderTabTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(orderTabEntity->entityID);
	orderTabSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(orderTabEntity->entityID);

	orderTabSpriteRenderer->isVisible = false;
}

void OrderTabLogic::Update()
{
	if (isAnimating)
	{
		elapsedTime += DuckEngine::DeltaTime();

		float t = elapsedTime / animationTime;
		if (t > 1.0f)
		{
			t = 1.0f;
		}

		float bounce = std::sin(t * 3.14159f) * bounceFactor * (1.0f - t);
		float interpolatedY = (1 - t) * 0.25f + t * targetPosition.y + bounce;

		orderTabTransform->SetPosition(Vec2(startPosition.x, interpolatedY));

		if (t >= 1.0f)
		{
			isAnimating = false;
			std::cout << "AddOrder animation completed." << std::endl;
		}
	}
}


void OrderTabLogic::FixedUpdate()
{

}

void OrderTabLogic::AddOrder(ItemType order)
{
	if (!orderTabTransform)
	{
		std::cerr << "Order tab transform is null!" << std::endl;
		return;
	}

	if (order == ItemType::CHEESE_BURGER_PLATE)
	{
		orderTabSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_1");
	}
	else
	{
		orderTabSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_2");
	}

	currentOrder = order;

	orderTabSpriteRenderer->isVisible = true;

	startPosition = orderTabTransform->GetPosition();
	targetPosition = Vec2(startPosition.x, -0.15f);
	orderTabTransform->SetPosition(Vec2(startPosition.x, 0.25f));

	animationTime = 1.0f;
	elapsedTime = 0.0f;
	isAnimating = true;

	std::cout << "AddOrder animation started." << std::endl;
}