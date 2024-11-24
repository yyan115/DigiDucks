#include <iostream>
#include "SnapshotManager.h"
#include "LevelManager.h"
#include "DuckEngine.h"
#include "ComponentFactory.h"

std::stack<nlohmann::json> SnapshotManager::undoStack;
std::stack<nlohmann::json> SnapshotManager::redoStack;

void SnapshotManager::SaveUndoState()
{
    nlohmann::json snapshot = TakeSceneSnapshot();
    undoStack.push(snapshot);

    if (undoStack.size() > MAX_UNDO_STEPS)
    {
        undoStack.pop();
    }

    while (!redoStack.empty())
    {
        redoStack.pop();
    }

    std::cout << "Saved undo state. Undo stack size: " << undoStack.size() << std::endl;
}

void SnapshotManager::Undo()
{
    if (undoStack.empty())
    {
        std::cout << "Undo stack is empty. Nothing to undo." << std::endl;
        return;
    }

    redoStack.push(TakeSceneSnapshot());

    nlohmann::json lastState = undoStack.top();
    undoStack.pop();

    ApplySceneSnapshot(lastState);

    std::cout << "Undo performed. Undo stack size: " << undoStack.size() << std::endl;
}

void SnapshotManager::Redo()
{
    if (redoStack.empty())
    {
        std::cout << "Redo stack is empty. Nothing to redo." << std::endl;
        return;
    }

    undoStack.push(TakeSceneSnapshot());

    nlohmann::json nextState = redoStack.top();
    redoStack.pop();

    ApplySceneSnapshot(nextState);

    std::cout << "Redo performed. Redo stack size: " << redoStack.size() << std::endl;
}

void SnapshotManager::ClearHistory()
{
    while (!undoStack.empty())
    {
        undoStack.pop();
    }

    while (!redoStack.empty())
    {
        redoStack.pop();
    }

    std::cout << "Undo and redo history cleared." << std::endl;
}

nlohmann::json SnapshotManager::TakeSceneSnapshot()
{
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    nlohmann::json sceneSnapshot;
    sceneSnapshot["gameObjects"] = nlohmann::json::object();

    for (const auto& entity : entities)
    {
        nlohmann::json entityData;
        entityData["name"] = entity.name.empty() ? "Entity_" + std::to_string(entity.entityID) : entity.name;
        entityData["layer"] = entity.layerName;

        ComponentFactory::SaveComponentsToJson(entity.entityID, entityData["components"]);

        sceneSnapshot["gameObjects"][entityData["name"]] = entityData;
    }

    return sceneSnapshot;
}


void SnapshotManager::ApplySceneSnapshot(const nlohmann::json& snapshot)
{
    DuckEngine::DUCKENGINE_EntityManager.RemoveAllEntities();

    for (auto& [entityName, entityData] : snapshot["gameObjects"].items())
    {
        Entity* entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
        entity->name = entityName;
        entity->layerName = entityData.value("layer", "Default");

        ComponentFactory::AddComponentsToEntity(entity, entityData["components"]);
    }

    std::cout << "Scene snapshot applied successfully." << std::endl;
}
