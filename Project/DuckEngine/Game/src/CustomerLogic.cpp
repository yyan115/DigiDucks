#include "CustomerLogic.h"
#include "GameScene.h"

void CustomerLogic::Start()
{
	IdleState = std::make_shared<CustomerIdleState>(this);
	WalkState = std::make_shared<CustomerWalkState>(this);
	stateMachine.ChangeState(WalkState);

	gameScene = DuckEngine::DUCKENGINE_SceneManager.GetScene<GameScene>("GameScene").get();
}

void CustomerLogic::Update()
{
	// if game not started yet return
	if (!gameScene->IsGameStarted()) return;
	stateMachine.currentState->Update();

}

void CustomerLogic::FixedUpdate()
{
	// if game not started yet return
	if (!gameScene->IsGameStarted()) return;
	stateMachine.currentState->FixedUpdate();
}
