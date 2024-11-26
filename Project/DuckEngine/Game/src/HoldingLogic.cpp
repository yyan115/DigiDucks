#include "HoldingLogic.h"
#include <iostream>

void HoldingLogic::Start()
{
	holder = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	holderTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	// Set Object ptr only when player is holding something
	object = nullptr;
	objectTransform = nullptr;
}


void HoldingLogic::Update()
{

}


void HoldingLogic::FixedUpdate()
{
	if (objectTransform)
	{
		objectTransform->SetPosition(holderTransform->GetPosition() + offSet);
	}
}


void HoldingLogic::setObject(std::pair<int, IngredientType> objData)
{
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	type = objData.second;
}


void HoldingLogic::deleteObject()
{
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;
	type = IngredientType::EMPTY;
	DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objectID);
}

std::pair<int,IngredientType> HoldingLogic::moveObject()
{
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;
	IngredientType temp = type;
	type = IngredientType::EMPTY;
	
	return std::make_pair(objectID, temp);
}