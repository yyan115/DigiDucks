/******************************************************************************/
/*!
\file		RigidbodySystem.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 28 2024
\brief  	This file includes System class for Rigidbody

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "RigidbodySystem.h"
#include "RigidbodyComponent.h"
//#include <algorithm>

void RigidbodySystem::Start()
{

}

void RigidbodySystem::Update()
{
    // Get deltaTime (elapsed time between frames)
    float deltaTime = DuckEngine::DeltaTime();

    // Loop through all entities that have RigidbodyComponent
    for (const auto& [entityId, rigidbodyComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<RigidbodyComponent>())
    {
        RigidbodyComponent* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);
        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);

        // Ensure the entity has both RigidbodyComponent and TransformComponent
        if (rigidbody && transform)
        {
            // Skip if the rigidbody is static (not affected by physics)
            if (rigidbody->isStatic)
            {
                continue;
            }

            // Update velocity based on acceleration and deltaTime
            rigidbody->velocity += rigidbody->acceleration * deltaTime;

            // Update the position based on the velocity
            transform->position += rigidbody->velocity * deltaTime;

            // Reset acceleration for the next frame
            rigidbody->acceleration = Vec2(0.0f, 0.0f);

            // Apply damping to the velocity
            rigidbody->velocity -= rigidbody->velocity * deltaTime;
        }
    }
}