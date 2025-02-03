#include "CustomerLogic.h"

void CustomerLogic::Start()
{
	queueTarget = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QueueUpSpot").get();
	if (queueTarget == nullptr)
	{
		std::cout << "QueueUpSpot entity not found!" << std::endl;
	}
}

void CustomerLogic::Update()
{
	


}

void CustomerLogic::FixedUpdate()
{
	if (queueTarget == nullptr)
	{
		return;
	}

	TransformComponent* customerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	TransformComponent* targetTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(queueTarget->entityID);

	if (customerTransform == nullptr || targetTransform == nullptr)
	{
		return;
	}

	Vec2 direction = targetTransform->GetPosition() - customerTransform->GetPosition();

	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance > 0.0f)
	{
		direction.x /= distance;
		direction.y /= distance;
	}

	float moveSpeed = 2.0f; 
	RigidbodyComponent* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(component->GetEntityID());
	if (rigidbody)
	{
		if (distance > 0.1f)
		{
			rigidbody->velocity = direction * moveSpeed;
		}
		else
		{
			rigidbody->velocity = Vec2(0.0f, 0.0f);
			std::cout << "Reached QueueUpSpot!" << std::endl;
		}
	}
}
