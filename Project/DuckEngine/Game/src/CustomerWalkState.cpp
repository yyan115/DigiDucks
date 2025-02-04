#include <iostream>
#include "CustomerWalkState.h"
#include "CustomerLogic.h"

CustomerWalkState::CustomerWalkState(CustomerLogic* customerLogicOwner)
	: State<CustomerLogic>(customerLogicOwner), queueTarget(nullptr) {}

void CustomerWalkState::Initialize(CustomerLogic* customerLogicOwner)
{
	owner = customerLogicOwner;
}

void CustomerWalkState::Enter()
{
	std::cout << "Customer enters Walk State" << std::endl;

}


void CustomerWalkState::Update()
{

}

void CustomerWalkState::FixedUpdate()
{
	queueTarget = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QueueUpSpot").get();
	TransformComponent* customerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->GetComponentID());
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
	RigidbodyComponent* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(owner->GetComponentID());
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
			owner->stateMachine.ChangeState(owner->IdleState);
		}
	}
}

void CustomerWalkState::Exit()
{
	std::cout << "Customer exits Walk State" << std::endl;
}