/******************************************************************************/
/*!
\file       ShrimpStockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 25 2024
\brief      Declartion of all Shrimp Stock Logic functions

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

class ShrimpStockLogic : public StockLogic
{
public:

	ShrimpStockLogic() : StockLogic(ItemType::SHRIMP) {}

	ShrimpStockLogic(GameLogicComponent* component) : StockLogic(component, ItemType::SHRIMP) {}

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<ShrimpStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};
