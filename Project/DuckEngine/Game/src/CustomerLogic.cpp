#include "CustomerLogic.h"
#include "GameScene.h"
#include "GameLogicManager.h"
#include "GameLoopLogic.h"

GameLoopLogic* gameLoopLogic = nullptr;

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

void CustomerLogic::StartWalking()
{
	stateMachine.ChangeState(WalkState);
}

void CustomerLogic::Start()
{
	IdleState = std::make_shared<CustomerIdleState>(this);
	WalkState = std::make_shared<CustomerWalkState>(this);
	WaitingOrderState = std::make_shared<CustomerWaitingOrderState>(this);

	Entity* currentEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
	Entity* childEntity = currentEntity->childEntities[0].get();

	if (childEntity)
	{
		customerOrderSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(childEntity->entityID);
		customerOrderSpriteRenderer->isVisible = false;
	}

	Entity* gameLogicManager = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("GameLoopManager").get();
	gameLoopLogic = GameLogicManager::GetLogicForEntity<GameLoopLogic>(gameLogicManager->entityID).get();

	stateMachine.ChangeState(IdleState);
}

void CustomerLogic::Update()
{
	if (!gameLoopLogic->IsGameStarted()) return;
	stateMachine.currentState->Update();

}

void CustomerLogic::FixedUpdate()
{
	if (!gameLoopLogic->IsGameStarted()) return;
	stateMachine.currentState->FixedUpdate();
}

void CustomerLogic::OrderCompleted()
{
	WalkState->CustomerOrderCollected();
	stateMachine.ChangeState(WalkState);
}

GameLoopLogic* CustomerLogic::GetGameLoopLogic()
{
	return gameLoopLogic;
}