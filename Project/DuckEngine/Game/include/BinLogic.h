#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class BinLogic : public StockLogic
{
public:

	BinLogic() : StockLogic(IngredientType::EMPTY, 5) {}

	BinLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::EMPTY, 5) {}

};