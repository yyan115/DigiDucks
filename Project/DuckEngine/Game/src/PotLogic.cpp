/******************************************************************************/
/*!
\file       PotLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernest@digipen.edu
\date       Mar 21 2025
\brief      Definition of Pot Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "PotLogic.h"

/****************************************************************
* @brief Start function for the Pot Logic
* ****************************************************************/
void PotLogic::Start()
{
	potEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	if (potEntity)
	{
		potSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(potEntity->entityID);
		if (potEntity)
		{
			potSprite->texture = AssetManager::GetTextureByName(whatType(type));
		}
	}

	currPortion = 0;
	isPotFilled = false;
}

/****************************************************************
* @brief Update function for the Pot Logic
* ****************************************************************/
void PotLogic::Update()
{

}

/****************************************************************
* @brief FixedUpdate function for the Pot Logic
* ****************************************************************/
void PotLogic::FixedUpdate()
{

}


/****************************************************************
* @brief Minus the portion of soup in the pot
*
* @return - the type of soup
* ****************************************************************/
ItemType PotLogic::TakeSoup()
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

	currPortion--;
	std::cout << "Portion: " << currPortion << std::endl;
	if (currPortion <= 0)
	{
		type = ItemType::POT;
		isPotFilled = false;
	}

	if (potSprite)
	{
		potSprite->texture = AssetManager::GetTextureByName(whatType(type));
	}

	return temp;
}