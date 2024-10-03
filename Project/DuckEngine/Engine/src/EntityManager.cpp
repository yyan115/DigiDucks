/******************************************************************************/
/*!
\file       EntityManager.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the EntityManager class, providing functionality for
            creating, removing, and retrieving entities, as well as managing
            their associated components.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "EntityManager.h"
#include "DuckEngine.h"

/************************************************************************
@brief Creates a new entity, assigns it a unique ID, and adds it to the list
       of entities managed by the EntityManager.
@return A reference to the newly created entity.
*************************************************************************/
Entity& EntityManager::CreateEntity()
{
    int newID = static_cast<int>(entities.size());
    entities.emplace_back(newID);
    return entities.back();
}

/************************************************************************
@brief Removes an entity by its ID, including all associated components.
@param entityID The ID of the entity to be removed.
*************************************************************************/
void EntityManager::RemoveEntity(int entityID)
{
    DuckEngine::DUCKENGINE_ComponentManager.RemoveAllComponents(entityID);
    
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [entityID](const Entity& entity) { return entity.entityID == entityID; }
        ),
        entities.end()
    );

}

/************************************************************************
@brief Retrieves a reference to the list of all entities managed by this
       EntityManager.
@return A reference to the vector of entities.
*************************************************************************/
std::vector<Entity>& EntityManager::GetEntities() 
{
    return entities;
}

/************************************************************************
@brief Retrieves a pointer to an entity by its name, if it exists in the
       entity list.
@param name The name of the entity to search for.
@return A pointer to the entity if found, otherwise nullptr.
*************************************************************************/
Entity* EntityManager::GetEntityByName(const std::string& name)
{
    for (auto& entity : entities)
    {
        if (entity.IsName(name.c_str()))
        {
            return &entity;
        }
    }
    return nullptr;
}

