#include "HoldingLogic.h"
#include <iostream>

void HoldingLogic::Start()
{
	// Initialize the components from the GameLogicComponent's owner entity
	transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());
	if (!holder)
	{
		holder = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	}
	holderTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(holder->entityID);
	//holderLogic
}


void HoldingLogic::Update()
{
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_R))
	{

	}
}

void HoldingLogic::FixedUpdate()
{
	transform->SetPosition(holderTransform->GetPosition() + offSet);
}