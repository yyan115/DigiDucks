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

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<LettuceStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};
