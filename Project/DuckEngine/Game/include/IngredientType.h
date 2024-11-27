#pragma once

enum class ItemType
{
    // Ingredient Types
    EMPTY,
    BIN,
    BUN,
    CHEESE,
    LETTUCE,
    MUSHROOM,
    SHRIMP,
    STEAK,
    TOMATO,

    // Cooked Ingredients
    C_CHEESE,              // Cut Cheese
    C_LETTUCE,             // Cut Lettuce
    C_MUSHROOM,            // Cut Mushroom
    C_SHRIMP,              // Cut Shrimp
    R_PATTY,               // Raw Patty
    C_PATTY,               // Cooked Patty
    C_TOMATO,              // Cut Tomato

    // Burger Types
    GREY_PLATE,            // Empty Plate
    BUN_PLATE,             // Plate with Bun
    CHEESE_PLATE,          // Plate with Cheese
    PATTY_PLATE,           // Plate with Patty
    BUN_CHEESE_PLATE,      // Plate with Bun and Cheese
    BURGER_PLATE,       // Plate with Bun and Patty
    CHEESE_PATTY_PLATE,    // Plate with Cheese and Patty
    CHEESE_BURGER_PLATE,// Plate with Bun, Cheese, and Patty

    // Salad Types
    WHITE_PLATE,           // Empty Plate
    LETTUCE_PLATE,         // Plate with Lettuce
    SHRIMP_PLATE,          // Plate with Shrimp
    TOMATO_PLATE,          // Plate with Tomato
    LETTUCE_SHRIMP_PLATE,  // Plate with Lettuce and Shrimp
    LETTUCE_TOMATO_PLATE,  // Plate with Lettuce and Tomato
    TOMATO_SHRIMP_PLATE,   // Plate with Tomato and Shrimp
    SALAD_PLATE // Plate with Lettuce, Tomato, and Shrimp
};