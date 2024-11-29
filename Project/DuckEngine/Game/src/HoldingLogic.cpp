/******************************************************************************/
/*!
\file       HoldingLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Definition of all Holding Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "HoldingLogic.h"
#include <iostream>

void HoldingLogic::Start()
{
	holder = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	holderTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	// Set Object ptr only when player is holding something
	object = nullptr;
	objectTransform = nullptr;
	type = ItemType::EMPTY;
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


void HoldingLogic::setObject(std::pair<int, ItemType> objData)
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
	type = ItemType::EMPTY;
	DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objectID);
}

std::pair<int,ItemType> HoldingLogic::moveObject()
{
	if (!object)
	{
		return std::pair<int, ItemType>();
	}
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;

	ItemType temp = type;
	type = ItemType::EMPTY;
	
	return std::make_pair(objectID, temp);
}