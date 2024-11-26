#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class SteakStockLogic : public StockLogic
{
public:

	SteakStockLogic() : StockLogic(IngredientType::STEAK, 5) {}

	SteakStockLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::STEAK, 5) {}

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<SteakStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};
