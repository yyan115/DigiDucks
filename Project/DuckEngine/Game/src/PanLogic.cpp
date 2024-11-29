/******************************************************************************/
/*!
\file       PanLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Definition of all Pan Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "PanLogic.h"


/****************************************************************
* @brief Start function for the Pan Logic
* ****************************************************************/
void PanLogic::Start()
{
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID());
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());

	object = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Frying_Pan");
	if (object) {
		objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(object->entityID);
		objectTransform->SetPosition(tableTransform->GetPosition());
		objectTransform->scale = Vec2(1.5f, 1.5f);
		objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(object->entityID);
		objectSprite->texture = AssetManager::GetTextureByName("fryingpan");
		type = ItemType::EMPTY;
	}
	isOccupied = false;
	isCooked = false;
	cookTime = 1.f;
}

/****************************************************************
* @brief Update function for the Pan Logic
* ****************************************************************/
void PanLogic::Update()
{
	if (isCooked)
	{
		switch (type)
		{
		case ItemType::R_PATTY:
			objectSprite->texture = AssetManager::GetTextureByName("fryingpan_cooked");
			type = ItemType::C_PATTY;
			break;
		default: 
			break;
		};
	}

}

/****************************************************************
* @brief FixedUpdate function for the Pan Logic
* ****************************************************************/
void PanLogic::FixedUpdate()
{
}

/****************************************************************
* @brief Set the object in the pan
*
* @param objData - the object to be set in the pan
* ****************************************************************/
void PanLogic::setObject(std::pair<int, ItemType> objData)
{
	int objectID{};
	if (object) {
		// Delete Empty Pan Object
		objectID = object->entityID;
		DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objectID);
		object = nullptr;
		objectTransform = nullptr;
		objectSprite = nullptr;
		type = ItemType::EMPTY;
		isOccupied = false;
	}

	// Assign new object
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first);
	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectTransform->SetPosition(tableTransform->GetPosition());

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	objectSprite->texture = AssetManager::GetTextureByName("fryingpan_raw");

	type = objData.second;
	isOccupied = true;

}

/****************************************************************
* @brief Get the object in the pan
*
* @return - the object in the pan
* ****************************************************************/
std::pair<int, ItemType> PanLogic::moveObject()
{
	int objectID = object->entityID;
	ItemType temp = type;

	if (type == ItemType::R_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("patty");
	}
	else if (type == ItemType::C_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("cooked_patty");
	}

	makeEmptyPan();

	return std::make_pair(objectID, temp);
}

/****************************************************************
* @brief Cook the object in the pan
* ****************************************************************/
void PanLogic::cookObject()
{
	cookTime -= DuckEngine::DeltaTime();
	if (cookTime <= 0.f)
	{
		isCooked = true;
	}
}


/****************************************************************
* @brief Make the pan empty
* ****************************************************************/
void PanLogic::makeEmptyPan()
{
	object = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity();

	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(object->entityID);
	objectTransform->SetPosition(tableTransform->GetPosition());
	objectTransform->scale = Vec2(1.5f, 1.5f);

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(object->entityID);
	objectSprite->texture = AssetManager::GetTextureByName("fryingpan");
	objectSprite->sortingOrder = 2;

	isOccupied = false;
	isCooked = false;

	type = ItemType::EMPTY;
	cookTime = 1.f;
}