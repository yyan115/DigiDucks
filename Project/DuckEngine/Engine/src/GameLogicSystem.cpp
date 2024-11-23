#include "GameLogicSystem.h"
#include "DuckEngine.h"
#include "GameLogicManager.h"
#include "GameLogicComponent.h"

void GameLogicSystem::Start()
{
    auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

    for (auto& [entityID, component] : components)
    {
        auto* logicComponent = static_cast<GameLogicComponent*>(component.get());

        for (const auto& logicName : logicComponent->logicNames)
        {
            auto logic = GameLogicManager::GetLogic(logicName);
            if (logic)
            {
                logic->SetComponent(logicComponent);
                logic->Start();
            }
        }
    }
}


void GameLogicSystem::Update()
{
    auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

    for (auto& [entityID, component] : components)
    {
        auto* logicComponent = static_cast<GameLogicComponent*>(component.get());

        for (const auto& logicName : logicComponent->logicNames)
        {
            auto logic = GameLogicManager::GetLogic(logicName);
            if (logic)
            {
                logic->Update();
            }
        }
    }
}