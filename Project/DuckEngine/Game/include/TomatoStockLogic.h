/******************************************************************************/
/*!
\file       TomatoStockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 25 2024
\brief      Declartion of all Tomato Stock Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"
#include <iostream>

class TomatoStockLogic : public StockLogic
{
public:
    TomatoStockLogic() : StockLogic(ItemType::TOMATO, 3) {}

    TomatoStockLogic(GameLogicComponent* component) : StockLogic(component, ItemType::TOMATO, 3){}

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<TomatoStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};
