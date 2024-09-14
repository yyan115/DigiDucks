#pragma warning(disable : 4251)
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

class DUCKENGINE_API EntityManager
{
  private:
    std::vector<Entity> entities;
    int nextEntityID = 0;

  public:
    Entity& CreateEntity();

    void RemoveEntity(int entityID);

    const std::vector<Entity>& GetEntities() const;
};
