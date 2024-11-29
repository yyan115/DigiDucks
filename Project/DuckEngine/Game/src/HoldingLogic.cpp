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

/****************************************************************
* @brief Start function for HoldingLogic
* ****************************************************************/
void HoldingLogic::Start()
{
	holder = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	holderTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	// Set Object ptr only when player is holding something
	object = nullptr;
	objectTransform = nullptr;
	type = ItemType::EMPTY;
}


/****************************************************************
* @brief Update function for HoldingLogic
* ****************************************************************/
void HoldingLogic::Update()
{

}


/****************************************************************
* @brief FixedUpdate function for HoldingLogic
* ****************************************************************/
void HoldingLogic::FixedUpdate()
{
	if (objectTransform)
	{
		objectTransform->SetPosition(holderTransform->GetPosition() + offSet);
	}
}


/****************************************************************
* @brief setObject function for HoldingLogic
*
* @param objData - pair of object ID and ItemType
* ****************************************************************/
void HoldingLogic::setObject(std::pair<int, ItemType> objData)
{
	std::cout << " Object ID: " << objData.first << " Type: " << whatType(objData.second) << std::endl;
	if (!object)
	{
		object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
		objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
		type = objData.second;		
	}
}


/****************************************************************
* @brief deleteObject function for HoldingLogic
* ****************************************************************/
void HoldingLogic::deleteObject()
{
	if (object) {
		int objectID = object->entityID;
		DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objectID);
		object = nullptr;
		objectTransform = nullptr;
		type = ItemType::EMPTY;
	}
}

/****************************************************************
* @brief moveObject function for HoldingLogic
*
* @return pair of object ID and ItemType
* ****************************************************************/
std::pair<int,ItemType> HoldingLogic::moveObject()
{
	std::cout << "Moving Object from hand" << std::endl;
	if (!object)
	{
		std::cout << "OBJ is NULLPTR" << std::endl;
		return std::pair<int, ItemType>();
	}
	int objectID = object->entityID;
	object = nullptr;
	objectTransform = nullptr;

	ItemType temp = type;
	type = ItemType::EMPTY;
	
	return std::make_pair(objectID, temp);
}