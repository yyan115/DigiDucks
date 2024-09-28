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

void BoxColliderSystem::Start()
{

}

void BoxColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	// Find player's box collider
	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
	{
		BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);
		TransformComponent* boxTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* boxRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		box->setCenter(boxTrans->position);

		// Check collision with box collider
		for (const auto& [entity2Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			if(entityId == entity2Id) continue;

			BoundingBox* box2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity2Id);
			TransformComponent* boxTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* boxRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			if (!boxTrans2) continue;

			if (!boxRb2) {	// One of the box is moving
				if (checkCollisionBB(*box, *box2, deltaTime, boxRb->velocity)) {
					boxRb->velocity = Vec2(0.0f, 0.0f);
				}
			}
			else{
				if (checkCollisionBB(*box, *box2, deltaTime, boxRb->velocity, boxRb2->velocity)) {
					// If there is a collision
					if (boxRb2->isStatic) {	// If the box is static
						boxRb->velocity = Vec2(0.0f, 0.0f);
					}
					else { // If the box is not static
						Vec2 combinedVelocity = boxRb->velocity + boxRb2->velocity;
						if (boxRb->velocity.lengthSquared() < boxRb2->velocity.lengthSquared()) {
							boxRb2->velocity = Vec2(0.f, 0.f);
							boxRb->velocity = combinedVelocity;
						}
						else {
							boxRb2->velocity = combinedVelocity;
							boxRb->velocity = Vec2(0.f, 0.f);
						}
					}
				}
			}
		}

		// Check collision with circle
		for (const auto& [entity2Id, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
		{
			if(entity2Id == entityId) continue;

			BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity2Id);
			TransformComponent* circleTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* circleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			if(!circleTrans) continue;

			if (!circleRb) {
				if (checkCollisionBC(*box, *circle, deltaTime, boxRb->velocity)) {
					boxRb->velocity = Vec2(0.0f, 0.0f);
				}
			}
			else {
				if (checkCollisionBC(*box, *circle, deltaTime, boxRb->velocity, circleRb->velocity)) {
					if (circleRb->isStatic) {
						boxRb->velocity = Vec2(0.0f, 0.0f);
					}
					else {
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
					}
				}
			}

		}

		box->setCenter(boxTrans->position);
	}
}