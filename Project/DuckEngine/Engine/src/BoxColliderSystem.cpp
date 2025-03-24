/******************************************************************************/
/*!
\file		BoxColliderSystem.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date       Jan 21 2025
\brief  	Implementation of the BoxColliderSystem class for handling 
			collision detection and response between bounding boxes and circles.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "BoxColliderSystem.h"

void BoxColliderSystem::Start()
{

}

/****************************************************************
* @brief Updates the Box Collider system each frame
*
* This function retrieves the delta time and checks for collisions
* between all BoundingBox and BoundingCircle components. It updates
* their positions based on their velocities and handles collision responses.
*
* @return void
****************************************************************/


void BoxColliderSystem::Update() 
{

}

void BoxColliderSystem::FixedUpdate()
{
	float deltaTime = DuckEngine::FixedDeltaTime();
	float cellSize = 50.0f;

	SpatialGrid::Clear();

	// Step 1: Populate the grid with box colliders
	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>()) {
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!entityTrans) continue;

		SpatialGrid::AddToCell(entityId, entityTrans->GetPosition(), cellSize);
	}


	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
	{
		BoundingBox* entityBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* entityRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);
		if (!entityTrans)
		{
			continue;
		}
		else if (!entityRb || entityRb->isStatic)
		{
			entityBox->setCenter(entityTrans->GetPosition() + entityBox->getOffSet());
			continue;	// No rb = not moving
		}

		// Update Collider to current position
		entityBox->setCenter(entityTrans->GetPosition() + entityBox->getOffSet());

		// Get nearby entities
		auto nearbyEntities = SpatialGrid::GetNearbyEntities(entityTrans->GetPosition(), cellSize);

		/****************************************************************
		* @brief Check collisions between this box other objects in the grid
		*
		* This section iterates through all nearby entities to check for
		* any collisions with the current box collider. Collision responses
		* are calculated based on the velocities of the objects.
		****************************************************************/
		for (int otherId : nearbyEntities)
		{
			if (otherId == entityId) continue;
			RigidbodyComponent* otherRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(otherId);

			BoundingBox* otherBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(otherId);
			// If the other entity is a box
			if (otherBox) {
				if (!otherRb)	// Only Original Box is Moving
				{
					if (checkCollisionBB(*entityBox, *otherBox, deltaTime, entityRb->velocity))
					{
						if (!entityBox->isKinematic || !otherBox->isKinematic)
						{
							continue;
						}

						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
				}
				else	// Both Box are moving
				{
					if (checkCollisionBB(*entityBox, *otherBox, deltaTime, entityRb->velocity, otherRb->velocity))
					{
						if (!entityBox->isKinematic || !otherBox->isKinematic)
						{
							continue;
						}
						// If there is a collision
						if (otherRb->isStatic)	// If the box is static
						{
							entityRb->velocity = Vec2(0.0f, 0.0f);
						}
						else	// If the box is not static
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

				// If object is box, do not check for circle
				continue;
			}

			BoundingCircle* otherCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(otherId);
			// If the other entity is a circle
			if (otherCircle)
			{
				Vec2 intercepPt;
				if (!otherRb)	// Only Original Box is Moving
				{
					if (checkCollisionBC(*entityBox, *otherCircle, intercepPt, deltaTime, entityRb->velocity))
					{
						if (!entityBox->isKinematic || !otherCircle->isKinematic)
						{
							continue;
						}
						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
				}
				else	// Both Box and Circle are moving
				{
					if (checkCollisionBC(*entityBox, *otherCircle, intercepPt, deltaTime, entityRb->velocity, otherRb->velocity))
					{
						if (!entityBox->isKinematic || !otherCircle->isKinematic)
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

		entityBox->setCenter(entityTrans->GetPosition() + entityBox->getOffSet());
	}

}

void BoxColliderSystem::Render()
{
	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
	{
		BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);

		if (box->showDebugCollider) DuckEngine::DrawRectangle(box->getBtmL() + box->getOffSet(), box->getTopR() + box->getOffSet(), box->rotation, {0.0f, 255.0f, 0.0f});
	}
}

void BoxColliderSystem::EditorUpdate()
{
	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
	{
		BoundingBox* entityBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);

		if (entityBox && entityTrans)
		{
			entityBox->setCenter(entityTrans->GetPosition() + entityBox->getOffSet());
			entityBox->setRotation(entityTrans->angle);
		}
	}
}