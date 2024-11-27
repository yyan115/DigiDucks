/******************************************************************************/
/*!
\file       TableLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 26 2024
\brief      Definition of all Table logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

void TableLogic::setObject(std::pair<int, ItemType> objData)
{
	objectOnTable = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectTransform->SetPosition(tableTransform->GetPosition());
	type = objData.second;
	isOccupied = true;
}

std::pair<int, ItemType> TableLogic::moveObject()
{
	int objectID = objectOnTable->entityID;
	objectOnTable = nullptr;
	objectTransform = nullptr;
	isOccupied = false;

	ItemType temp = type;
	type = ItemType::EMPTY;

	return std::make_pair(objectID, temp);
}