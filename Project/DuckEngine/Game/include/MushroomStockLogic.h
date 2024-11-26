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

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<MushroomStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};
