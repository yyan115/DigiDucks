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
        auto logic = GameLogicManager::GetLogic(logicComponent->logicName);

        if (logic)
        {
            logic->Start();
        }
    }
}

void GameLogicSystem::Update()
{
    auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

    for (auto& [entityID, component] : components)
    {
        auto* logicComponent = static_cast<GameLogicComponent*>(component.get());
        auto logic = GameLogicManager::GetLogic(logicComponent->logicName);

        if (logic)
        {
            logic->Update();
        }
    }
}
