#include "TableLogic.h"

void TableLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	objectOnTable = nullptr;
	objectTransform = nullptr;
	isOccupied = false;

}

void TableLogic::Update()
{
}

void TableLogic::FixedUpdate()
{
}

void TableLogic::setObject(std::pair<int, IngredientType> objData)
{
	objectOnTable = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectTransform->SetPosition(tableTransform->GetPosition());
	type = objData.second;
	isOccupied = true;
}

std::pair<int, IngredientType> TableLogic::moveObject()
{
	int objectID = objectOnTable->entityID;
	objectOnTable = nullptr;
	objectTransform = nullptr;
	isOccupied = false;

	IngredientType temp = type;
	type = IngredientType::EMPTY;

	return std::make_pair(objectID, temp);
}