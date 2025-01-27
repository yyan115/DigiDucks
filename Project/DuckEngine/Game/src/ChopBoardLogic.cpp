/******************************************************************************/
/*!
\file       ChopBoardLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Definition of all Chopping Board Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "ChopBoardLogic.h"


/****************************************************************
* @brief Start function for the ChopBoardLogic
* ***************************************************************/
void ChopBoardLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	type = ItemType::EMPTY;
	chopTime = 1.f;
	isOccupied = false;
	isChopped = false;

}


/****************************************************************
* @brief Update function for the ChopBoardLogic
* ***************************************************************/
void ChopBoardLogic::Update()
{
	if (isChopped)
	{
		switch (type)
		{
		case ItemType::CHEESE:
			objectSprite->texture = AssetManager::GetTextureByName("cheese_chop");
			type = ItemType::C_CHEESE;
			break;
		case ItemType::LETTUCE:
			objectSprite->texture = AssetManager::GetTextureByName("lettuce_chop");
			type = ItemType::C_LETTUCE;
			break;
		case ItemType::MUSHROOM:
			objectSprite->texture = AssetManager::GetTextureByName("mushroom_chop");
			type = ItemType::C_MUSHROOM;
			break;
		case ItemType::SHRIMP:
			objectSprite->texture = AssetManager::GetTextureByName("shrimp_chop");
			type = ItemType::C_SHRIMP;
			break;
		case ItemType::STEAK:
			objectSprite->texture = AssetManager::GetTextureByName("patty");
			type = ItemType::R_PATTY;
			break;
		case ItemType::TOMATO:
			objectSprite->texture = AssetManager::GetTextureByName("tomato_chop");
			type = ItemType::C_TOMATO;
			break;
		default:
			break;
		};
	}
}

/****************************************************************
* @brief FixedUpdate function for the ChopBoardLogic
* ***************************************************************/
void ChopBoardLogic::FixedUpdate()
{
}

/****************************************************************
* @brief Set the object on the chopping board
*
* @param objData - the object data to be set
* ***************************************************************/
void ChopBoardLogic::setObject(std::pair<int, ItemType> objData)
{
	std::cout << "Chop Object ID: " << objData.first << " Type: " << whatType(objData.second) << std::endl;
	if (!object) {
		object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first).get();
		objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
		objectTransform->SetPosition(tableTransform->GetPosition());

		objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
		type = objData.second;
		isOccupied = true;
		isChopped = false;
		chopTime = 3.0f;
	}
}


/****************************************************************
* @brief Move the object on the chopping board
*
* @return std::pair<int, ItemType> - the object data to be moved
* ***************************************************************/
std::pair<int, ItemType> ChopBoardLogic::moveObject()
{

	std::cout << "Moving Object from Chopboard";
	if (!object)
	{
		std::cout << "OBJ is NULLPTR" << std::endl;
		return std::pair<int, ItemType>();
	}
	std::cout << "Object ID: " << object->entityID << " Type: " << whatType(type) << std::endl;

	int objectID = object->entityID;
	ItemType temp = type;

	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	isOccupied = false;
	type = ItemType::EMPTY;

	return std::make_pair(objectID, temp);
}


/****************************************************************
* @brief Chop the object on the chopping board
* ***************************************************************/
void ChopBoardLogic::chopObject()
{
	chopTime -= DuckEngine::FixedDeltaTime();
	if (chopTime <= 0.f)
	{
		isChopped = true;
	}
}