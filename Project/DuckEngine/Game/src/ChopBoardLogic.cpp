#include "ChopBoardLogic.h"

void ChopBoardLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	type = IngredientType::EMPTY;
	chopTime = 2.f;
	isOccupied = false;
	isChopped = false;

}

void ChopBoardLogic::Update()
{
	if (isChopped)
	{
		switch (type)
		{
		case IngredientType::CHEESE:
			objectSprite->texture = AssetManager::GetTextureByName("cheese_chop");
			type = IngredientType::C_CHEESE;
			break;
		case IngredientType::LETTUCE:
			objectSprite->texture = AssetManager::GetTextureByName("lettuce_chop");
			type = IngredientType::C_LETTUCE;
			break;
		case IngredientType::MUSHROOM:
			objectSprite->texture = AssetManager::GetTextureByName("mushroom_chop");
			type = IngredientType::C_MUSHROOM;
			break;
		case IngredientType::SHRIMP:
			objectSprite->texture = AssetManager::GetTextureByName("shrimp_chop");
			type = IngredientType::C_SHRIMP;
			break;
		case IngredientType::STEAK:
			objectSprite->texture = AssetManager::GetTextureByName("patty");
			type = IngredientType::R_PATTY;
			break;
		case IngredientType::TOMATO:
			objectSprite->texture = AssetManager::GetTextureByName("tomato_chop");
			type = IngredientType::C_TOMATO;
			break;
		default:
			break;
		};
	}
}

void ChopBoardLogic::FixedUpdate()
{
}

void ChopBoardLogic::setObject(std::pair<int, IngredientType> objData)
{
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectTransform->SetPosition(tableTransform->GetPosition());

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	type = objData.second;
	isOccupied = true;
	isChopped = false;
	chopTime = 3.f;
}

std::pair<int, IngredientType> ChopBoardLogic::moveObject()
{
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	isOccupied = false;

	IngredientType temp = type;
	type = IngredientType::EMPTY;

	return std::make_pair(objectID, temp);
}


void ChopBoardLogic::chopObject()
{
	chopTime -= DuckEngine::DeltaTime() / 3.f;
	if (chopTime <= 0.f)
	{
		isChopped = true;
	}
}