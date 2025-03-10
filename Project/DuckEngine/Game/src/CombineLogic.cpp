/******************************************************************************/
/*!
\file       CombineLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Definition of all Combine Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "CombineLogic.h"

/****************************************************************
* @brief Check if two items can be combined
*
* @param lhs - the first item
* @param rhs - the second item
*
* @return true if the two items can be combined, false otherwise*
* ****************************************************************/
bool canCombine(ItemType lhs, ItemType rhs)
{
	// Make lhs always the smaller value
	if (static_cast<int>(lhs) > static_cast<int>(rhs))
	{
		std::swap(lhs, rhs);
	}

    // Define the possible combinations for lhs
    switch (lhs)
    {
		// BURGER INGREDIENTS
		// Single Ingredient combine with empty or partial plates
    case ItemType::BUN:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::CHEESE_PLATE || rhs == ItemType::PATTY_PLATE || rhs == ItemType::CHEESE_PATTY_PLATE);
    case ItemType::C_CHEESE:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::BUN_PLATE || rhs == ItemType::PATTY_PLATE || rhs == ItemType::BURGER_PLATE);
    case ItemType::C_PATTY:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::CHEESE_PLATE || rhs == ItemType::BUN_PLATE || rhs == ItemType::BUN_CHEESE_PLATE);

		// SALAD INGREDIENTS
		// Single ingredients can combine with empty or partial plates
    case ItemType::C_LETTUCE:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::TOMATO_PLATE || rhs == ItemType::SHRIMP_PLATE || rhs == ItemType::TOMATO_SHRIMP_PLATE);
    case ItemType::C_TOMATO:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::LETTUCE_PLATE || rhs == ItemType::SHRIMP_PLATE || rhs == ItemType::LETTUCE_SHRIMP_PLATE);
    case ItemType::C_SHRIMP:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::TOMATO_PLATE || rhs == ItemType::LETTUCE_PLATE || rhs == ItemType::LETTUCE_TOMATO_PLATE);

    default:
        return false;
    }
}


/****************************************************************
* @brief Combine two items
*
* @param lhs - the first item
* @param rhs - the second item
*
* @return the combined item
* ****************************************************************/
std::pair<int, ItemType> combineObjects(std::pair<int, ItemType> lhs, std::pair<int, ItemType> rhs) {
    auto transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(lhs.first);
    if (!transform) {
        std::cerr << "Error: No transform component on first object\n";
        return std::pair<int, ItemType>();
    }

    Vector2D position = transform->GetPosition();
    Vector2D scale = transform->scale;

    Entity* newEntity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity().get();
    auto newTransform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(newEntity->entityID);
    auto sprite = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(newEntity->entityID, true);

    newTransform->SetPosition(position);
    newTransform->scale = scale;
    sprite->sortingOrder = 7;

    ItemType newItem = ItemType::EMPTY;

    // Make lhs always the smaller value
    if (static_cast<int>(lhs.second) > static_cast<int>(rhs.second))
    {
		std::swap(lhs, rhs);
    }

    switch (lhs.second) {

    case ItemType::BUN:
        if (rhs.second == ItemType::WHITE_PLATE) {
            newItem = ItemType::BUN_PLATE;
        }
        else if (rhs.second == ItemType::CHEESE_PLATE) {
            newItem = ItemType::BUN_CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::PATTY_PLATE) {
            newItem = ItemType::BURGER_PLATE;
        }
        else if (rhs.second == ItemType::CHEESE_PATTY_PLATE) {
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
        break;

    case ItemType::C_CHEESE:
        if (rhs.second == ItemType::WHITE_PLATE) {
            newItem = ItemType::CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::BUN_PLATE) {
            newItem = ItemType::BUN_CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::PATTY_PLATE) {
            newItem = ItemType::CHEESE_PATTY_PLATE;
        }
        else if (rhs.second == ItemType::BURGER_PLATE) {
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
        break;

    case ItemType::C_PATTY:
        if (rhs.second == ItemType::WHITE_PLATE) {
            newItem = ItemType::PATTY_PLATE;
        }
        else if (rhs.second == ItemType::BUN_PLATE) {
            newItem = ItemType::BURGER_PLATE;
        }
        else if (rhs.second == ItemType::CHEESE_PLATE) {
            newItem = ItemType::CHEESE_PATTY_PLATE;
        }
        else if (rhs.second == ItemType::BUN_CHEESE_PLATE) {
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
        break;
    

        // SALAD INGREDIENTS
    case ItemType::C_LETTUCE:
        if (rhs.second == ItemType::WHITE_PLATE) {
            newItem = ItemType::LETTUCE_PLATE;
        }
        else if (rhs.second == ItemType::TOMATO_PLATE) {
            newItem = ItemType::LETTUCE_TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::SHRIMP_PLATE) {
            newItem = ItemType::LETTUCE_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::TOMATO_SHRIMP_PLATE) {
            newItem = ItemType::SALAD_PLATE;
        }
        break;

    case ItemType::C_TOMATO:
        if (rhs.second == ItemType::WHITE_PLATE) {
            newItem = ItemType::TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::LETTUCE_PLATE) {
            newItem = ItemType::LETTUCE_TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::SHRIMP_PLATE) {
            newItem = ItemType::TOMATO_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::LETTUCE_SHRIMP_PLATE) {
            newItem = ItemType::SALAD_PLATE;
        }
        break;

    case ItemType::C_SHRIMP:
        if (rhs.second == ItemType::WHITE_PLATE) {
            newItem = ItemType::SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::LETTUCE_PLATE) {
            newItem = ItemType::LETTUCE_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::TOMATO_PLATE) {
            newItem = ItemType::TOMATO_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::LETTUCE_TOMATO_PLATE) {
            newItem = ItemType::SALAD_PLATE;
        }
        break;

    default:
        break;
    }


    sprite->texture = AssetManager::GetTextureByName(whatType(newItem));

    if (newItem != ItemType::EMPTY) {
        DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(lhs.first);
        DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(rhs.first);
        return std::make_pair(newEntity->entityID, newItem);
    }
    else {
        DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(newEntity->entityID);
        return std::pair<int, ItemType>();
    }
}