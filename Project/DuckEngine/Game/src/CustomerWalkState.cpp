#include <iostream>
#include <algorithm>
#include "CustomerWalkState.h"
#include "CustomerLogic.h"
#include "GameManager.h"

// Constructor
CustomerWalkState::CustomerWalkState(CustomerLogic* customerLogicOwner)
    : State<CustomerLogic>(customerLogicOwner)
    , currentQueueTarget(nullptr)
{
}

// Called when the customer enters the Walk State
void CustomerWalkState::Enter()
{
    std::cout << "Customer enters Walk State" << std::endl;

    // Get all entities
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    // Collect all "QueueUp" entities
    for (auto& entity : entities)
    {
        if (entity->name.find("QueueUpPoint") != std::string::npos)
        {
            queueTargets.push_back(entity.get());
            std::cout << "Added queue target: " << entity->name << "\n";
        }
    }

    // Collect all "Wait" entities
    for (auto& entity : entities)
    {
        if (entity->name.find("WaitPoint") != std::string::npos)
        {
            waitTargets.push_back(entity.get());
            std::cout << "Added wait target: " << entity->name << "\n";
        }
    }

    // Collect all "Leave" entities
    for (auto& entity : entities)
    {
        if (entity->name.find("LeavePoint") != std::string::npos)
        {
            leaveTargets.push_back(entity.get());
            std::cout << "Added leave target: " << entity->name << "\n";
        }
    }

    // Sort them based on the suffix number (e.g., QueueUp_1, Wait_2, etc.)
    auto sortEntities = [](Entity* a, Entity* b) {
        int numA = std::stoi(a->name.substr(a->name.find_last_of('_') + 1));
        int numB = std::stoi(b->name.substr(b->name.find_last_of('_') + 1));
        return numA < numB;
        };

    std::sort(queueTargets.begin(), queueTargets.end(), sortEntities);
    std::sort(waitTargets.begin(), waitTargets.end(), sortEntities);
    std::sort(leaveTargets.begin(), leaveTargets.end(), sortEntities);

    // Grab animator
    customerAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(owner->GetComponentID());

    // The logic says: Start by walking through the queue
    currentTargetIndex = 0;
    isOrderTaken = owner->WaitingOrderState->GetIsOrderTaken();

    if (!queueTargets.empty())
    {
        currentQueueTarget = queueTargets[currentTargetIndex];  // Start with queue
    }
    else
    {
        std::cerr << "No valid queue targets found!" << std::endl;
    }

    if (isOrderTaken && orderCollected) {
        currentQueueTarget = leaveTargets[0];
    }

    std::cout << "Queue size = " << queueTargets.size()
        << ", Wait size = " << waitTargets.size()
        << ", Leave size = " << leaveTargets.size() << std::endl;
}

// Update (not used, but kept for completeness)
void CustomerWalkState::Update()
{
}

// FixedUpdate: Movement, transitions, animations
void CustomerWalkState::FixedUpdate()
{
    TransformComponent* customerTransform =
        DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->GetComponentID());

    if (!currentQueueTarget || !customerTransform)
    {
        return;
    }

    // Get the transform of the current target
    TransformComponent* targetTransform =
        DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(currentQueueTarget->entityID);
    if (!targetTransform)
    {
        return;
    }

    // Calculate direction & distance
    Vec2 direction = targetTransform->GetPosition() - customerTransform->GetPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.0f)
    {
        direction.x /= distance;
        direction.y /= distance;
    }

    // Movement speed
    float moveSpeed = 5.0f;

    // Get rigidbody
    RigidbodyComponent* rigidbody =
        DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(owner->GetComponentID());
    if (!rigidbody)
    {
        return;
    }

    if (currentQueueTarget == leaveTargets.back() && distance <= 0.1f)
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




    // If close enough to the current target, we do transitions
    if (distance <= 0.1f)
    {
        // Stop moving
        rigidbody->velocity = Vec2(0.0f, 0.0f);

        // 1) Walking the queue
        if (!isOrderTaken && currentTargetIndex < queueTargets.size() - 1)
        {
            std::cout << "walking to queue\n";

            // Go to the next queue point
            currentTargetIndex++;
            currentQueueTarget = queueTargets[currentTargetIndex];
        }
        else if (!isOrderTaken && currentTargetIndex == queueTargets.size() - 1)
        {
            std::cout << "waiting for order\n";

            // Arrived at the last queue point, go to WaitingOrderState
            owner->stateMachine.ChangeState(owner->WaitingOrderState);

            // Show idle
            if (customerAnimator)
            {
                customerAnimator->PlayAnimation("BACK_IDLE");
            }
            return;
        }
        // 2) Once the order is taken, move through waitTargets
        else if (isOrderTaken && !orderCollected && currentTargetIndex < waitTargets.size() - 1)
        {
            std::cout << "order taken, go move to wait\n";

            currentTargetIndex++;
            currentQueueTarget = waitTargets[currentTargetIndex];
        }
        else if (isOrderTaken && !orderCollected && currentTargetIndex == waitTargets.size() - 1)
        {
            // At last wait point, do idle
            if (customerAnimator)
            {
                customerAnimator->PlayAnimation("BACK_IDLE");
                isWaitingToCollectOrder = true;
            }
            std::cout << "Waiting at last wait point for order to be collected...\n";
        }
        // 3) If the order is collected, move through leaveTargets
        else if (isOrderTaken && orderCollected && currentTargetIndex < leaveTargets.size() - 1)
        {
            if (isWaitingToCollectOrder) {
                currentTargetIndex = 0;
                isWaitingToCollectOrder = false;
            }

            std::cout << "index: " << currentTargetIndex << "\n";
            
            currentTargetIndex++;
            currentQueueTarget = leaveTargets[currentTargetIndex];
        }
        else if (isOrderTaken && orderCollected && currentTargetIndex == leaveTargets.size() - 1)
        {
            std::cout << "left\n";

            // Reached final leave point
            owner->stateMachine.ChangeState(owner->IdleState);
        }
        // If we haven't accounted for all conditions, do an idle as fallback
        else
        {
            std::cout << "ERROR, should never be here\n";
            if (customerAnimator)
            {
                customerAnimator->PlayAnimation("BACK_IDLE");
            }
        }
    }
    else
    {
        std::cout << "walking...\n";

        // We're still walking
        rigidbody->velocity = direction * moveSpeed;

        SoundComponent* Footstep = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CustomerFootstepSFX").get()->entityID);

        // Play walk animations
        if (customerAnimator)
        {
            if (std::abs(direction.x) > std::abs(direction.y))
            {
                if (direction.x > 0)
                {
                    customerAnimator->PlayAnimation("RIGHT_WALK");

                }
                else
                {
                    customerAnimator->PlayAnimation("LEFT_WALK");
                }
            }
            else
            {
                if (direction.y > 0)
                {
                    customerAnimator->PlayAnimation("BACK_WALK");
                }
                else
                {
                    customerAnimator->PlayAnimation("FRONT_WALK");
                }
            }

            Footstep->Play();
        }
    }
}

// Exit: Called when leaving this state
void CustomerWalkState::Exit()
{
    std::cout << "Customer exits Walk State" << std::endl;
}
