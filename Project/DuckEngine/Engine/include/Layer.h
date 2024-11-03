#pragma once

#include <vector>
#include "Entity.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class Layer
{
public:
    // Adds an entity to the layer
    DUCKENGINE_API void AddEntity(Entity* entity);

    // Accessor for entities in this layer
    DUCKENGINE_API const std::vector<Entity*>& GetEntities() const;

private:
    std::vector<Entity*> entities; // Entities in this layer
};
