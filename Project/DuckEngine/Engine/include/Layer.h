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

    DUCKENGINE_API int GetOrder() const;
    DUCKENGINE_API void SetOrder(int _order);

    DUCKENGINE_API bool IsVisible() const;
    DUCKENGINE_API void SetVisible(bool _visible);

private:
    std::vector<Entity*> entities; // Entities in this layer
    int order = 1;                     // Order of the layer
    bool visible = true;                  // Visibility of the layer
};
