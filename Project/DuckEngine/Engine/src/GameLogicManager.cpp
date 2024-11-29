/******************************************************************************/
/*!
\file       GameLogicManager.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Implements the GameLogicManager class, which manages a collection
            of game logic objects and their associations with entities.
            Provides functionality to add, retrieve, update, and clear game
            logic components for entities in a game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameLogicManager.h"

std::unordered_map<std::string, std::shared_ptr<GameLogic>> GameLogicManager::logicMap;
std::unordered_map<int, std::vector<std::shared_ptr<GameLogic>>> GameLogicManager::entityLogicMap;

/**************************************************************************
* @brief Adds a logic object to the global logic map.
* @param name The name of the logic to add.
* @param logic A shared pointer to the GameLogic instance.
**************************************************************************/
void GameLogicManager::AddLogic(const std::string& name, std::shared_ptr<GameLogic> logic)
{
    logicMap[name] = logic;
}

/**************************************************************************
* @brief Associates a logic object with a specific entity.
* @param entityID The ID of the entity.
* @param logic A shared pointer to the GameLogic instance.
**************************************************************************/
void GameLogicManager::AddLogicToEntity(int entityID, std::shared_ptr<GameLogic> logic)
{
    auto& logics = entityLogicMap[entityID];

    if (std::find(logics.begin(), logics.end(), logic) == logics.end())
    {
        logics.push_back(logic);
    }
    else
    {
    }
}

/**************************************************************************
* @brief Retrieves a logic object by its name.
* @param name The name of the logic to retrieve.
* @return A shared pointer to the GameLogic instance, or nullptr if not found.
**************************************************************************/
std::shared_ptr<GameLogic> GameLogicManager::GetLogic(const std::string& name)
{
    if (logicMap.find(name) != logicMap.end())
    {
        return logicMap[name];
    }
    return nullptr;
}

/**************************************************************************
* @brief Retrieves all logic objects associated with a specific entity.
* @param entityID The ID of the entity.
* @return A vector of shared pointers to the GameLogic instances.
**************************************************************************/
std::vector<std::shared_ptr<GameLogic>> GameLogicManager::GetAllLogicsForEntity(int entityID)
{
    if (entityLogicMap.find(entityID) != entityLogicMap.end())
    {
        return entityLogicMap[entityID];
    }
    return {};
}

/**************************************************************************
* @brief Removes all logic objects associated with a specific entity.
* @param entityID The ID of the entity.
**************************************************************************/
void GameLogicManager::RemoveLogicsForEntity(int entityID)
{
    if (entityLogicMap.find(entityID) != entityLogicMap.end())
    {
        entityLogicMap.erase(entityID);
    }
}

/**************************************************************************
* @brief Calls the Start method on all logic objects in the global logic map.
**************************************************************************/
void GameLogicManager::StartAll()
{
    for (auto& [name, logic] : logicMap)
    {
        logic->Start();
    }
}

/**************************************************************************
* @brief Calls the Update method on all logic objects in the global logic map.
**************************************************************************/
void GameLogicManager::UpdateAll()
{
    for (auto& [name, logic] : logicMap)
    {
        logic->Update();
    }
}

/**************************************************************************
* @brief Clears all logic objects and their associations with entities.
**************************************************************************/
void GameLogicManager::Clear()
{
    for (auto& [entityID, logics] : entityLogicMap)
    {
        for (auto& logic : logics)
        {
            //std::cout << "Clearing logic for Entity ID: " << entityID << std::endl;
            logic.reset();
        }
    }
    entityLogicMap.clear();

}