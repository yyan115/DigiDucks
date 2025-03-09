/******************************************************************************
\file       SnapshotManager.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Implements the SnapshotManager class, which provides functionality
            for managing undo and redo operations in the game editor. This
            includes taking scene snapshots, applying snapshots, and managing
            undo/redo history.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include <iostream>
#include "SnapshotManager.h"
#include "LevelManager.h"
#include "DuckEngine.h"
#include "ComponentFactory.h"

// Now using deques
std::deque<nlohmann::json> SnapshotManager::undoStack;
std::deque<nlohmann::json> SnapshotManager::redoStack;

/**************************************************************************
* @brief Saves the current scene state to the undo stack.
* @details Also clears the redo stack when a new undo state is saved.
**************************************************************************/
void SnapshotManager::SaveUndoState()
{
    nlohmann::json snapshot = TakeSceneSnapshot();

    while (!redoStack.empty())
    {
        redoStack.pop_back();
    }

    undoStack.push_back(snapshot);

    if (undoStack.size() > MAX_UNDO_STEPS)
    {
        undoStack.pop_front();
    }
}

/**************************************************************************
* @brief Performs an undo operation by reverting to the previous scene state.
* @details Saves the current scene state to the redo stack before applying
*          the top state from the undo stack.
**************************************************************************/
void SnapshotManager::Undo()
{
    if (undoStack.empty())
    {
        std::cout << "Undo stack is empty. Nothing to undo." << std::endl;
        return;
    }

    nlohmann::json currentState = TakeSceneSnapshot();
    redoStack.push_back(currentState);

    nlohmann::json lastState = undoStack.back();
    undoStack.pop_back();

    ApplySceneSnapshot(lastState);

    std::cout << "Undo performed. Undo stack size: " << undoStack.size() << std::endl;
}

/**************************************************************************
* @brief Performs a redo operation by reapplying a previously undone state.
* @details Saves the current scene state to the undo stack before applying
*          the top state from the redo stack.
**************************************************************************/
void SnapshotManager::Redo()
{
    if (redoStack.empty())
    {
        std::cout << "Redo stack is empty. Nothing to redo." << std::endl;
        return;
    }

    nlohmann::json currentState = TakeSceneSnapshot();
    undoStack.push_back(currentState);

    nlohmann::json nextState = redoStack.back();
    redoStack.pop_back();

    ApplySceneSnapshot(nextState);

    std::cout << "Redo performed. Redo stack size: " << redoStack.size() << std::endl;
}

/**************************************************************************
* @brief Clears both the undo and redo history stacks.
**************************************************************************/
void SnapshotManager::ClearHistory()
{
    undoStack.clear();
    redoStack.clear();

    std::cout << "Undo and redo history cleared." << std::endl;
}

/**************************************************************************
* @brief Takes a snapshot of the current scene state.
* @details Iterates over all entities in the scene and serializes their
*          properties and components into a JSON object.
* @return A JSON object representing the current scene state.
**************************************************************************/
nlohmann::json SnapshotManager::TakeSceneSnapshot()
{
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    nlohmann::json sceneSnapshot;
    sceneSnapshot["gameObjects"] = nlohmann::json::object();

    for (const auto& entity : entities)
    {
        nlohmann::json entityData;
        std::string entityName = entity->name.empty()
            ? "GameObject_" + std::to_string(entity->entityID)
            : entity->name;

        entityData["layer"] = entity->layerName;

        if (!entity->childNames.empty())
        {
            entityData["childNames"] = entity->childNames;
        }

        if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID))
        {
            entityData["position"]["x"] = transform->GetPosition().x;
            entityData["position"]["y"] = transform->GetPosition().y;
        }

        if (!entity->prefabName.empty())
        {
            entityData["prefab"] = entity->prefabName;
        }
        else
        {
            ComponentFactory::SaveComponentsToJson(entity->entityID, entityData["components"]);
        }

        sceneSnapshot["gameObjects"][entityName] = entityData;
    }

    return sceneSnapshot;
}

/**************************************************************************
* @brief Applies a scene snapshot to restore a specific state.
* @details Removes all entities from the current scene and recreates them
*          based on the data in the provided snapshot.
* @param snapshot The JSON object representing the scene state to apply.
**************************************************************************/
void SnapshotManager::ApplySceneSnapshot(const nlohmann::json& snapshot)
{
    DuckEngine::DUCKENGINE_EntityManager.RemoveAllEntities();

    for (auto& [entityName, entityData] : snapshot["gameObjects"].items())
    {
        Entity* entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity().get();
        entity->name = entityName;
        entity->layerName = entityData.value("layer", "Default");

        if (entityData.contains("childNames"))
        {
            entity->childNames = entityData["childNames"].get<std::vector<std::string>>();
        }

        if (entityData.contains("prefab"))
        {
            entity->prefabName = entityData["prefab"].get<std::string>();
            auto prefab = PrefabManager::GetPrefab(entity->prefabName);
            if (prefab)
            {
                ComponentFactory::AddComponentsToEntity(entity, prefab->componentsData);
            }
        }
        else
        {
            ComponentFactory::AddComponentsToEntity(entity, entityData["components"]);
        }

        if (entityData.contains("position"))
        {
            if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID))
            {
                transform->SetPosition(Serialization::GetVec2(entityData, "position", { 0.f, 0.f }));
            }
        }
    }

    auto& allEntities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : allEntities)
    {
        if (!entity->childNames.empty())
        {
            entity->childEntities.clear();
            for (const auto& childName : entity->childNames)
            {
                std::shared_ptr<Entity> childEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(childName);
                if (childEntity)
                {
                    entity->childEntities.push_back(childEntity);
                    auto* childTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(childEntity->entityID);
                    auto* parentTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID);
                    if (childTransform && parentTransform)
                    {
                        childTransform->localPosition = childTransform->worldPosition - parentTransform->worldPosition;
                    }
                }
            }
        }
    }
}

/**************************************************************************
* @brief Removes the most recent state from the undo stack.
**************************************************************************/
void SnapshotManager::RemoveLatestUndoState()
{
    if (!undoStack.empty())
    {
        undoStack.pop_back();
    }
}