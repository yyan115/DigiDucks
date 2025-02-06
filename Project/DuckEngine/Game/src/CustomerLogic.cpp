#include "CustomerLogic.h"
#include "GameScene.h"

void CustomerLogic::SetOrder(ItemType order)
{
	customerOrderType = order;

	if (order == ItemType::CHEESE_BURGER_PLATE)
	{
		customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("speech_buncheesepattyplate");
	}
	else if (order == ItemType::SALAD_PLATE)
	{
		customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("speech_lettucetomatoshrimp");

	}
	else if (order == ItemType::LETTUCE_PLATE)
	{
		customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("speech_lettuce");

	}
}

void CustomerLogic::Start()
{
	IdleState = std::make_shared<CustomerIdleState>(this);
	WalkState = std::make_shared<CustomerWalkState>(this);
	WaitingOrderState = std::make_shared<CustomerWaitingOrderState>(this);
	stateMachine.ChangeState(IdleState);

	customerOrder = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Customer1_Order").get();
	customerOrderSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(customerOrder->entityID);

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
