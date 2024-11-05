/******************************************************************************/
/*!
\file       Prefab.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the Prefab class, allowing for the addition of components
			and the instantiation of prefabs into entities with their associated
			components.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Prefab.h"
#include "DuckEngine.h"

/************************************************************************
@brief Adds a component to the prefab's list of components. When the prefab
	   is instantiated, all components in this list will be cloned and added
	   to the new entity.
@param component A shared pointer to the component to be added to the prefab.
*************************************************************************/
void Prefab::AddComponent(const std::shared_ptr<Component>& component)
{
	components.push_back(component);
}

/************************************************************************
@brief Instantiates the prefab, creating a new entity at the specified
	   position with the prefab's texture, scale, and components. Each
	   component is cloned and added to the new entity.
@param newPosition The position where the new entity will be created.
@return A pointer to the newly created entity.
*************************************************************************/
Entity* Prefab::Instantiate(Vec2 newPosition)
{
	Entity* entity = EntityFactory::CreateEntity(texturePath, newPosition, { 1.0f, 1.0f });
    //entity->name = "GameObject " + entity->entityID;
	entity->prefabName = name;

    for (const auto& component : components)
    {
        std::shared_ptr<Component> clonedComponent = component->Clone();

        if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRendererComponent>(clonedComponent))
        {
            spriteRenderer->texturePath = this->texturePath;

            if (!spriteRenderer->texturePath.empty())
            {
                spriteRenderer->texture = *DuckEngine::DUCKENGINE_AssetManager.LoadTexture(spriteRenderer->texturePath.c_str())[0];
            }
        }

        if (auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(clonedComponent))
        {
            transformComponent->SetPosition(newPosition);
        }

        if (auto animatorComponent = std::dynamic_pointer_cast<AnimatorComponent>(clonedComponent))
        {
            if (!animatorComponent->animations.empty())
            {
                auto firstAnimation = animatorComponent->animations.begin();
                animatorComponent->currentAnimation = &firstAnimation->second;
            }
        }

        DuckEngine::DUCKENGINE_ComponentManager.AddComponent(entity->entityID, clonedComponent);
    }

    return entity;
}