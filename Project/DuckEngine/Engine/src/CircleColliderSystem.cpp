/******************************************************************************/
/*!
\file		CircleColliderSystem.h
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 13 2024
\brief  	This file includes System class for Circle Collider

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "CircleColliderSystem.h"

void CircleColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	// Find player's circle collider
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		// Only 1 circle collider in this scene
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* circleTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* circleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		circle->setCenter(circleTrans->position);
		Vec2 nextPos;

		// Check collision with box collider
		for (const auto& [entity2Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			if(entityId == entity2Id) continue;
			BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity2Id);
			TransformComponent* boxTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* boxRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);
			
			// Ensure the entity has both BoundingBox and RigidbodyComponent
			if(!boxTrans || !boxRb) continue;

			nextPos = circle->getCenter() + circleRb->velocity * deltaTime;
			// Check collision
			if (checkCollisionCB(*circle, nextPos, *box)) {
				// If there is a collision
				if (boxRb->isStatic) {	// If the box is static
					circleRb->velocity = Vec2(0.0f, 0.0f);
				}
				else { // If the box is not static
					// Give half of the velocity to the box
					boxRb->velocity = circleRb->velocity / 2;
					// So that the obstacle does not stick to the player
					boxTrans->position += boxRb->velocity * deltaTime;
					
					// Give half of the velocity to the circle
					circleRb->velocity = circleRb->velocity / 2;
				}
			}
		}

		// Check collision with circle collider
		for (const auto& [entity2Id, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
		{
			if(entityId == entity2Id) continue;
			BoundingCircle* circle2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity2Id);
			TransformComponent* circle2Trans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* circle2Rb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			// Ensure the entity has both BoundingCircle and RigidbodyComponent
			if(!circle2Trans || !circle2Rb) continue;

			// Check collision
			if (checkCollisionCC(*circle, *circle2, deltaTime, circleRb->velocity, circle2Rb->velocity)) {
				// If there is a collision
				if (circle2Rb->isStatic) {	// If the circle is static
					circleRb->velocity = Vec2(0.0f, 0.0f);
				}
				else { // If the circle is not static
					// Give half of the velocity to the circle
					circle2Rb->velocity = circleRb->velocity / 2;
					// So that the obstacle does not stick to the player
					circle2Trans->position += circle2Rb->velocity * deltaTime;

					// Give half of the velocity to the circle
					circleRb->velocity = circleRb->velocity /2;
				}
			}

		}

		// Update Collider's position based on velocity
		circle->setCenter(circleTrans->position);
	}
}