/******************************************************************************/
/*!
\file		BoxColliderSystem.h
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 13 2024
\brief  	This file includes System class for Box Collider

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "BoxColliderSystem.h"

void BoxColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	// Find player's box collider
	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
	{
		BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);
		TransformComponent* boxTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* boxRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		// Check collision with box collider
		for (const auto& [entity2Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			if(entityId == entity2Id) continue;

			BoundingBox* box2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity2Id);
			TransformComponent* boxTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* boxRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			if(!boxTrans2 || !boxRb2) continue;
			if(checkCollisionBB(*box,*box2, deltaTime,boxRb->velocity,boxRb2->velocity)) {
				// If there is a collision
				if (boxRb->isStatic) {	// If the box is static
					boxRb2->velocity = Vec2(0.0f, 0.0f);
				}
				else { // If the box is not static
					// Give half of the velocity to the box
					boxRb2->velocity = boxRb->velocity / 2;
					// So that the obstacle does not stick to the player
					boxTrans2->position += boxRb2->velocity * deltaTime;
					
					// Give half of the velocity to the circle
					boxRb->velocity = boxRb->velocity / 2;
				}
			}
		}

		box->setCenter(boxTrans->position);
	}
}