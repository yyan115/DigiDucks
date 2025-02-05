#include "CustomerLogic.h"
#include "GameScene.h"

void CustomerLogic::Start()
{
	IdleState = std::make_shared<CustomerIdleState>(this);
	WalkState = std::make_shared<CustomerWalkState>(this);
	WaitingOrderState = std::make_shared<CustomerWaitingOrderState>(this);
	stateMachine.ChangeState(IdleState);

	customerOrder = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Customer1_Order").get();
	customerOrderSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(customerOrder->entityID);

	int randomDishOrder = DuckEngine::RandomRange(1, 2);

	// 1 is Hamburger
	// 2 is Salad
	if (randomDishOrder == 1)
	{

		customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_1");
		customerOrderType = ItemType::CHEESE_BURGER_PLATE;
	}
	else
	{
		customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_2");
		customerOrderType = ItemType::SALAD_PLATE;
	}



	gameScene = DuckEngine::DUCKENGINE_SceneManager.GetScene<GameScene>("GameScene").get();
}

void CustomerLogic::Update()
{
	stateMachine.currentState->Update();

}

void CustomerLogic::FixedUpdate()
{
	if (!gameScene->IsGameStarted()) return;
	stateMachine.currentState->FixedUpdate();
}

void CustomerLogic::OrderCompleted()
{
	WalkState->CustomerOrderCollected();
	stateMachine.ChangeState(WalkState);
}
