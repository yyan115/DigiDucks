#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class BinLogic : public StockLogic
{
public:

	BinLogic() : StockLogic(IngredientType::BIN, 5) {}

	BinLogic(GameLogicComponent* component) : StockLogic(component, IngredientType::BIN, 5) {}

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<BinLogic>(*this);
        clone->component = nullptr;
        return clone;
    }


};