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
    std::vector<Entity> entities;

  public:
    /************************************************************************
    @brief Creates a new entity and adds it to the entity list.
    @return A reference to the newly created entity.
    *************************************************************************/
    DUCKENGINE_API Entity& CreateEntity();

    /************************************************************************
    @brief Removes an entity by its ID from the entity list.
    @param entityID The ID of the entity to be removed.
    *************************************************************************/
    DUCKENGINE_API void RemoveEntity(int entityID);

    /************************************************************************
    @brief Retrieves all entities managed by this EntityManager.
    @return A reference to the vector of entities.
    *************************************************************************/
    DUCKENGINE_API std::vector<Entity>& GetEntities();

    /************************************************************************
    @brief Retrieves a pointer to an entity based on its name.
    @param name The name of the entity to search for.
    @return A pointer to the entity if found, otherwise nullptr.
    *************************************************************************/
    DUCKENGINE_API Entity* GetEntityByName(const std::string& name);

    DUCKENGINE_API Entity* GetEntity(int entityID);

	DUCKENGINE_API int GetEntityCount();

};
