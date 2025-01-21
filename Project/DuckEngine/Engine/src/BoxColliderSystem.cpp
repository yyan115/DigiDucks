/******************************************************************************/
/*!
\file		BoxColliderSystem.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Oct 28 2024
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
	float cellSize = 50.0f;
	// Step 1: Populate the grid with box colliders
	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>()) {
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		if (!entityTrans) continue;

		SpatialGrid::AddToCell(entityId, entityTrans->GetPosition(), cellSize);
	}
}

void BoxColliderSystem::FixedUpdate()
{
	float deltaTime = DuckEngine::FixedDeltaTime();

	float cellSize = 50.0f;
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

		{
			///****************************************************************
			//* @brief Check collisions between this box and other boxes
			//*
			//* This section iterates through all BoundingBox components to
			//* check for collisions with the current box collider. Collision
			//* responses are calculated based on the velocities of the boxes.
			//****************************************************************/
			//for (const auto& [entity2Id, boxColliderComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
			//{
			//	if (entity2Id == entityId) continue;

			//	BoundingBox* entityBox2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity2Id);
			//	TransformComponent* entityTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			//	RigidbodyComponent* entityRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			//	if (!entityTrans2) continue;

			//	if (!entityRb2) // One of the box is moving
			//	{
			//		if (checkCollisionBB(*entityBox, *entityBox2, deltaTime, entityRb->velocity))
			//		{
			//			if (!entityBox->isKinematic || !entityBox2->isKinematic)
			//			{
			//				continue;
			//			}

			//			entityRb->velocity = Vec2(0.0f, 0.0f);
			//		}
			//	}
			//	else
			//	{
			//		if (checkCollisionBB(*entityBox, *entityBox2, deltaTime, entityRb->velocity, entityRb2->velocity))
			//		{
			//			if (!entityBox->isKinematic || !entityBox2->isKinematic)
			//			{
			//				continue;
			//			}

			//			// If there is a collision
			//			if (entityRb2->isStatic)	// If the box is static
			//			{
			//				entityRb->velocity = Vec2(0.0f, 0.0f);
			//			}
			//			else	// If the box is not static
			//			{
			//				Vec2 combinedVelocity = entityRb->velocity + entityRb2->velocity;
			//				if (entityRb->velocity.lengthSquared() < entityRb2->velocity.lengthSquared())
			//				{
			//					entityRb2->velocity = Vec2(0.f, 0.f);
			//					entityRb->velocity = 3 * combinedVelocity / 4;
			//				}
			//				else
			//				{
			//					entityRb2->velocity = 3 * combinedVelocity / 4;
			//					entityRb->velocity = Vec2(0.f, 0.f);
			//				}

			//				// Check for collisions between the second box and circles
			//				/****************************************************************
			//				* @brief Check for collisions between the second box and circles
			//				*
			//				* This section checks for collisions between the second box collider
			//				* and all BoundingCircle components, handling their collision responses.
			//				****************************************************************/
			//				for (const auto& [entity3Id, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
			//				{
			//					if (entity2Id == entity3Id || entity3Id == entityId) continue;

			//					BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity3Id);
			//					TransformComponent* circleTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity3Id);
			//					RigidbodyComponent* circleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity3Id);

			//					if (!circleTrans) continue;

			//					if (!circleRb)
			//					{
			//						if (checkCollisionBC(*entityBox2, *circle, intercepPt, deltaTime, entityRb2->velocity))
			//						{
			//							if (!circle->isKinematic)
			//							{
			//								continue;
			//							}

			//							entityRb->velocity = Vec2(0.0f, 0.0f);
			//							entityRb2->velocity = Vec2(0.0f, 0.0f);
			//						}
			//					}
			//					else
			//					{
			//						if (checkCollisionBC(*entityBox2, *circle, intercepPt, deltaTime, entityRb2->velocity, circleRb->velocity))
			//						{
			//							if (!circle->isKinematic)
			//							{
			//								continue;
			//							}

			//							if (circleRb->isStatic) {
			//								entityRb->velocity = Vec2(0.0f, 0.0f);
			//								entityRb2->velocity = Vec2(0.0f, 0.0f);
			//							}
			//							else
			//							{
			//								combinedVelocity = circleRb->velocity + entityRb2->velocity;
			//								if (circleRb->velocity.lengthSquared() < entityRb2->velocity.lengthSquared())
			//								{
			//									entityRb2->velocity = Vec2(0.f, 0.f);
			//									circleRb->velocity = 3 * combinedVelocity / 4;
			//								}
			//								else
			//								{
			//									entityRb2->velocity = 3 * combinedVelocity / 4;
			//									circleRb->velocity = Vec2(0.f, 0.f);
			//								}

			//								entityRb->velocity = Vec2(0.0f, 0.0f);
			//							}
			//						}
			//					}

			//				}
			//			}
			//		}

			//	}
			//}

			//// Check collision with circle
			///****************************************************************
			//* @brief Check collisions between this box and circles
			//*
			//* This section checks for collisions between the box collider and
			//* all BoundingCircle components, handling the collision responses.
			//****************************************************************/
			//for (const auto& [entity2Id, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
			//{
			//	if (entity2Id == entityId) continue;

			//	BoundingCircle* entityCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity2Id);
			//	TransformComponent* entityTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			//	RigidbodyComponent* entityRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			//	if (!entityTrans2) continue;

			//	if (!entityRb2)
			//	{
			//		if (checkCollisionBC(*entityBox, *entityCircle, intercepPt, deltaTime, entityRb->velocity))
			//		{
			//			if (!entityBox->isKinematic || !entityCircle->isKinematic)
			//			{
			//				continue;
			//			}
			//			entityRb->velocity = Vec2(0.0f, 0.0f);
			//		}
			//	}
			//	else
			//	{
			//		if (checkCollisionBC(*entityBox, *entityCircle, intercepPt, deltaTime, entityRb->velocity, entityRb2->velocity))
			//		{
			//			if (!entityBox->isKinematic || !entityCircle->isKinematic)
			//			{
			//				continue;
			//			}

			//			if (entityRb2->isStatic) {
			//				entityRb->velocity = Vec2(0.0f, 0.0f);
			//			}
			//			else
			//			{
			//				Vec2 combinedVelocity = entityRb2->velocity + entityRb->velocity;
			//				if (entityRb2->velocity.lengthSquared() < entityRb->velocity.lengthSquared())
			//				{
			//					entityRb->velocity = Vec2(0.f, 0.f);
			//					entityRb2->velocity = 3 * combinedVelocity / 4;
			//				}
			//				else
			//				{
			//					// If circle velocity is greater, box will gain more velocity
			//					entityRb->velocity = 3 * combinedVelocity / 4;
			//					entityRb2->velocity = Vec2(0.f, 0.f);
			//				}

			//				/****************************************************************
			//				* @brief Check for collisions between circles
			//				*
			//				* This section checks for collisions between the moving circle
			//				* and other circles, handling their collision responses.
			//				****************************************************************/
			//				for (const auto& [entity3Id, circleColliderComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
			//				{
			//					if (entity2Id == entity3Id || entity3Id == entityId) continue;

			//					BoundingCircle* entityCircle2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity3Id);
			//					TransformComponent* circleTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity3Id);
			//					RigidbodyComponent* circleRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity3Id);

			//					if (!circleTrans2) continue;

			//					if (!circleRb2)
			//					{
			//						if (checkCollisionCC(*entityCircle, *entityCircle2, deltaTime, entityRb2->velocity))
			//						{
			//							if (!entityCircle2->isKinematic)
			//							{
			//								continue;
			//							}
			//							entityRb2->velocity = Vec2(0.0f, 0.0f);
			//							entityRb->velocity = Vec2(0.0f, 0.0f);
			//						}
			//					}
			//					else
			//					{
			//						if (checkCollisionCC(*entityCircle, *entityCircle2, deltaTime, entityRb2->velocity, circleRb2->velocity))
			//						{
			//							if (!entityCircle2->isKinematic)
			//							{
			//								continue;
			//							}
			//							// If there is a collision
			//							if (circleRb2->isStatic)	// If the circle is static
			//							{
			//								entityRb2->velocity = Vec2(0.0f, 0.0f);
			//								entityRb->velocity = Vec2(0.0f, 0.0f);
			//							}
			//							else // If the circle is not static
			//							{
			//								combinedVelocity = entityRb2->velocity + circleRb2->velocity;
			//								if (entityRb2->velocity.lengthSquared() < circleRb2->velocity.lengthSquared())
			//								{
			//									circleRb2->velocity = Vec2(0.f, 0.f);
			//									entityRb2->velocity = 3 * combinedVelocity / 4;
			//								}
			//								else
			//								{
			//									// If circle velocity is greater, box will gain more velocity
			//									circleRb2->velocity = 3 * combinedVelocity / 4;
			//									entityRb2->velocity = Vec2(0.f, 0.f);
			//								}

			//								entityRb->velocity = Vec2(0.0f, 0.0f);
			//							}
			//						}
			//					}
			//				}
			//			}
			//		}
			//	}

			//}
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

			//Vec2 initialSize = entityBox->getInitialSize();
			//Vec2 scaledSize = Vec2(initialSize.x * entityTrans->scale.x,
			//	initialSize.y * entityTrans->scale.y);
			//entityBox->setSize(scaledSize);

			entityBox->setRotation(entityTrans->angle);
		}
	}
}