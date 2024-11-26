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
            auto baseLogic = GameLogicManager::GetLogic(logicName);
            if (baseLogic)
            {
                auto logic = baseLogic->Clone();

                logic->SetComponent(logicComponent);
                logic->Start();
                GameLogicManager::AddLogicToEntity(entityID, logic);
            }
        }
    }
}

void GameLogicSystem::Update()
{
    auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

    for (auto& [entityID, component] : components)
    {
        auto logics = GameLogicManager::GetAllLogicsForEntity(entityID);
        for (auto& logic : logics)
        {
            if (logic)
            {
                logic->Update();
            }
        }
    }
}
void GameLogicSystem::FixedUpdate()
{
    auto& components = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<GameLogicComponent>();

    for (auto& [entityID, component] : components)
    {
        auto logics = GameLogicManager::GetAllLogicsForEntity(entityID);
        for (auto& logic : logics)
        {
            if (logic)
            {
                logic->FixedUpdate();
            }
        }
    }
}