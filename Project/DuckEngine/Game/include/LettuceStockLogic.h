#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class LettuceStockLogic : public StockLogic
{
public:

	LettuceStockLogic() : StockLogic(IngredientType::LETTUCE, 5) {}

	LettuceStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::LETTUCE, 5) {}

};
