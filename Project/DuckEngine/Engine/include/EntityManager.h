#pragma once
#include <vector>
#include "ComponentManager.h"
#include "Entity.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class EntityManager
{
  private:
    std::vector<Entity> entities;

  public:
    DUCKENGINE_API Entity& CreateEntity();

    DUCKENGINE_API void RemoveEntity(int entityID);

    DUCKENGINE_API std::vector<Entity>& GetEntities();

    DUCKENGINE_API Entity* GetEntityByName(const std::string& name);
};
