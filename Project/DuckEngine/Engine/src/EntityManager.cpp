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
#include "ComponentFactory.h"

/************************************************************************
@brief Creates a new entity, assigns it a unique ID, and adds it to the list
       of entities managed by the EntityManager.
@return A reference to the newly created entity.
*************************************************************************/
std::shared_ptr<Entity> EntityManager::CreateEntity()
{
    int newID = nextEntityID++;
    auto entity = std::make_shared<Entity>(newID); // Create shared_ptr
    entities.push_back(entity);
    return entity;
}

/************************************************************************
@brief Removes an entity by its ID, including all associated components.
@param entityID The ID of the entity to be removed.
*************************************************************************/
void EntityManager::RemoveEntity(int entityID)
{
	for (auto& potentialParent : entities)
	{
		auto& kids = potentialParent->childEntities;
		auto& names = potentialParent->childNames;

		kids.erase(
			std::remove_if(
				kids.begin(), kids.end(),
				[entityID](std::shared_ptr<Entity> const& child)
				{
					return child->entityID == entityID;
				}
			),
			kids.end()
		);
		names.erase(
			std::remove_if(
				names.begin(), names.end(),
				[&](std::string const& childName)
				{
					return false;
				}
			),
			names.end()
		);
	}

	DuckEngine::DUCKENGINE_ComponentManager.RemoveAllComponents(entityID);

	auto it = std::find_if(entities.begin(), entities.end(),
		[entityID](const std::shared_ptr<Entity>& entity)
		{
			return entity->entityID == entityID;
		});

	if (it != entities.end())
		entities.erase(it);
}


/************************************************************************
@brief Retrieves a reference to the list of all entities managed by this
       EntityManager.
@return A reference to the vector of entities.
*************************************************************************/
std::vector<std::shared_ptr<Entity>>& EntityManager::GetEntities()
{
    return entities;
}

/************************************************************************
@brief Retrieves a pointer to an entity by its name, if it exists in the
       entity list.
@param name The name of the entity to search for.
@return A pointer to the entity if found, otherwise nullptr.
*************************************************************************/
std::shared_ptr<Entity> EntityManager::GetEntityByName(const std::string& name)
{
    for (auto& entity : entities) 
    {
        if (entity->IsName(name.c_str())) 
        {
            return entity;
        }
    }
    return nullptr;
}

std::shared_ptr<Entity> EntityManager::GetEntity(int entityID)
{
    for (auto& entity : entities)
    {
        if (entity->entityID == entityID)
        {
            return entity;
        }
    }
    return nullptr;
}

int EntityManager::GetEntitiesCount() 
{
    return static_cast<int>(entities.size());
}


void EntityManager::RemoveAllEntities() 
{
    for (const auto& entity : entities) {
        DuckEngine::DUCKENGINE_ComponentManager.RemoveAllComponents(entity->entityID);
    }
    entities.clear();
    ResetEntityID();
}

std::shared_ptr<Entity> EntityManager::GetParentEntity(int childEntityID)
{
    for (auto& potentialParent : entities)
    {
        for (const auto& child : potentialParent->childEntities)
        {
            if (child->entityID == childEntityID)
            {
                return potentialParent;
            }
        }
    }
    return nullptr;
}

bool EntityManager::ShouldSkipDueToInvisibleAncestor(int entityID)
{
	std::shared_ptr<Entity> currentParent = GetParentEntity(entityID);
	while (currentParent) 
    {
		auto* parentSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(currentParent->entityID);
		if (parentSpriteRenderer && !parentSpriteRenderer->isVisible) 
        {
			return true; 
		}
		currentParent = GetParentEntity(currentParent->entityID);
	}
	return false;
}

std::shared_ptr<Entity> EntityManager::DuplicateEntity(int oldEntityID)
{
	std::shared_ptr<Entity> oldEntity = GetEntity(oldEntityID);
	if (!oldEntity)
	{
		return nullptr;
	}

	std::string baseName = oldEntity->name + "_copy";
	std::string uniqueName = baseName;
	int copyIndex = 1;

	while (GetEntityByName(uniqueName))
	{
		uniqueName = baseName + "_" + std::to_string(copyIndex);
		copyIndex++;
	}

	std::shared_ptr<Entity> newEntity = CreateEntity();
	newEntity->name = uniqueName;
	newEntity->layerName = oldEntity->layerName;
	newEntity->prefabName = oldEntity->prefabName;

	nlohmann::json tempJson;
	ComponentFactory::SaveComponentsToJson(oldEntityID, tempJson["components"]);
	ComponentFactory::AddComponentsToEntity(newEntity.get(), tempJson["components"]);

	TransformComponent* oldTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(oldEntityID);
	TransformComponent* newTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(newEntity->entityID);

	if (oldTransform && newTransform)
	{
		newTransform->SetPosition(oldTransform->GetPosition());
	}

	for (const auto& oldChild : oldEntity->childEntities)
	{
		if (!oldChild)
		{
			continue;
		}

		std::shared_ptr<Entity> newChild = DuplicateEntity(oldChild->entityID);
		if (newChild)
		{
			newEntity->childEntities.push_back(newChild);
			newEntity->childNames.push_back(newChild->name);
		}
	}

	return newEntity;
}
