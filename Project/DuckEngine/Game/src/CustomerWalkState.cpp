#include <iostream>
#include "CustomerWalkState.h"
#include "CustomerLogic.h"
#include "GameManager.h"

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

	customerAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(owner->GetComponentID());


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

	float moveSpeed = 5.0f;

	RigidbodyComponent* rigidbody =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(owner->GetComponentID());

	if (!rigidbody)
	{
		return;
	}

	if (currentQueueTarget == finalPath && distance <= 0.1f)
	{
		owner->stateMachine.ChangeState(owner->IdleState);

		if (owner->GetGameScene()->currentCustomerIndex < owner->GetGameScene()->customers.size() - 1)
		{
			std::cout << "NEXT CUSTOMER" << std::endl;

			owner->GetGameScene()->currentCustomerIndex++;
			CustomerLogic* nextCustomer = owner->GetGameScene()->customers[owner->GetGameScene()->currentCustomerIndex];

			int randomDishOrder = DuckEngine::RandomRange(1, 2);
			if (randomDishOrder == 1)
			{
				nextCustomer->SetOrder(ItemType::CHEESE_BURGER_PLATE);
			}
			else
			{
				nextCustomer->SetOrder(ItemType::SALAD_PLATE);
			}
			customerEnded = true;

		}
		else
		{
			std::cout << "All customers served. Transitioning to EndScene." << std::endl;
			GameManager::SetActiveScene("EndScene");
			customerEnded = true;

		}
	}


	if (distance > 0.1f)
	{
		rigidbody->velocity = direction * moveSpeed;
		SoundComponent* Footstep = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CustomerFootstepSFX").get()->entityID);
		
		if (customerAnimator) 
		{
			if (std::abs(direction.x) > std::abs(direction.y)) 
			{
				if (direction.x > 0)
				{
					customerAnimator->PlayAnimation("RIGHT_WALK");
					Footstep->Play();
				}
				else
				{
					customerAnimator->PlayAnimation("LEFT_WALK");
					Footstep->Play();
				}
			}
			else 
			{
				if (direction.y > 0)
				{
					customerAnimator->PlayAnimation("BACK_WALK");
					Footstep->Play();
				}
				else
				{
					customerAnimator->PlayAnimation("FRONT_WALK");
					Footstep->Play();
				}
			}
		}
	}
	else
	{
		rigidbody->velocity = Vec2(0.0f, 0.0f);

		if (customerAnimator)
		{
			if (std::abs(direction.x) > std::abs(direction.y)) 
			{
				if (direction.x > 0)
				{
					customerAnimator->PlayAnimation("RIGHT_IDLE");
				}
				else
				{
					customerAnimator->PlayAnimation("LEFT_IDLE");
				}
			}
			else
			{
				if (direction.y > 0)
				{
					customerAnimator->PlayAnimation("BACK_IDLE");
				}
				else
				{
					customerAnimator->PlayAnimation("FRONT_IDLE");
				}
			}
		}

		if (!isOrderTaken && currentTargetIndex == 0)
		{
			customerAnimator->PlayAnimation("BACK_IDLE");
			owner->stateMachine.ChangeState(owner->WaitingOrderState);
			return;
		}

		else if (isOrderTaken && orderCollected && !customerEnded)
		{
			if (currentQueueTarget != finalPath)
			{
				currentQueueTarget = finalPath;
			}
			else
			{
				
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
				isWaitingToCollectOrder = true;
				customerAnimator->PlayAnimation("RIGHT_IDLE");
			}
		}
	}
}


void CustomerWalkState::Exit()
{
	std::cout << "Customer exits Walk State" << std::endl;
}