/******************************************************************************/
/*!
\file		CircleColliderSystem.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date       Jan 21 2025
\brief  	Implementation of the CircleColliderSystem class for handling
			collision detection and response between bounding boxes and circles.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "CircleColliderSystem.h"


void CircleColliderSystem::Start()
{
	float cellSize = 50.0f;
	// Add circle colliders to the grid
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>()) {
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!entityTrans) continue;

		SpatialGrid::AddToCell(entityId, entityTrans->GetPosition(), cellSize);
	}
}

/****************************************************************
 * @brief Updates the Circle Collider system each frame
 *
 * This function retrieves the delta time and checks for collisions
 * between all BoundingCircle and BoundingBox components. It updates
 * their positions based on their velocities and handles collision responses.
 *
 * @return void
 ****************************************************************/
void CircleColliderSystem::Update() 
{
}

void CircleColliderSystem::FixedUpdate()
{
	float deltaTime = DuckEngine::FixedDeltaTime();
	float cellSize = 50.0f;

	// Clear the spatial grid and repopulate it
	SpatialGrid::Clear();

	// First update all collider positions and add them to the spatial grid
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!entityTrans) continue;

		BoundingCircle* entityCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		if (entityCircle)
		{
			entityCircle->setCenter(entityTrans->GetPosition() + entityCircle->getOffSet());
			SpatialGrid::AddToCell(entityId, entityTrans->GetPosition(), cellSize);
		}
	}

	// Now check for collisions between circle colliders
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		if (!DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityId)) {
			continue;
		}
		BoundingCircle* entityCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* entityRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		if (!entityTrans || !entityCircle)
		{
			continue;
		}

		if (!entityRb || entityRb->isStatic) {
			continue; // Skip static objects for collision detection
		}

		// Get nearby entities for optimized collision detection
		auto nearbyEntities = SpatialGrid::GetNearbyEntities(entityTrans->GetPosition(), cellSize + entityCircle->getRadius());

		for (int otherId : nearbyEntities)
		{
			if (otherId == entityId) continue; // Skip self

			// Check collision with other circles
			BoundingCircle* otherCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(otherId);
			if (otherCircle)
			{
				RigidbodyComponent* otherRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(otherId);

				// Vector for each object's velocity, default to zero if no rigidbody
				Vec2 vel1 = entityRb ? entityRb->velocity : Vec2(0.0f, 0.0f);
				Vec2 vel2 = otherRb ? otherRb->velocity : Vec2(0.0f, 0.0f);

				// Check collision between the two circles
				if (checkCollisionCC(*entityCircle, *otherCircle, deltaTime, vel1, vel2))
				{
					// Skip collision response if either collider isn't kinematic
					if (!entityCircle->isKinematic || !otherCircle->isKinematic)
					{
						continue;
					}

					// Handle collision response if other object has a rigidbody
					if (otherRb)
					{
						if (otherRb->isStatic) {
							// If other object is static, just stop this object
							entityRb->velocity = Vec2(0.0f, 0.0f);
						}
						else
						{
							// Both objects are dynamic - redistribute momentum
							Vec2 combinedVelocity = entityRb->velocity + otherRb->velocity;

							if (entityRb->velocity.lengthSquared() < otherRb->velocity.lengthSquared())
							{
								otherRb->velocity = Vec2(0.f, 0.f);
								entityRb->velocity = 3 * combinedVelocity / 4;
							}
							else
							{
								otherRb->velocity = 3 * combinedVelocity / 4;
								entityRb->velocity = Vec2(0.f, 0.f);
							}
						}
					}
					else
					{
						// Other object has no rigidbody, just stop this object
						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
				}
			}

			// Check collision with box colliders (code similar to your existing box collision)
			BoundingBox* otherBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(otherId);
			if (otherBox)
			{
				// Your existing box collision code...
				RigidbodyComponent* otherRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(otherId);
				Vec2 interceptPoint;

				Vec2 vel1 = entityRb ? entityRb->velocity : Vec2(0.0f, 0.0f);
				Vec2 vel2 = otherRb ? otherRb->velocity : Vec2(0.0f, 0.0f);

				if (checkCollisionCB(*entityCircle, *otherBox, interceptPoint, deltaTime, vel1, vel2))
				{
					if (!entityCircle->isKinematic || !otherBox->isKinematic)
					{
						continue;
					}

					if (otherRb)
					{
						if (otherRb->isStatic) {
							entityRb->velocity = Vec2(0.0f, 0.0f);
						}
						else
						{
							Vec2 combinedVelocity = entityRb->velocity + otherRb->velocity;
							if (entityRb->velocity.lengthSquared() < otherRb->velocity.lengthSquared())
							{
								otherRb->velocity = Vec2(0.f, 0.f);
								entityRb->velocity = 3 * combinedVelocity / 4;
							}
							else
							{
								otherRb->velocity = 3 * combinedVelocity / 4;
								entityRb->velocity = Vec2(0.f, 0.f);
							}
						}
					}
					else
					{
						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
				}
			}
		}

		entityCircle->setCenter(entityTrans->GetPosition() + entityCircle->getOffSet());
	}
}
void CircleColliderSystem::Render() {
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);

		if (circle->showDebugCollider) DuckEngine::DrawCircle(circle->getCenter() + circle->getOffSet(), circle->getRadius(),{ 0.0f, 255.0f, 0.0f });
	}
}

void CircleColliderSystem::EditorUpdate()
{
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		BoundingCircle* entityCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* entityRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		if (entityCircle && entityTrans && entityRb)
		{
			entityCircle->setCenter(entityTrans->GetPosition());
		}
	}
}