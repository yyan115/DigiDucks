/******************************************************************************/
/*!
\file       GreyPlateStockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 25 2024
\brief      Declartion of all Grey Plate Stock Logic functions

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

class GreyPlateStockLogic : public StockLogic
{
public:

    GreyPlateStockLogic() : StockLogic(ItemType::GREY_PLATE, 10000) {}

    GreyPlateStockLogic(GameLogicComponent* component) : StockLogic(component, ItemType::GREY_PLATE, 10000) {}

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<GreyPlateStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};