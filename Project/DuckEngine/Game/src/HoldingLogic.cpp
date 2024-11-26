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


void HoldingLogic::setObject(int EntityID)
{
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(EntityID);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(EntityID);
}


void HoldingLogic::deleteObject()
{
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;
	DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objectID);
}

int HoldingLogic::moveObject()
{
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;
	return objectID;
}