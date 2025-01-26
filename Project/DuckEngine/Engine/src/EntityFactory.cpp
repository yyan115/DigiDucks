/******************************************************************************/
/*!
\file       EntityFactory.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the EntityFactory class, which provides methods for
            creating entities with optional position, scale, and texture
            configurations in the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "EntityFactory.h"
#include "DuckEngine.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "Bounding.h"

/************************************************************************
@brief Creates a new entity with default properties.
@return A pointer to the newly created entity.
*************************************************************************/
Entity* EntityFactory::CreateEntity()
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity().get();
    return entity;
}

/************************************************************************
@brief Creates a new entity with specified position and scale.
@param position The initial position of the entity.
@param scale The initial scale of the entity.
@return A pointer to the newly created entity.
*************************************************************************/
Entity* EntityFactory::CreateEntity(Vec2 position, Vec2 scale)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity().get();
    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->entityID);
    transform->SetPosition(position);
    transform->scale = scale;
    return entity;
}

/************************************************************************
@brief Creates a new entity with a texture, position, and scale.
@param texturePath The file path to the texture used by the entity.
@param position The initial position of the entity.
@param scale The initial scale of the entity.
@return A pointer to the newly created entity.
*************************************************************************/
Entity* EntityFactory::CreateEntity(const std::string& texturePath, Vec2 position, Vec2 scale)
{
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity().get();

    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->entityID);
    SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entity->entityID, true);

    transform->SetPosition(position);
    transform->scale = scale;

    if (!texturePath.empty())
    {
        spriteRenderer->texture = *AssetManager::LoadTexture(texturePath.c_str())[0];
    }
    else
    {
        spriteRenderer->useColor = true;
        spriteRenderer->color = { 255.0f, 0.0f, 255.0f, 255.0f };
    }

    return entity;

}