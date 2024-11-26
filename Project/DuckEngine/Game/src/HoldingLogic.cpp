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

}

void HoldingLogic::FixedUpdate()
{
	transform->SetPosition(holderTransform->GetPosition() + offSet);
}


void HoldingLogic::changeIngredient(IngredientType type) {
	switch (type)
	{
	case IngredientType::EMPTY:
		spriteRenderer->texture = AssetManager::GetTextureByName("Empty");
		break;
	case IngredientType::BUN:
		spriteRenderer->texture = AssetManager::GetTextureByName("bun");
		break;
	case IngredientType::CHEESE:
		spriteRenderer->texture = AssetManager::GetTextureByName("cheese");
		break;
	case IngredientType::LETTUCE:
		spriteRenderer->texture = AssetManager::GetTextureByName("lettuce");
		break;
	case IngredientType::MUSHROOM:
		spriteRenderer->texture = AssetManager::GetTextureByName("mushroom");
		break;
	case IngredientType::SHRIMP:
		spriteRenderer->texture = AssetManager::GetTextureByName("shrimp");
		break;
	case IngredientType::STEAK:
		spriteRenderer->texture = AssetManager::GetTextureByName("steak");
		break;
	case IngredientType::TOMATO:
		spriteRenderer->texture = AssetManager::GetTextureByName("tomato");
		break;
	};
}