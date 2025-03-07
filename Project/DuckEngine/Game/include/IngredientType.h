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

enum class ItemType
{
    // Ingredient Types
    EMPTY = -2,
    BIN = -1,

	/* Reset to 0 when Sprites are done*/
    LETTUCE = 0,	// Change back to 2
	SHRIMP = 1,		// Change back to 4
    TOMATO = 2,		// Change back to 6
    MUSHROOM,
    BUN,
    STEAK,
    CHEESE,

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

    // Burger Types
    GREY_PLATE,				// Empty Plate
    BUN_PLATE,				// Plate with Bun
    CHEESE_PLATE,			// Plate with Cheese
    PATTY_PLATE,			// Plate with Patty
    BUN_CHEESE_PLATE,		// Plate with Bun and Cheese
    BURGER_PLATE,			// Plate with Bun and Patty
    CHEESE_PATTY_PLATE,		// Plate with Cheese and Patty
    CHEESE_BURGER_PLATE,	// Plate with Bun, Cheese, and Patty

    // Salad Types
    WHITE_PLATE,			// Empty Plate
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


inline std::string whatType(ItemType type)
{
    switch (type)
    {
	case ItemType::EMPTY:
		return "EMPTY";
	case ItemType::BIN:
		return "BIN";
	case ItemType::BUN:
		return "BUN";
	case ItemType::CHEESE:
		return "CHEESE";
	case ItemType::LETTUCE:
		return "LETTUCE";
	case ItemType::MUSHROOM:
		return "MUSHROOM";
	case ItemType::SHRIMP:
		return "SHRIMP";
	case ItemType::STEAK:
		return "STEAK";
	case ItemType::TOMATO:
		return "TOMATO";

	case ItemType::C_CHEESE:    
		return "C_CHEESE";
	case ItemType::C_LETTUCE:
		return "C_LETTUCE";
	case ItemType::C_MUSHROOM:
		return "C_MUSHROOM";
	case ItemType::C_SHRIMP:
		return "C_SHRIMP";
	case ItemType::R_PATTY:
		return "R_PATTY";
	case ItemType::C_TOMATO:
		return "C_TOMATO";

	case ItemType::C_PATTY:
		return "C_PATTY";
	case ItemType::B_PATTY:
		return "B_PATTY";

	case ItemType::PAN:
		return "PAN";
	case ItemType::PAN_C_PATTY:
		return "PAN_C_PATTY";
	case ItemType::PAN_B_PATTY:
		return "PAN_B_PATTY";

	case ItemType::POT:
		return "POT";
	case ItemType::POT_MUSHROOM:
		return "POT_MUSHROOM";
	case ItemType::POT_TOMATO:
		return "POT_TOMATO";
	case ItemType::POT_SUS:
		return "POT_SUS";

	case ItemType::GREY_PLATE:
		return "GREY_PLATE";
	case ItemType::BUN_PLATE:
		return "BUN_PLATE";
	case ItemType::CHEESE_PLATE:
		return "CHEESE_PLATE";
	case ItemType::PATTY_PLATE:
		return "PATTY_PLATE";
	case ItemType::BUN_CHEESE_PLATE:
		return "BUN_CHEESE_PLATE";
	case ItemType::BURGER_PLATE:
		return "BURGER_PLATE";
	case ItemType::CHEESE_PATTY_PLATE:
		return "CHEESE_PATTY_PLATE";
	case ItemType::CHEESE_BURGER_PLATE:
		return "CHEESE_BURGER_PLATE";

	case ItemType::WHITE_PLATE:
		return "WHITE_PLATE";
	case ItemType::LETTUCE_PLATE:
		return "LETTUCE_PLATE";
	case ItemType::SHRIMP_PLATE:
		return "SHRIMP_PLATE";
	case ItemType::TOMATO_PLATE:
		return "TOMATO_PLATE";
	case ItemType::LETTUCE_SHRIMP_PLATE:
		return "LETTUCE_SHRIMP_PLATE";
	case ItemType::LETTUCE_TOMATO_PLATE:
		return "LETTUCE_TOMATO_PLATE";
	case ItemType::TOMATO_SHRIMP_PLATE:
		return "TOMATO_SHRIMP_PLATE";
	case ItemType::SALAD_PLATE:
		return "SALAD_PLATE";

	case ItemType::BOWL:
		return "BOWL";
	case ItemType::BOWL_MUSHROOM:
		return "BOWL_MUSHROOM";
	case ItemType::BOWL_TOMATO:
		return "BOWL_TOMATO";
	case ItemType::BOWL_SUS:
		return "BOWL_SUS";

	default:
		return "UNKNOWN";
    };
}

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