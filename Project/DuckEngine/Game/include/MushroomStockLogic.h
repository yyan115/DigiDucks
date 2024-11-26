#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class MushroomStock : public StockLogic
{
public:

	MushroomStock() : StockLogic(IngredientType::MUSHROOM, 5) {}

	MushroomStock(GameLogicComponent* component) : StockLogic(component, IngredientType::MUSHROOM, 5) {}

};
