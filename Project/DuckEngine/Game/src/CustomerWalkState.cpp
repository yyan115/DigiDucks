#include <iostream>
#include "CustomerWalkState.h"
#include "CustomerLogic.h"

CustomerWalkState::CustomerWalkState(CustomerLogic* customerLogicOwner)
	: State<CustomerLogic>(customerLogicOwner), currentQueueTarget(nullptr) {}

void CustomerWalkState::Enter()
{
	std::cout << "Customer enters Walk State" << std::endl;

	if (queueTargets.empty())
	{
		queueTargets.push_back(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QueueUp_1").get());
		queueTargets.push_back(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QueueUp_2").get());
		queueTargets.push_back(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QueueUp_3").get());
		queueTargets.push_back(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("QueueUp_4").get());
	}

	if (finalPath == nullptr)
	{
		finalPath = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FinalPath").get();
	}


	isOrderTaken = owner->WaitingOrderState->GetIsOrderTaken();

	currentTargetIndex = 0;
	currentQueueTarget = queueTargets[currentTargetIndex];

	if (isOrderTaken && orderCollected)
	{
		currentQueueTarget = finalPath;
	}

	std::cout << "queue size = " << queueTargets.size() << std::endl;
}


void CustomerWalkState::Update()
{

}

void CustomerWalkState::FixedUpdate()
{
	TransformComponent* customerTransform =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->GetComponentID());

	// Always ensure our current target is valid before we proceed
	if (!currentQueueTarget || !customerTransform)
	{
		return;
	}

	TransformComponent* targetTransform =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(currentQueueTarget->entityID);

	if (!targetTransform)
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

	float moveSpeed = 3.0f;

	RigidbodyComponent* rigidbody =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(owner->GetComponentID());

	if (!rigidbody)
	{
		return;
	}

	if (distance > 0.1f)
	{
		rigidbody->velocity = direction * moveSpeed;
	}
	else
	{
		rigidbody->velocity = Vec2(0.0f, 0.0f);

		if (!isOrderTaken && currentTargetIndex == 0)
		{
			owner->stateMachine.ChangeState(owner->WaitingOrderState);
			return;
		}

		else if (isOrderTaken && orderCollected)
		{
			if (currentQueueTarget != finalPath)
			{
				currentQueueTarget = finalPath;
			}
			else
			{
				// destroy customer do later
			}
		}

		else if (isOrderTaken)
		{
			if (currentTargetIndex < queueTargets.size() - 1)
			{
				currentTargetIndex++;
				currentQueueTarget = queueTargets[currentTargetIndex];
			}
			else
			{
				// final queue target reached
				isWaitingToCollectOrder = true;
			}
		}
	}
}



void CustomerWalkState::Exit()
{
	std::cout << "Customer exits Walk State" << std::endl;
}