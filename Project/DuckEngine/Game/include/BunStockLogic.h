#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class BunStockLogic : public StockLogic
{
public:

	BunStockLogic() : StockLogic(IngredientType::BUN, 5) {}

	BunStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::BUN, 5) {}

};