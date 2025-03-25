/******************************************************************************/
/*!
\file       CustomerLogic.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Implements the CustomerLogic class, which controls customer
			behavior through a finite state machine. The class manages
			state transitions for customer movement, waiting for orders,
			and order completion. It also updates order-related sprites
			and interacts with the game’s logic systems.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CustomerLogic.h"
#include "GameLogicManager.h"
#include "GameLoopLogic.h"

GameLoopLogic* gameLoopLogic = nullptr;

void CustomerLogic::SetOrder(ItemType order)
{
	customerOrderType = order;

	customerOrderSpriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("speech_" + whatType(order));
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
