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

void CircleColliderSystem::Start()
{

}

void CircleColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	// Find player's circle collider
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		// Only 1 circle collider in this scene
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* circleTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* circleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		if(!circleRb) continue;	// No rb = not moving

		// Update Collider to current position
		circle->setCenter(circleTrans->position);

		// Circle - Circle
		for (const auto& [entity2Id, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
		{
			if (entityId == entity2Id) continue;
			BoundingCircle* circle2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity2Id);
			TransformComponent* circle2Trans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* circle2Rb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			// Ensure the entity has both BoundingCircle and RigidbodyComponent
			if (!circle2Trans) continue;

			Vec2 c2_velo;
			if (!circle2Rb) { // Consider static
				if (checkCollisionCC(*circle, *circle2, deltaTime, circleRb->velocity)) {
					circleRb->velocity = Vec2(0.0f, 0.0f);
				}
			}
			else {
				// Check collision
				if (checkCollisionCC(*circle, *circle2, deltaTime, circleRb->velocity, circle2Rb->velocity)) {
					// If there is a collision
					if (!circle2Rb || circle2Rb->isStatic) {	// If the circle is static
						circleRb->velocity = Vec2(0.0f, 0.0f);
					}
					else { // If the circle is not static
						Vec2 combinedVelocity = circleRb->velocity + circle2Rb->velocity;
						if (circleRb->velocity.lengthSquared() < circle2Rb->velocity.lengthSquared()) {
							circle2Rb->velocity = Vec2(0.f, 0.f);
							circleRb->velocity = 3 * combinedVelocity / 4;
						}
						else {
							// If circle velocity is greater, box will gain more velocity
							circle2Rb->velocity = 3 * combinedVelocity / 4;
							circleRb->velocity = Vec2(0.f, 0.f);
						}


					}
				}
			}
		}

		// Circle - Box
		for (const auto& [entity2Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			if(entityId == entity2Id) continue;
			BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity2Id);
			TransformComponent* boxTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* boxRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);
			
			// Ensure the entity has both BoundingBox and RigidbodyComponent
			if(!boxTrans) continue;

			// Check collision
			if (!boxRb) {
				if (checkCollisionCB(*circle, *box, deltaTime, circleRb->velocity)) {
					circleRb->velocity = Vec2(0.0f, 0.0f);
				}
			}
			if (checkCollisionCB(*circle, *box, deltaTime, circleRb->velocity, boxRb->velocity)) {
				// If there is a collision
				if (boxRb->isStatic) {	// If the box is static
					circleRb->velocity = Vec2(0.0f, 0.0f);
				}
				else { // If the box is not static
					Vec2 combinedVelocity = circleRb->velocity + boxRb->velocity;
					if (circleRb->velocity.lengthSquared() < boxRb->velocity.lengthSquared()) {
						boxRb->velocity = Vec2(0.f, 0.f);
						circleRb->velocity = 3 * combinedVelocity / 4;
					}
					else {
						// If circle velocity is greater, box will gain more velocity
						boxRb->velocity = 3 * combinedVelocity / 4;
						circleRb->velocity = Vec2(0.f, 0.f);
					}

					// Check if Box next position is colliding with a static object
					for (const auto& [entity3Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
					{
						if(entity2Id == entity3Id || entity3Id == entityId) continue;
						BoundingBox* box2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity3Id);
						TransformComponent* boxTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity3Id);
						RigidbodyComponent* boxRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity3Id);

						if (!boxRb2) {	// One of the box is moving
							if (checkCollisionBB(*box, *box2, deltaTime, boxRb->velocity)) {
								boxRb->velocity = Vec2(0.0f, 0.0f);
								circleRb->velocity = Vec2(0.0f, 0.0f);
							}
						}
						else {
							if (checkCollisionBB(*box, *box2, deltaTime, boxRb->velocity, boxRb2->velocity)) {
								// If there is a collision
								if (boxRb2->isStatic) {	// If the box is static
									boxRb->velocity = Vec2(0.0f, 0.0f);
									circleRb->velocity = Vec2(0.0f, 0.0f);
								}
								else { // If the box is not static
									Vec2 combinedVelocity = boxRb->velocity + boxRb2->velocity;
									if(boxRb->velocity.lengthSquared() < boxRb2->velocity.lengthSquared()) {
										boxRb2->velocity = Vec2(0.f, 0.f);
										boxRb->velocity = combinedVelocity;
									}
									else {
										boxRb2->velocity = combinedVelocity;
										boxRb->velocity = Vec2(0.f, 0.f);
									}

									circleRb->velocity = Vec2(0.0f, 0.0f);
								}
							}
						}
					}
				}
			}
		}

		
		// Update Collider's position based on velocity
		circle->setCenter(circleTrans->position);
	}
}