/******************************************************************************/
/*!
\file       ChopBoardLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
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
	if (table)
	{
		if (table->childEntities.size() > 0)
		{
			for (int i = 0; i < table->childEntities.size(); i++)
			{
				sliderLogic = GameLogicManager::GetLogicForEntity<SliderLogic>(table->childEntities[i]->entityID);
				if (sliderLogic)
				{
					std::cout << "Found Slider\n"; break;
				}
				else
				{
					std::cout << "Slider Not Found\n";
				}
			}
		}
	}
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());

	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	type = ItemType::EMPTY;
	currChopTime = chopTime;
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
		objectTransform->SetPosition(tableTransform->GetPosition() + Vec2(0.0f, 0.3f));

		objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
		type = objData.second;
		isOccupied = true;
		isChopped = false;
		currChopTime = chopTime;
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

	if (sliderLogic)
		sliderLogic->ResetSlider();

	return std::make_pair(objectID, temp);
}


/****************************************************************
* @brief Chop the object on the chopping board
* ***************************************************************/
void ChopBoardLogic::chopObject()
{
	// Run slider logic
	if (sliderLogic)
		sliderLogic->EnableSlider(true);

	currChopTime -= DuckEngine::FixedDeltaTime();
	if (currChopTime <= 0.f)
	{
		isChopped = true;
	}
}


/****************************************************************
* @brief Check if the object can be put on the chopping board
*
* @param type - the type of the object.
* 
* Types Allowed: CHEESE, LETTUCE, MUSHROOM, SHRIMP, STEAK, TOMATO
*
* @return true if the object can be put on the chopping board, false otherwise
* ***************************************************************/
bool ChopBoardLogic::checkIngredient(ItemType ingredientType)
{
	switch (ingredientType)
	{
	case ItemType::CHEESE:
	case ItemType::LETTUCE:
	case ItemType::MUSHROOM:
	case ItemType::SHRIMP:
	case ItemType::STEAK:
	case ItemType::TOMATO:
		return true;
	default:
		return false;
	}
}