#include "GameLogicManager.h"

std::unordered_map<std::string, std::shared_ptr<GameLogic>> GameLogicManager::logicMap;
std::unordered_map<int, std::vector<std::shared_ptr<GameLogic>>> GameLogicManager::entityLogicMap;

void GameLogicManager::AddLogic(const std::string& name, std::shared_ptr<GameLogic> logic)
{
    logicMap[name] = logic;
}

void GameLogicManager::AddLogicToEntity(int entityID, std::shared_ptr<GameLogic> logic)
{
    entityLogicMap[entityID].push_back(logic);
}

std::shared_ptr<GameLogic> GameLogicManager::GetLogic(const std::string& name)
{
    if (logicMap.find(name) != logicMap.end())
    {
        return logicMap[name];
    }
    return nullptr;
}

std::vector<std::shared_ptr<GameLogic>> GameLogicManager::GetAllLogicsForEntity(int entityID)
{
    if (entityLogicMap.find(entityID) != entityLogicMap.end())
    {
        return entityLogicMap[entityID];
    }
    return {};
}

void GameLogicManager::RemoveLogicsForEntity(int entityID)
{
    if (entityLogicMap.find(entityID) != entityLogicMap.end())
    {
        entityLogicMap.erase(entityID);
    }
}

void GameLogicManager::StartAll()
{
    for (auto& [name, logic] : logicMap)
    {
        logic->Start();
    }
}

void GameLogicManager::UpdateAll()
{
    for (auto& [name, logic] : logicMap)
    {
        logic->Update();
    }
}
