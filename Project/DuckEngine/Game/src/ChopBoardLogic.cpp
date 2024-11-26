#include "ChopBoardLogic.h"

void ChopBoardLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	type = IngredientType::EMPTY;
	chopTime = 3.f;
	isOccupied = false;

}

void ChopBoardLogic::Update()
{
}

void ChopBoardLogic::FixedUpdate()
{
}

void ChopBoardLogic::setObject(std::pair<int, IngredientType> objData)
{
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	type = objData.second;
	isOccupied = true;
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

}