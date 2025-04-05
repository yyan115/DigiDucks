/******************************************************************************/
/*!
\file       TransformComponent.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the TransformComponent class, which represents the
            position, rotation, and scale of an entity in the game engine.
            This component also includes the option to set the transform
            relative to the camera.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "TransformComponent.h"
#include "DuckEngine.h"

void TransformComponent::SetPosition(const Vec2& newPos)
{
    previousPosition = newPos;
    bool positionChanged = (worldPosition != newPos);
    Vec2 oldPosition = worldPosition;

    // Update the world position
    worldPosition = newPos;

    // Find parent entity
    Entity* parentEntity = nullptr;
    for (const auto& potentialParent : DuckEngine::DUCKENGINE_EntityManager.GetEntities())
    {
        auto it = std::find_if(potentialParent->childEntities.begin(), potentialParent->childEntities.end(),
            [this](const std::shared_ptr<Entity>& child) { return child->entityID == GetEntityID(); });
        if (it != potentialParent->childEntities.end())
        {
            parentEntity = potentialParent.get();
            break;
        }
    }

    // Update local position
    if (parentEntity)
    {
        auto parentTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(parentEntity->entityID);
        if (parentTransform)
        {
            localPosition = worldPosition - parentTransform->worldPosition;
        }
    }
    else
    {
        localPosition = worldPosition;
    }

    // Update child positions if position changed
    if (positionChanged)
    {
        Vec2 delta = newPos - oldPosition;
        UpdateChildPositions(delta);
    }

    // Finally update previous position
    previousPosition = worldPosition;
}


Vec2& TransformComponent::GetPosition()
{
	return worldPosition;
}

Vec2& TransformComponent::GetLocalPosition()
{
	return localPosition;
}

void TransformComponent::UpdateChildPositions(const Vec2& delta)
{
	auto entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetEntityID());
	if (!entity)
	{
		std::cerr << "Warning: Entity not found for entity ID " << GetEntityID() << std::endl;
		return;
	}

	if (!entity->childEntities.empty())
	{
		for (const auto& childEntity : entity->childEntities)
		{
			if (childEntity)
			{
				auto childTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(childEntity->entityID);
				if (childTransform)
				{
					childTransform->worldPosition += delta;
					childTransform->localPosition = childTransform->worldPosition - worldPosition;

					childTransform->UpdateChildPositions(delta);
				}
			}
		}
	}
}
