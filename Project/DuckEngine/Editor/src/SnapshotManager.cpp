/******************************************************************************/
/*!
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
*/
/******************************************************************************/

#include <iostream>
#include "SnapshotManager.h"
#include "LevelManager.h"
#include "DuckEngine.h"
#include "ComponentFactory.h"

std::stack<nlohmann::json> SnapshotManager::undoStack;
std::stack<nlohmann::json> SnapshotManager::redoStack;

/**************************************************************************
* @brief Saves the current scene state to the undo stack.
* @details Also clears the redo stack when a new undo state is saved.
**************************************************************************/
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

    redoStack.push(TakeSceneSnapshot());

    nlohmann::json lastState = undoStack.top();
    undoStack.pop();

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

    undoStack.push(TakeSceneSnapshot());

    nlohmann::json nextState = redoStack.top();
    redoStack.pop();

    ApplySceneSnapshot(nextState);

    std::cout << "Redo performed. Redo stack size: " << redoStack.size() << std::endl;
}

/**************************************************************************
* @brief Clears both the undo and redo history stacks.
**************************************************************************/
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
        entityData["name"] = entity.get()->name.empty() ? "Entity_" + std::to_string(entity.get()->entityID) : entity.get()->name;
        entityData["layer"] = entity.get()->layerName;

        ComponentFactory::SaveComponentsToJson(entity.get()->entityID, entityData["components"]);

        sceneSnapshot["gameObjects"][entityData["name"]] = entityData;
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

        ComponentFactory::AddComponentsToEntity(entity, entityData["components"]);
    }

    std::cout << "Scene snapshot applied successfully." << std::endl;
}

/**************************************************************************
* @brief Removes the most recent state from the undo stack.
**************************************************************************/
void SnapshotManager::RemoveLatestUndoState()
{
    if (!undoStack.empty())
    {
        undoStack.pop();
    }
}
