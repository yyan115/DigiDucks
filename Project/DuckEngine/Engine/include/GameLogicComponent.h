#pragma once
#include <string>
#include <vector>
#include "Component.h"

class GameLogicComponent : public Component
{
public:
    std::vector<std::string> logicNames;

    GameLogicComponent() = default;

    GameLogicComponent(const std::vector<std::string>& logicNames)
        : logicNames(logicNames) {
    }

    GameLogicComponent(const std::string& logicName)
    {
        logicNames.push_back(logicName);
    }

    void AddLogic(const std::string& logicName)
    {
        if (std::find(logicNames.begin(), logicNames.end(), logicName) == logicNames.end())
        {
            logicNames.push_back(logicName);
        }
    }

    void RemoveLogic(const std::string& logicName)
    {
        auto it = std::remove(logicNames.begin(), logicNames.end(), logicName);
        if (it != logicNames.end())
        {
            logicNames.erase(it, logicNames.end());
        }
    }



    std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<GameLogicComponent>(*this);
    }
};
