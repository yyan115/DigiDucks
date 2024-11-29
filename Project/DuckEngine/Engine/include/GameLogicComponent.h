/******************************************************************************/
/*!
\file       GameLogicComponent.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Implements the GameLogicComponent class, which extends the
            Component base class. This class manages a collection of logic names,
            providing functionality for adding, removing, and cloning logic
            components for a game entity.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

    /**************************************************************************
    * @brief Adds a logic name to the component if it does not already exist.
    * @param logicName The logic name to add.
    **************************************************************************/
    void AddLogic(const std::string& logicName)
    {
        if (std::find(logicNames.begin(), logicNames.end(), logicName) == logicNames.end())
        {
            logicNames.push_back(logicName);
        }
    }

    /**************************************************************************
    * @brief Removes a logic name from the component if it exists.
    * @param logicName The logic name to remove.
    **************************************************************************/
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
