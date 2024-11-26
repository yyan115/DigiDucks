#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class TomatoStockLogic : public StockLogic
{
public:

	TomatoStockLogic() : StockLogic(IngredientType::TOMATO, 5) {}

	TomatoStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::TOMATO, 5) {}

};
