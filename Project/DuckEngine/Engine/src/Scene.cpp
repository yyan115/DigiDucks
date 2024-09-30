#pragma once

#include <vector>
#include "Scene.h"
#include "DuckEngine.h"

void Scene::Load() {}
void Scene::Start() {}
void Scene::Update() {}
void Scene::PostUpdate() {}
void Scene::Exit() {}
void Scene::Unload() 
{
    std::vector<Entity>& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    std::vector<int> entityIDs;
    for (Entity& entity : entities)
    {
        entityIDs.push_back(entity.entityID);
        entity.Release();
    }

    for (int entityID : entityIDs)
    {
        DuckEngine::DUCKENGINE_ComponentManager.RemoveAllComponents(entityID);
        DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(entityID);
    }

    DuckEngine::SetCameraPosition(0.0f, 0.0f);

}
