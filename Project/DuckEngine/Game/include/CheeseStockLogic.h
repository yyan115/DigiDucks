#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class CheeseStockLogic : public StockLogic
{
public:

	CheeseStockLogic() : StockLogic(IngredientType::CHEESE, 5) {}

	CheeseStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::CHEESE, 5) {}

};
