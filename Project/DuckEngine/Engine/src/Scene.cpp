/******************************************************************************
\file       Scene.cpp
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Implementation of the Scene class, including methods to manage
            scene layers, entities, and lifecycle events (load, update,
            unload, etc.).

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once

#include <vector>
#include "Scene.h"
#include "DuckEngine.h"
#include "SoundSystem.h"

/**************************************************************************
@brief Loads resources and initializes the scene.
**************************************************************************/
void Scene::Load() {}

/**************************************************************************
@brief Called at the start of the scene to initialize components or settings.
**************************************************************************/
void Scene::Start() 
{
    DuckEngine::DUCKENGINE_SystemManager.StartAll();
}

/**************************************************************************
@brief Updates the scene every frame.
**************************************************************************/
void Scene::Update() {}

/**************************************************************************
@brief Called after each update, typically for post-update tasks.
**************************************************************************/
void Scene::PostUpdate() {}

/**************************************************************************
@brief Handles scene exit procedures, including cleanup.
**************************************************************************/
void Scene::Exit() {}

/**************************************************************************
@brief Unloads resources and clears entities from the scene.
**************************************************************************/
void Scene::Unload()
{
    std::vector<std::shared_ptr<Entity>> entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    std::vector<int> entityIDs;
    for (const auto& entity : entities) 
    {
        entityIDs.push_back(entity->entityID);
    }

    SoundSystem::StopAllSounds();

    for (int entityID : entityIDs)
    {
        DuckEngine::DUCKENGINE_ComponentManager.RemoveAllComponents(entityID);
        DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(entityID);
    }

    DuckEngine::SetCameraPosition(0.0f, 0.0f);

    DuckEngine::DUCKENGINE_EntityManager.ResetEntityID();
    GameLogicManager::Clear();
}

/**************************************************************************
@brief Adds a new layer to the scene if it doesn’t already exist.
@param name The name of the layer.
@param layer The Layer object to add.
**************************************************************************/
void Scene::AddLayer(const std::string& name, Layer layer)
{
    if (layers.find(name) == layers.end())
    {
        layers[name] = layer;
    }
}

/**************************************************************************
@brief Retrieves a pointer to a layer by name.
@param name The name of the layer to retrieve.
@return Pointer to the layer if found, nullptr otherwise.
**************************************************************************/
Layer* Scene::GetLayer(const std::string& name)
{
    auto it = layers.find(name);
    if (it != layers.end())
    {
        return &(it->second);
    }
    return nullptr;
}

/**************************************************************************
@brief Gets a constant reference to the layers map in the scene.
@return An unordered_map containing the layers in the scene.
**************************************************************************/
const std::unordered_map<std::string, Layer>& Scene::GetLayers() const
{
    return layers;
}

/**************************************************************************
@brief Adds an entity to a specified layer within the scene.
@param layerName The name of the layer.
@param entity Pointer to the entity to add.
**************************************************************************/
void Scene::AddEntityToLayer(const std::string& layerName, Entity* entity)
{
    //auto it = layers.find(layerName);
    //if (it == layers.end())
    //{
    //    layers[layerName] = Layer(layerName);
    //}

    layers[layerName].AddEntity(entity);
}

void Scene::RemoveEntityFromLayer(const std::string& layerName, int entityID)
{
    if (layers.find(layerName) != layers.end())
    {
        layers[layerName].RemoveEntityByID(entityID);
    }
}
