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
    // Define the possible combinations for lhs
    switch (lhs)
    {
        // Empty plates can combine with initial ingredients
    case ItemType::GREY_PLATE:
        return (rhs == ItemType::BUN || rhs == ItemType::C_CHEESE || rhs == ItemType::C_PATTY);

    case ItemType::WHITE_PLATE:
        return (rhs == ItemType::C_LETTUCE || rhs == ItemType::C_TOMATO || rhs == ItemType::C_SHRIMP);


		// BURGER INGREDIENTS
		// Single Ingredient combine with empty or partial plates
    case ItemType::BUN:
        return (rhs == ItemType::GREY_PLATE || rhs == ItemType::CHEESE_PLATE || rhs == ItemType::PATTY_PLATE || rhs == ItemType::CHEESE_PATTY_PLATE);
    case ItemType::C_CHEESE:
        return (rhs == ItemType::GREY_PLATE || rhs == ItemType::BUN_PLATE || rhs == ItemType::PATTY_PLATE || rhs == ItemType::BURGER_PLATE);
    case ItemType::C_PATTY:
        return (rhs == ItemType::GREY_PLATE || rhs == ItemType::CHEESE_PLATE || rhs == ItemType::BUN_PLATE || rhs == ItemType::BUN_CHEESE_PLATE);

		// Partial plate combine with single ingredients
    case ItemType::BUN_PLATE:
        return (rhs == ItemType::C_CHEESE || rhs == ItemType::C_PATTY);

    case ItemType::CHEESE_PLATE:
        return (rhs == ItemType::BUN || rhs == ItemType::C_PATTY);

    case ItemType::PATTY_PLATE:
        return (rhs == ItemType::BUN || rhs == ItemType::C_CHEESE);

    case ItemType::BUN_CHEESE_PLATE:
        return (rhs == ItemType::C_PATTY);

    case ItemType::BURGER_PLATE:
        return (rhs == ItemType::C_CHEESE);

    case ItemType::CHEESE_PATTY_PLATE:
        return (rhs == ItemType::BUN);


		// SALAD INGREDIENTS
		// Single ingredients can combine with empty or partial plates
    case ItemType::C_LETTUCE:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::TOMATO_PLATE || rhs == ItemType::SHRIMP_PLATE || rhs == ItemType::TOMATO_SHRIMP_PLATE);
    case ItemType::C_TOMATO:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::LETTUCE_PLATE || rhs == ItemType::SHRIMP_PLATE || rhs == ItemType::LETTUCE_SHRIMP_PLATE);
    case ItemType::C_SHRIMP:
        return (rhs == ItemType::WHITE_PLATE || rhs == ItemType::TOMATO_PLATE || rhs == ItemType::LETTUCE_PLATE || rhs == ItemType::LETTUCE_TOMATO_PLATE);

        // Partial plate combine with single ingredients
    case ItemType::LETTUCE_PLATE:
        return (rhs == ItemType::C_TOMATO || rhs == ItemType::C_SHRIMP);

    case ItemType::TOMATO_PLATE:
        return (rhs == ItemType::C_LETTUCE || rhs == ItemType::C_SHRIMP);

    case ItemType::SHRIMP_PLATE:
        return (rhs == ItemType::C_LETTUCE || rhs == ItemType::C_TOMATO);

    case ItemType::LETTUCE_TOMATO_PLATE:
        return (rhs == ItemType::C_SHRIMP);

    case ItemType::LETTUCE_SHRIMP_PLATE:
        return (rhs == ItemType::C_TOMATO);

    case ItemType::TOMATO_SHRIMP_PLATE:
        return (rhs == ItemType::C_LETTUCE);

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
std::pair<int, ItemType> combineObjects(std::pair<int, ItemType> lhs, std::pair<int, ItemType> rhs)
{
    DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(rhs.first);
    ItemType newItem = ItemType::EMPTY;
    int ObjectID = lhs.first;
	SpriteRendererComponent* sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(ObjectID);
    if (!sprite)
    {
        std::cout << "NO SPRITE\n";
        return std::pair<int, ItemType>();
    }

    // Define the possible combinations for lhs
    switch (lhs.second)
    {
        // Empty plates can combine with initial ingredients
    case ItemType::GREY_PLATE:
        if (rhs.second == ItemType::BUN) 
        {
            sprite->texture = AssetManager::GetTextureByName("bun_plate");
			newItem = ItemType::BUN_PLATE;
            break;
		}
		else if (rhs.second == ItemType::C_CHEESE) 
        {
			sprite->texture = AssetManager::GetTextureByName("cheese_plate");
			newItem = ItemType::CHEESE_PLATE;
			break;
		} 
		else if (rhs.second == ItemType::C_PATTY) 
        {
			sprite->texture = AssetManager::GetTextureByName("patty_plate");
			newItem = ItemType::PATTY_PLATE;
			break;
		}
        break;

    case ItemType::WHITE_PLATE:
		if (rhs.second == ItemType::C_LETTUCE) 
        {
			sprite->texture = AssetManager::GetTextureByName("lettuce_plate");
			newItem = ItemType::LETTUCE_PLATE;
			break;
		}
		else if (rhs.second == ItemType::C_TOMATO) 
        {
			sprite->texture = AssetManager::GetTextureByName("tomato_plate");
			newItem = ItemType::TOMATO_PLATE;
			break;
		}
		else if (rhs.second == ItemType::C_SHRIMP) 
        {
			sprite->texture = AssetManager::GetTextureByName("shrimp_plate");
			newItem = ItemType::SHRIMP_PLATE;
			break;
		}
        break;

        // BURGER INGREDIENTS
        // Single Ingredient combine with empty or partial plates
    case ItemType::BUN:
    {
        if (rhs.second == ItemType::GREY_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("bun_plate");
            newItem = ItemType::BUN_PLATE;
        }
        else if (rhs.second == ItemType::CHEESE_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("bun_cheese_plate");
            newItem = ItemType::BUN_CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::PATTY_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("burger_plate");
            newItem = ItemType::BURGER_PLATE;
        }
        else if (rhs.second == ItemType::CHEESE_PATTY_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_burger_plate");
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
    }
        break;
    case ItemType::C_CHEESE:
    {
        if (rhs.second == ItemType::GREY_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_plate");
            newItem = ItemType::CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::BUN_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("bun_cheese_plate");
            newItem = ItemType::BUN_CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::PATTY_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_patty_plate");
            newItem = ItemType::CHEESE_PATTY_PLATE;
        }
        else if (rhs.second == ItemType::BURGER_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_burger_plate");
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
    }
        break;
    case ItemType::C_PATTY:
    {
        if (rhs.second == ItemType::GREY_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("patty_plate");
            newItem = ItemType::PATTY_PLATE;
        }
        else if (rhs.second == ItemType::BUN_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("burger_plate");
            newItem = ItemType::BURGER_PLATE;
        }
        else if (rhs.second == ItemType::CHEESE_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_patty_plate");
            newItem = ItemType::CHEESE_PATTY_PLATE;
        }
        else if (rhs.second == ItemType::BUN_CHEESE_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_burger_plate");
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
    }
		break;

        // Partial plate combine with single ingredients
    case ItemType::BUN_PLATE:
    {
        if (rhs.second == ItemType::C_CHEESE)
        {
            sprite->texture = AssetManager::GetTextureByName("bun_cheese_plate");
            newItem = ItemType::BUN_CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::C_PATTY)
        {
            sprite->texture = AssetManager::GetTextureByName("burger_plate");
            newItem = ItemType::BURGER_PLATE;
        }
    }
        break;

    case ItemType::CHEESE_PLATE:
    {
        if (rhs.second == ItemType::BUN)
        {
            sprite->texture = AssetManager::GetTextureByName("bun_cheese_plate");
            newItem = ItemType::BUN_CHEESE_PLATE;
        }
        else if (rhs.second == ItemType::C_PATTY)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_patty_plate");
            newItem = ItemType::CHEESE_PATTY_PLATE;
            newItem = ItemType::CHEESE_PATTY_PLATE;
        }
    }
        break;

    case ItemType::PATTY_PLATE:
    {
        if (rhs.second == ItemType::BUN)
        {
            sprite->texture = AssetManager::GetTextureByName("burger_plate");
            newItem = ItemType::BURGER_PLATE;
        }
        else if (rhs.second == ItemType::C_CHEESE)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_patty_plate");
            newItem = ItemType::CHEESE_PATTY_PLATE;
        }
    }
        break;

    case ItemType::BUN_CHEESE_PLATE:
    {
        if (rhs.second == ItemType::C_PATTY)
        {
            sprite->texture = AssetManager::GetTextureByName("cheese_burger_plate");
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
    }
        break;

    case ItemType::BURGER_PLATE:
    {
        if (rhs.second == ItemType::C_CHEESE) {
            sprite->texture = AssetManager::GetTextureByName("cheese_burger_plate");
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
    }
        break;

    case ItemType::CHEESE_PATTY_PLATE:
    {
        if (rhs.second == ItemType::BUN) {
            sprite->texture = AssetManager::GetTextureByName("cheese_burger_plate");
            newItem = ItemType::CHEESE_BURGER_PLATE;
        }
    }
        break;


        // SALAD INGREDIENTS
        // Single ingredients can combine with empty or partial plates
    case ItemType::C_LETTUCE:
    {
        if (rhs.second == ItemType::WHITE_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_plate");
            newItem = ItemType::LETTUCE_PLATE;
        }
        else if (rhs.second == ItemType::TOMATO_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_tomato_plate");
            newItem = ItemType::LETTUCE_TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::SHRIMP_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_shrimp_plate");
            newItem = ItemType::LETTUCE_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::TOMATO_SHRIMP_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("salad_plate");
            newItem = ItemType::SALAD_PLATE;
        }
    }
        break;

    case ItemType::C_TOMATO:
    {
        if (rhs.second == ItemType::WHITE_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("tomato_plate");
            newItem = ItemType::TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::LETTUCE_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_tomato_plate");
            newItem = ItemType::LETTUCE_TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::SHRIMP_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("tomato_shrimp_plate");
            newItem = ItemType::TOMATO_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::LETTUCE_SHRIMP_PLATE)
        {
            sprite->texture = AssetManager::GetTextureByName("salad_plate");
            newItem = ItemType::SALAD_PLATE;
        }
    }
		break;

    case ItemType::C_SHRIMP:
	{
		if (rhs.second == ItemType::WHITE_PLATE)
		{
			sprite->texture = AssetManager::GetTextureByName("shrimp_plate");
			newItem = ItemType::SHRIMP_PLATE;
		}
		else if (rhs.second == ItemType::LETTUCE_PLATE)
		{
			sprite->texture = AssetManager::GetTextureByName("lettuce_shrimp_plate");
			newItem = ItemType::LETTUCE_SHRIMP_PLATE;
		}
		else if (rhs.second == ItemType::TOMATO_PLATE)
		{
			sprite->texture = AssetManager::GetTextureByName("tomato_shrimp_plate");
			newItem = ItemType::TOMATO_SHRIMP_PLATE;
		}
		else if (rhs.second == ItemType::LETTUCE_TOMATO_PLATE)
		{
			sprite->texture = AssetManager::GetTextureByName("salad_plate");
			newItem = ItemType::SALAD_PLATE;
		}
	}
	    break;

        // Partial plate combine with single ingredients
    case ItemType::LETTUCE_PLATE:
    {
        if (rhs.second == ItemType::C_TOMATO)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_tomato_plate");
            newItem = ItemType::LETTUCE_TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::C_SHRIMP)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_shrimp_plate");
            newItem = ItemType::LETTUCE_SHRIMP_PLATE;
        }
    }
        break;

    case ItemType::TOMATO_PLATE:
    {
        if (rhs.second == ItemType::C_LETTUCE)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_tomato_plate");
            newItem = ItemType::LETTUCE_TOMATO_PLATE;
        }
        else if (rhs.second == ItemType::C_SHRIMP)
        {
            sprite->texture = AssetManager::GetTextureByName("tomato_shrimp_plate");
            newItem = ItemType::TOMATO_SHRIMP_PLATE;
        }
    }
		break;

    case ItemType::SHRIMP_PLATE:
    {
        if (rhs.second == ItemType::C_LETTUCE)
        {
            sprite->texture = AssetManager::GetTextureByName("lettuce_shrimp_plate");
            newItem = ItemType::LETTUCE_SHRIMP_PLATE;
        }
        else if (rhs.second == ItemType::C_TOMATO)
        {
            sprite->texture = AssetManager::GetTextureByName("tomato_shrimp_plate");
            newItem = ItemType::TOMATO_SHRIMP_PLATE;
        }
    }
		break;

    case ItemType::LETTUCE_TOMATO_PLATE:
    {
        if (rhs.second == ItemType::C_SHRIMP) {
            sprite->texture = AssetManager::GetTextureByName("salad_plate");
            newItem = ItemType::SALAD_PLATE;
        }
    }
		break;

    case ItemType::LETTUCE_SHRIMP_PLATE:
    {
        if (rhs.second == ItemType::C_TOMATO) {
            sprite->texture = AssetManager::GetTextureByName("salad_plate");
            newItem = ItemType::SALAD_PLATE;
        }
    }
        break;

    case ItemType::TOMATO_SHRIMP_PLATE:
    {
        if (rhs.second == ItemType::C_LETTUCE) {
            sprite->texture = AssetManager::GetTextureByName("salad_plate");
            newItem = ItemType::SALAD_PLATE;
        }
    }
        break;

    default:
        break;
    }

    return std::make_pair(ObjectID, newItem);
}