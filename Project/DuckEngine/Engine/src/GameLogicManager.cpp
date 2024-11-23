#include "GameLogicManager.h"

std::unordered_map<std::string, std::shared_ptr<GameLogic>> GameLogicManager::logicMap;

void GameLogicManager::AddLogic(const std::string& name, std::shared_ptr<GameLogic> logic)
{
    logicMap[name] = logic;
}

std::shared_ptr<GameLogic> GameLogicManager::GetLogic(const std::string& name)
{
    if (logicMap.find(name) != logicMap.end())
    {
        return logicMap[name];
    }
    return nullptr;
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
