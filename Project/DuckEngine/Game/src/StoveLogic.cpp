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

#include "StoveLogic.h"


/****************************************************************
* @brief Start function for the Stove Logic
* ****************************************************************/
void StoveLogic::Start()
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
	tableSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(component->GetEntityID());
	object = nullptr;
	objectTransform = nullptr;
	objectSprite = nullptr;

	currCookTime = cookTime;
	isCooked = false;

	isPan = false;
	isPot = false;
	isOccupied = false;
}

/****************************************************************
* @brief Update function for the Stove Logic
* ****************************************************************/
void StoveLogic::Update()
{

}

/****************************************************************
* @brief FixedUpdate function for the Stove Logic
* ****************************************************************/
void StoveLogic::FixedUpdate()
{
	if (isOccupied && !isCooked)
	{
		cookObject();
	}
	else if (isOccupied && isCooked)
	{
		if (isPan) if (tableSFX) tableSFX->Play(2);
		if (isPot) if (tableSFX) tableSFX->Play(4);
	}
}

/****************************************************************
* @brief Set the object on the stove.
*
* @param objData - the object to be set on the stove
* ****************************************************************/
void StoveLogic::setObject(std::pair<int, ItemType> objData)
{
	// If either is true, change the sprite and type to the respective one.
	if (isPan || isPot)
	{
		// Delete the ingredient entity.
		DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(objData.first);

		if (isPan)
		{
			if (objData.second == ItemType::R_PATTY)
			{
				type = ItemType::PAN_R_PATTY;
			}
			else if (objData.second == ItemType::C_PATTY)
			{
				type = ItemType::PAN_C_PATTY;
			}
			else if (objData.second == ItemType::B_PATTY)
			{
				type = ItemType::PAN_B_PATTY;
			}
		}
		else if (isPot)
		{
			type = objData.second;
			std::cout << "Ingredient Type: " << whatType(type) << std::endl;
		}
		else
		{
			std::cout << "No Cooking Type" << std::endl;
			return;
		}

		setObjectSprite(type);

		isOccupied = true;
		currCookTime = cookTime;
	}
}

/****************************************************************
* @brief Get the object on the stove
*
* @return - Id and type of the object on the Stove
* ****************************************************************/
std::pair<int, ItemType> StoveLogic::moveObject()
{
	std::cout << "Moving Object from Stove";
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
	type = ItemType::EMPTY;

	isCooked = false;
	isOccupied = false;

	isPan = false;
	isPot = false;


	if(sliderLogic)
		sliderLogic->ResetSlider();


	return std::make_pair(objectID, temp);
}


/****************************************************************
* @brief Clear the Pot and return type of Soup
*
* @return - the type of soup
* ****************************************************************/
ItemType StoveLogic::moveSoup()
{
	ItemType temp{};
	switch (type)
	{
	case ItemType::POT_TOMATO:
		temp = ItemType::BOWL_TOMATO;
		break;
	case ItemType::POT_MUSHROOM:
		temp = ItemType::BOWL_MUSHROOM;
		break;
	case ItemType::POT_SUS:
		temp = ItemType::BOWL_SUS;
		break;
	}
	std::cout << whatType(temp) << std::endl;

	type = ItemType::POT;
	setObjectSprite(type);

	isCooked = false;
	isOccupied = false;

	if(sliderLogic)
		sliderLogic->ResetSlider();

	return temp;
}


/****************************************************************
* @brief Clear the Pan and return type of Patty
*
* @return - the type of patty
* ****************************************************************/
ItemType StoveLogic::movePatty()
{
	
	ItemType temp{};
	switch (type)
	{
	case ItemType::PAN_R_PATTY:
		temp = ItemType::R_PATTY;
		break;
	case ItemType::PAN_C_PATTY:
		temp = ItemType::C_PATTY;
		break;
	case ItemType::PAN_B_PATTY:
		temp = ItemType::B_PATTY;
		break;
	}
	std::cout << whatType(temp) << std::endl;

	type = ItemType::PAN;
	setObjectSprite(type);

	isCooked = false;
	isOccupied = false;

	if (sliderLogic)
		sliderLogic->ResetSlider();

	return temp;
}

/****************************************************************
* @brief Cook the object on the stove
* ****************************************************************/
void StoveLogic::cookObject()
{
	// Run slider logic
	if(sliderLogic)
		sliderLogic->EnableSlider(true);

	currCookTime -= DuckEngine::FixedDeltaTime();
	EmitSparks();
	

	if (currCookTime <= 0.f && !isCooked)
	{
		
		if (objectSprite)
		{			
			// If object is R_PATTY and not cooked.
			if(isPan)
			{
				
				if (type == ItemType::PAN_R_PATTY)
				{
					type = ItemType::PAN_C_PATTY;
					currCookTime = cookTime;
					if (tableSFX) {
						tableSFX->Stop();
						tableSFX->Play(5);
					}
						
					if (sliderLogic)
						sliderLogic->ResetSlider();
				}
				else if (type == ItemType::PAN_C_PATTY)
				{
					
					type = ItemType::PAN_B_PATTY;
					isCooked = true;
				}
			}
			
			else if(isPot)
			{
				if (tableSFX) tableSFX->Play(1);
				if (type == ItemType::C_TOMATO)
				{
					type = ItemType::POT_TOMATO;
					if (tableSFX) {
						tableSFX->Stop();
						tableSFX->Play(5);
					}
				}
				else if (type == ItemType::C_MUSHROOM)
				{
					type = ItemType::POT_MUSHROOM;
					if (tableSFX) {
						tableSFX->Stop();
						tableSFX->Play(5);
					}
				}
				else if (isIngredient(type))
				{
					type = ItemType::POT_SUS;
					if (tableSFX) {
						tableSFX->Stop();
						tableSFX->Play(5);
					}
				}
				isCooked = true;
			}

			setObjectSprite(type);
		}
	}

	// SFX
	if (isPan)
	{
		if (type == ItemType::PAN_R_PATTY) if (tableSFX) tableSFX->Play();
		if (type == ItemType::PAN_C_PATTY) if (tableSFX) tableSFX->Play(1);
	}
	else if (isPot)
	{
		if (type == ItemType::C_TOMATO) if (tableSFX) tableSFX->Play(3);
		if (type == ItemType::C_MUSHROOM) if (tableSFX) tableSFX->Play(3);
		if (isIngredient(type)) if (tableSFX) tableSFX->Play(3);
	}
}



/****************************************************************
* @brief Set the Object on the Stove to Pan.
*
* @param objType - the type of object to be set
*
* @return - true if the object is set
* ****************************************************************/
bool StoveLogic::setObjectSprite(ItemType objType)
{
	if (objectSprite)
	{
		if(isPan)
		{
			switch (objType)
			{
			case ItemType::PAN:
			case ItemType::R_PATTY:
			case ItemType::C_PATTY:
			case ItemType::B_PATTY:
			case ItemType::PAN_R_PATTY:
			case ItemType::PAN_C_PATTY:
			case ItemType::PAN_B_PATTY:
				objectSprite->texture = AssetManager::GetTextureByName(whatType(objType));
				break;
			default:
				std::cout << "Invalid Object Type" << std::endl;
				objectSprite->texture = AssetManager::GetTextureByName("NULL");
				return false;
			}
		}

		else if (isPot)
		{
			// Any Other Ingredient becomes SUS
			switch (objType)
			{
			case ItemType::POT_MUSHROOM:
			case ItemType::POT_TOMATO:
			case ItemType::POT_SUS:
				objectSprite->texture = AssetManager::GetTextureByName(whatType(objType));
				break;
			default:
				objectSprite->texture = AssetManager::GetTextureByName("pot");
			}
		}
	}


	return true;
}


/****************************************************************
* @brief Set the cooking type of the stove
*
* @param objType - Either Pan or Pot
* ****************************************************************/
void StoveLogic::setCookingType(std::pair<int, ItemType> objData)
{
	// Assign new object
	object = DuckEngine::DUCKENGINE_EntityManager.GetEntity(objData.first).get();

	objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objData.first);
	if (objectTransform)
	{
		objectTransform->SetPosition(tableTransform->GetPosition() + Vec2(0.0f, 0.6f));
	}

	objectSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(objData.first);
	objectSprite->sortingOrder = 7;

	if (objData.second == ItemType::PAN)
	{
		isPan = true;
		isPot = false;
	}
	else if (objData.second == ItemType::POT)
	{
		isPan = false;
		isPot = true;
	}
	else
	{
		std::cout << "Invalid Object Type : "<< whatType(objData.second) << std::endl;
		return;
	}

	objectSprite->texture = AssetManager::GetTextureByName(whatType(objData.second));
	type = objData.second;

}


void StoveLogic::EmitSparks() {
	DuckEngine::Emit("CookingSparks", tableTransform->GetPosition(), {0.2f, 0.2f});
}