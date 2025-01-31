/******************************************************************************/
/*!
\file       EntityManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the EntityManager class responsible for managing the
            lifecycle of entities in the game engine. It includes methods
            for creating, removing, and retrieving entities.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

/************************************************************************
@brief The EntityManager class manages the creation, removal, and retrieval
       of entities within the game engine.
*************************************************************************/
class EntityManager
{
  private:
    std::vector<std::shared_ptr<Entity>> entities;
    int nextEntityID = 0;

public:
    DUCKENGINE_API std::shared_ptr<Entity> CreateEntity();
    DUCKENGINE_API void RemoveEntity(int entityID);
    DUCKENGINE_API std::vector<std::shared_ptr<Entity>>& GetEntities();
    DUCKENGINE_API std::shared_ptr<Entity> GetEntityByName(const std::string& name); 
    DUCKENGINE_API std::shared_ptr<Entity> GetEntity(int entityID);
    DUCKENGINE_API int GetEntitiesCount();
    DUCKENGINE_API void ResetEntityID() { nextEntityID = 0; }
    DUCKENGINE_API void RemoveAllEntities();
    DUCKENGINE_API std::shared_ptr<Entity> GetParentEntity(int childEntityID);
};
