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
	table = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	if (table)
	{
		if (table->childEntities.size() > 0)
		{
			for (int i = 0; i < table->childEntities.size(); i++)
			{
				sliderLogic = GameLogicManager::GetLogicForEntity<SliderLogic>(table->childEntities[i]->entityID);
				if (sliderLogic)
					break;
			}
		}
	}
	tableTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());

	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	isOccupied = false;
	cookTime = 1.f;
}

/****************************************************************
* @brief Update function for the Pan Logic
* ****************************************************************/
void PanLogic::Update()
{
	if (isOccupied && type != ItemType::B_PATTY)
	{
		cookObject();
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
	std::cout << "Pan Object ID: " << objData.first << " Type: " << whatType(objData.second) << std::endl;
	// Assign new object
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first).get();

	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	objectTransform->SetPosition(tableTransform->GetPosition());

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	objectSprite->texture = AssetManager::GetTextureByName("fryingpan_raw");
	objectSprite->sortingOrder = 7;

	type = objData.second;
	isOccupied = true;
	isCooked = false;
	cookTime = 3.0f;
}

/****************************************************************
* @brief Get the object in the pan
*
* @return - the object in the pan
* ****************************************************************/
std::pair<int, ItemType> PanLogic::moveObject()
{
	std::cout << "Moving Object from Pan";
	if (!object)
	{
		std::cout << "OBJ is NULLPTR" << std::endl;
		return std::pair<int, ItemType>();
	}

	if (type == ItemType::R_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("patty");
	}
	else if (type == ItemType::C_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("cooked_patty");
	}
	else if (type == ItemType::B_PATTY)
	{
		objectSprite->texture = AssetManager::GetTextureByName("burnt_patty");
	}

	std::cout << "Object ID: " << object->entityID << " Type: " << whatType(type) << std::endl;
	int objectID = object->entityID;
	ItemType temp = type;

	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;
	type = ItemType::EMPTY;
	isOccupied = false;

	if(sliderLogic)
		sliderLogic->ResetSlider();

	return std::make_pair(objectID, temp);
}

/****************************************************************
* @brief Cook the object in the pan
* ****************************************************************/
void PanLogic::cookObject()
{
	// Run slider logic
	if(sliderLogic)
		sliderLogic->EnableSlider(true);

	cookTime -= DuckEngine::FixedDeltaTime();
	if (cookTime <= 0.f)
	{
		if (objectSprite)
		{
			// If object is R_PATTY and not cooked.
			if (type == ItemType::R_PATTY && !isCooked)
			{
				objectSprite->texture = AssetManager::GetTextureByName("fryingpan_cooked");
				type = ItemType::C_PATTY;
				isCooked = true;
				cookTime = 3.0f;

				if (sliderLogic)
					sliderLogic->ResetSlider();
			}
			else if (type == ItemType::C_PATTY && isCooked)
			{
				objectSprite->texture = AssetManager::GetTextureByName("fryingpan_burnt");
				type = ItemType::B_PATTY;
				isCooked = true;
			}
		}
	}
}

void PanLogic::EmitSparks() {
	DuckEngine::Emit("CookingSparks", tableTransform->GetPosition(), {0.2f, 0.2f});
}