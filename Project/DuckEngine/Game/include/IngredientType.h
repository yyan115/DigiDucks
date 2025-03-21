/******************************************************************************/
/*!
\file       IngredientType.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Holds the ItemType enum class, which defines all the types of
			ingredients and dishes in the game. This enum is used to identify
			the type of an object in the game, such as a burger, salad, or
			individual ingredient.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

enum class ItemType : int
{
    // Ingredient Types
    EMPTY = -2,
    BIN = -1,

	// Ingredients
    BUN,
    CHEESE,
    LETTUCE,
    MUSHROOM,
	SHRIMP,
    STEAK,
    TOMATO,

    // Cutted Ingredients
    C_CHEESE,				// Cut Cheese
    C_LETTUCE,				// Cut Lettuce
    C_MUSHROOM,				// Cut Mushroom
    C_SHRIMP,				// Cut Shrimp
    C_TOMATO,				// Cut Tomato

    R_PATTY,				// Raw Patty
    C_PATTY,				// Cooked Patty
	B_PATTY,				// Burnt Patty

	// Pan
	PAN,					// Empty Pan
	PAN_R_PATTY,			// Pan with Raw Patty
	PAN_C_PATTY,			// Pan with Cooked Patty
	PAN_B_PATTY,			// Pan with Burnt Patty

	// Pot
	POT,					// Empty Pot
	POT_MUSHROOM,			// Pot with Mushroom
	POT_TOMATO,				// Pot with Tomato
	POT_SUS,				// Pot with Suspicious Soup

	//Plate
    WHITE_PLATE,			// Empty Plate

    // Burger Types
    BUN_PLATE,				// Plate with Bun
    CHEESE_PLATE,			// Plate with Cheese
    PATTY_PLATE,			// Plate with Patty
    BUN_CHEESE_PLATE,		// Plate with Bun and Cheese
    BURGER_PLATE,			// Plate with Bun and Patty
    CHEESE_PATTY_PLATE,		// Plate with Cheese and Patty
    CHEESE_BURGER_PLATE,	// Plate with Bun, Cheese, and Patty

    // Salad Types
    LETTUCE_PLATE,			// Plate with Lettuce
    SHRIMP_PLATE,			// Plate with Shrimp
    TOMATO_PLATE,			// Plate with Tomato
    LETTUCE_SHRIMP_PLATE,	// Plate with Lettuce and Shrimp
    LETTUCE_TOMATO_PLATE,	// Plate with Lettuce and Tomato
    TOMATO_SHRIMP_PLATE,	// Plate with Tomato and Shrimp
    SALAD_PLATE,			// Plate with Lettuce, Tomato, and Shrimp

	// Soup
	BOWL,					// Empty Bowl
	BOWL_MUSHROOM,			// Bowl with Mushroom
	BOWL_TOMATO,			// Bowl with Tomato
	BOWL_SUS				// Bowl with Suspicious Soup

};

// Get the string representation of the ItemType
// Can be used for debugging or getting image file names

/****************************************************************
* @brief Get the string representation of the ItemType,
*        used for debugging or getting image file names
* 
* @param type - the type of item
* 
* @return - the string representation of the ItemType
* ****************************************************************/
inline std::string whatType(ItemType type)
{
    switch (type)
    {
	case ItemType::EMPTY:
		return "EMPTY";
	case ItemType::BIN:
		return "BIN";

	case ItemType::BUN:
		return "bun";
	case ItemType::CHEESE:
		return "cheese";
	case ItemType::LETTUCE:
		return "lettuce";
	case ItemType::MUSHROOM:
		return "mushroom";
	case ItemType::SHRIMP:
		return "shrimp";
	case ItemType::STEAK:
		return "steak";
	case ItemType::TOMATO:
		return "tomato";

	case ItemType::C_CHEESE:    
		return "cheese_chop";
	case ItemType::C_LETTUCE:
		return "lettuce_chop";
	case ItemType::C_MUSHROOM:
		return "mushroom_chop";
	case ItemType::C_SHRIMP:
		return "shrimp_chop";
	case ItemType::C_TOMATO:
		return "tomato_chop";

	case ItemType::R_PATTY:
		return "patty";
	case ItemType::C_PATTY:
		return "cooked_patty";
	case ItemType::B_PATTY:
		return "burnt_patty";

	case ItemType::PAN:
		return "pan";
	case ItemType::PAN_R_PATTY:
		return "pan_raw";
	case ItemType::PAN_C_PATTY:
		return "pan_cooked";
	case ItemType::PAN_B_PATTY:
		return "pan_burnt";

	case ItemType::POT:
		return "pot";
	case ItemType::POT_MUSHROOM:
		return "pot_mushroom";
	case ItemType::POT_TOMATO:
		return "pot_tomato";
	case ItemType::POT_SUS:
		return "pot_sus";

	case ItemType::BUN_PLATE:
		return "bun_plate";
	case ItemType::CHEESE_PLATE:
		return "cheese_plate";
	case ItemType::PATTY_PLATE:
		return "cooked_patty_plate";
	case ItemType::BUN_CHEESE_PLATE:
		return "bun_cheese_plate";
	case ItemType::BURGER_PLATE:
		return "burger_plate";
	case ItemType::CHEESE_PATTY_PLATE:
		return "cheese_patty_plate";
	case ItemType::CHEESE_BURGER_PLATE:
		return "cheese_burger_plate";

	case ItemType::WHITE_PLATE:
		return "white_plate";
	case ItemType::LETTUCE_PLATE:
		return "lettuce_plate";
	case ItemType::SHRIMP_PLATE:
		return "shrimp_plate";
	case ItemType::TOMATO_PLATE:
		return "tomato_plate";
	case ItemType::LETTUCE_SHRIMP_PLATE:
		return "lettuce_shrimp_plate";
	case ItemType::LETTUCE_TOMATO_PLATE:
		return "lettuce_tomato_plate";
	case ItemType::TOMATO_SHRIMP_PLATE:
		return "tomato_shrimp_plate";
	case ItemType::SALAD_PLATE:
		return "salad_plate";

	case ItemType::BOWL:
		return "bowl";
	case ItemType::BOWL_MUSHROOM:
		return "bowl_mushroom";
	case ItemType::BOWL_TOMATO:
		return "bowl_tomato";
	case ItemType::BOWL_SUS:
		return "bowl_sus";

	default:
		return "UNKNOWN";
    };
}


/****************************************************************
* @brief Check if the item is an ingredient
* 
* @param type - the type of item
* 
* @return - true if the item is an ingredient, false otherwise
* ****************************************************************/
inline bool isIngredient(ItemType type)
{
	switch (type)
	{
	case ItemType::BUN:
	case ItemType::CHEESE:
	case ItemType::C_CHEESE:
	case ItemType::LETTUCE:
	case ItemType::C_LETTUCE:
	case ItemType::MUSHROOM:
	case ItemType::C_MUSHROOM:
	case ItemType::SHRIMP:
	case ItemType::C_SHRIMP:
	case ItemType::TOMATO:
	case ItemType::C_TOMATO:
	case ItemType::STEAK:
	case ItemType::R_PATTY:
	case ItemType::C_PATTY:
	case ItemType::B_PATTY:
		return true;
		break;
	default:
		return false;
		break;
	}
}

/****************************************************************
* @brief Check if the item is a plate
* 
* @param type - the type of item
* 
* @return - true if the item is a plate, false otherwise
* ****************************************************************/
inline bool isPlate(ItemType type)
{
	switch (type)
	{
	case ItemType::WHITE_PLATE:
	case ItemType::BUN_PLATE:
	case ItemType::CHEESE_PLATE:
	case ItemType::PATTY_PLATE:
	case ItemType::BUN_CHEESE_PLATE:
	case ItemType::BURGER_PLATE:
	case ItemType::CHEESE_PATTY_PLATE:
	case ItemType::CHEESE_BURGER_PLATE:

	case ItemType::LETTUCE_PLATE:
	case ItemType::SHRIMP_PLATE:
	case ItemType::TOMATO_PLATE:
	case ItemType::LETTUCE_SHRIMP_PLATE:
	case ItemType::LETTUCE_TOMATO_PLATE:
	case ItemType::TOMATO_SHRIMP_PLATE:
	case ItemType::SALAD_PLATE:
		return true;
		break;
	default:
		return false;
		break;
	}
}

/****************************************************************
* @brief Check if the item is a bowl
* 
* @param type - the type of item
* 
* @return - true if the item is a bowl, false otherwise
* ****************************************************************/
inline bool isBowl(ItemType type)
{
	switch (type)
	{
	case ItemType::BOWL:
	case ItemType::BOWL_MUSHROOM:
	case ItemType::BOWL_TOMATO:
	case ItemType::BOWL_SUS:
		return true;
		break;
	default:
		return false;
		break;
	}
}

/****************************************************************
* @brief Check if the item is a pan
* 
* @param type - the type of item
* 
* @return - true if the item is pan, false otherwise
* ****************************************************************/
inline bool isTypePan(ItemType type)
{
	switch (type)
	{
	case ItemType::PAN:
	case ItemType::PAN_R_PATTY:
	case ItemType::PAN_C_PATTY:
	case ItemType::PAN_B_PATTY:
		return true;
		break;
	default:
		return false;
		break;
	}
}

/****************************************************************
* @brief Check if the item is a pot
* 
* @param type - the type of item
* 
* @return - true if the item is pot, false otherwise
* ****************************************************************/
inline bool isTypePot(ItemType type)
{
	switch (type)
	{
	case ItemType::POT:
	case ItemType::POT_MUSHROOM:
	case ItemType::POT_TOMATO:
	case ItemType::POT_SUS:
		return true;
		break;
	default:
		return false;
		break;
	}
}

/****************************************************************
* @brief Check if the item is part of/a equipment
* 
* @param type - the type of item
* 
* @return - true if the item is a pan or pot, false otherwise
* ****************************************************************/
inline bool isEquipment(ItemType type)
{
	switch (type)
	{
	case ItemType::PAN:
	case ItemType::PAN_R_PATTY:
	case ItemType::PAN_C_PATTY:
	case ItemType::PAN_B_PATTY:
	case ItemType::POT:
	case ItemType::POT_MUSHROOM:
	case ItemType::POT_TOMATO:
	case ItemType::POT_SUS:
		return true;
		break;
	default:
		return false;
		break;
	}
}