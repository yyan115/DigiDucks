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
	if(objectTransform)
	{
	}
}


void TableLogic::setObject(int EntityID)
{
	objectOnTable = DuckEngine::DUCKENGINE_EntityManager.GetEntity(EntityID);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(EntityID);
	objectTransform->SetPosition(tableTransform->GetPosition());
	//std::cout << "Table ID: " << table->entityID << std::endl;
	//std::cout << "Table Transform: " << tableTransform->GetPosition().x << ", " << tableTransform->GetPosition().y << std::endl;
	//std::cout << "Object Transform: " << objectTransform->GetPosition().x << ", " << objectTransform->GetPosition().y << std::endl;
	isOccupied = true;
}

int TableLogic::moveObject()
{
	int objectID = objectOnTable->entityID;
	objectOnTable = nullptr;
	objectTransform = nullptr;
	isOccupied = false;
	return objectID;
}