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

	// Add circle colliders to the grid
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>()) {
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!entityTrans) continue;

		SpatialGrid::AddToCell(entityId, entityTrans->GetPosition(), cellSize);
	}

	// Find player's circle collider
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		BoundingCircle* entityCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* entityRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);
		if (!entityTrans)
		{
			continue;
		}
		else if (!entityRb || entityRb->isStatic) 
		{
			entityCircle->setCenter(entityTrans->GetPosition() + entityCircle->getOffSet());
			continue;	// No rb = not moving
		}

		// Update Collider to current position
		entityCircle->setCenter(entityTrans->GetPosition() + entityCircle->getOffSet());

		auto nearbyEntities = SpatialGrid::GetNearbyEntities(entityTrans->GetPosition(), cellSize);

		for (int otherId : nearbyEntities)
		{
			if (otherId == entityId) continue;
			RigidbodyComponent* otherRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(otherId);

			BoundingCircle* otherCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(otherId);
			// If the other entity is a circle
			if (otherCircle)
			{
				if (!otherRb)	// Only Original Circle is Moving
				{
					if (checkCollisionCC(*entityCircle, *otherCircle, deltaTime, entityRb->velocity))
					{
						if (!entityCircle->isKinematic || !otherCircle->isKinematic)
						{
							continue;
						}
						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
				}
				else	// Both Circles are moving
				{
					if (checkCollisionCC(*entityCircle, *otherCircle, deltaTime, entityRb->velocity, otherRb->velocity))
					{
						if (!entityCircle->isKinematic || !otherCircle->isKinematic)
						{
							continue;
						}
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
				}

				// If object is circle, do not check for box
				continue;
			}

			BoundingBox* otherBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(otherId);
			// If the other entity is a box
			if (otherBox)
			{
				Vec2 interceptPoint;
				if (!otherRb)	// Only Original Circle is Moving
				{
					if (checkCollisionCB(*entityCircle, *otherBox, interceptPoint, deltaTime, entityRb->velocity))
					{
						if (!entityCircle->isKinematic || !otherBox->isKinematic)
						{
							continue;
						}
						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
				}
				else	// Both Circle and Box are moving
				{
					if (checkCollisionCB(*entityCircle, *otherBox, interceptPoint, deltaTime, entityRb->velocity, otherRb->velocity))
					{
						if (!entityCircle->isKinematic || !otherBox->isKinematic)
						{
							continue;
						}
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
				}
			}
		}

		// Update Collider's position based on velocity
		entityCircle->setCenter(entityTrans->GetPosition() + entityCircle->getOffSet());
	}
}

void CircleColliderSystem::Render() {
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);

		if (circle->showDebugCollider) DuckEngine::DrawCircle(circle->getCenter() + circle->getOffSet(), circle->getRadius(), { 0.0f, 255.0f, 0.0f });
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