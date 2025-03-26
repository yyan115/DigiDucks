/******************************************************************************/
/*!
\file       BunStockLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 25 2024
\brief      Declartion of all Bun Stock Logic functions

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

class BunStockLogic : public StockLogic
{
public:

	BunStockLogic() : StockLogic(ItemType::BUN) {}

	BunStockLogic(GameLogicComponent* component) : StockLogic(component, ItemType::BUN) {}

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<BunStockLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

};