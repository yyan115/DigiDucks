#include "EntityManager.h"
#include "DuckEngine.h"

// Create a new entity and return it
// Create a new entity and return it
Entity& EntityManager::CreateEntity()
{
    int newID = static_cast<int>(entities.size());
    entities.emplace_back(newID);
    return entities.back();
}

// Remove an entity (and its associated components)
void EntityManager::RemoveEntity(int entityID)
{
    // Automatically remove all components associated with the entity
    DuckEngine::DUCKENGINE_ComponentManager.RemoveAllComponents(entityID);
    
    // Remove the entity itself
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [entityID](const Entity& entity) { return entity.EntityID == entityID; }),
        entities.end());
}

// Return a reference to the list of entities
const std::vector<Entity>& EntityManager::GetEntities() const {
    return entities;
}
