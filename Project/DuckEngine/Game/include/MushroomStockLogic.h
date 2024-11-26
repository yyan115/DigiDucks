#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class MushroomStockLogic : public StockLogic
{
public:

	MushroomStockLogic() : StockLogic(IngredientType::MUSHROOM, 5) {}

	MushroomStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::MUSHROOM, 5) {}

};
