#pragma once
#include <string>
#include "Component.h"

class GameLogicComponent : public Component
{
public:
    GameLogicComponent(const std::string& logicName = "")
        : logicName(logicName) {}

    std::string logicName;

    std::shared_ptr<Component> Clone() const
    {
        return std::make_shared<GameLogicComponent>(*this);
    }
};
