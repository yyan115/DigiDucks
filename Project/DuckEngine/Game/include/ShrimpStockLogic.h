#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class ShrimpStockLogic : public StockLogic
{
public:

	ShrimpStockLogic() : StockLogic(IngredientType::SHRIMP, 5) {}

	ShrimpStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::SHRIMP, 5) {}

};
