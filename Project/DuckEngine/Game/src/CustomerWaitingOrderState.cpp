#include "CustomerWaitingOrderState.h"
#include "CustomerLogic.h"
#include "OrderTabLogic.h"
#include "DuckEngine_Input.h"

CustomerWaitingOrderState::CustomerWaitingOrderState(CustomerLogic* customerLogicOwner)
	: State<CustomerLogic>(customerLogicOwner) {}

void CustomerWaitingOrderState::Enter()
{
	std::cout << "Customer enters Waiting Order State" << std::endl;
}


void CustomerWaitingOrderState::Update()
{
	TransformComponent* customerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(owner->GetComponentID());

	Entity* playerEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player").get();
	TransformComponent* playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(playerEntity->entityID);


	Vec2 customerPosition = customerTransform->GetPosition();
	Vec2 playerPosition = playerTransform->GetPosition();
	float distance = std::sqrt(
		(playerPosition.x - customerPosition.x) * (playerPosition.x - customerPosition.x) +
		(playerPosition.y - customerPosition.y) * (playerPosition.y - customerPosition.y)
	);

	float proximityThreshold = 4.2f;
	if (distance < proximityThreshold && !orderTaken)
	{
		owner->GetCustomerOrderSpriteRenderer()->isVisible = true;

		Entity* orderTab = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Order_Tab").get();
		OrderTabLogic* orderTabLogic = GameLogicManager::GetLogicForEntity<OrderTabLogic>(orderTab->entityID).get();

		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_J))
		{
			orderTabLogic->AddOrder(ItemType::CHEESE_BURGER_PLATE);
			orderTaken = true;
		}
	}
	else
	{
		owner->GetCustomerOrderSpriteRenderer()->isVisible = false;
	}
}
void CustomerWaitingOrderState::FixedUpdate()
{

}

void CustomerWaitingOrderState::Exit()
{
	std::cout << "Customer exits Waiting Order State" << std::endl;
}