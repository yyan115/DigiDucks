/******************************************************************************/
/*!
\file		CircleColliderSystem.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Oct 28 2024
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
void CircleColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	Vec2 interceptPoint;

	// Find player's circle collider
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityId);

		// Only 1 circle collider in this scene
		BoundingCircle* entityCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity->entityID);
		TransformComponent* entityTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID);
		RigidbodyComponent* entityRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity->entityID);

		if(!entityRb) continue;	// No rb = not moving

		// Update Collider to current position
		entityCircle->setCenter(entityTrans->position);

		/****************************************************************
		* @brief Check collisions between this circle and other circles
		*
		* This section iterates through all BoundingCircle components to
		* check for collisions with the current circle collider. Collision
		* responses are calculated based on the velocities of the circles.
		****************************************************************/
		for (const auto& [entity2Id, circleColliderComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
		{
			if (entityId == entity2Id) continue;

			BoundingCircle* entityCircle2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entity2Id);
			TransformComponent* entityTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* entityRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);

			// Ensure the entity has both BoundingCircle and RigidbodyComponent
			if (!entityTrans2) continue;

			if (!entityRb2) { // Consider static
				if (checkCollisionCC(*entityCircle, *entityCircle2, deltaTime, entityRb->velocity)) {
					entityRb->velocity = Vec2(0.0f, 0.0f);
				}
			}
			else {
				// Check collision
				if (checkCollisionCC(*entityCircle, *entityCircle2, deltaTime, entityRb->velocity, entityRb2->velocity)) {
					// If there is a collision
					if (!entityRb2 || entityRb2->isStatic) {	// If the circle is static
						entityRb->velocity = Vec2(0.0f, 0.0f);
					}
					else { // If the circle is not static
						Vec2 combinedVelocity = entityRb->velocity + entityRb2->velocity;
						if (entityRb->velocity.lengthSquared() < entityRb2->velocity.lengthSquared()) {
							entityRb2->velocity = Vec2(0.f, 0.f);
							entityRb->velocity = 3 * combinedVelocity / 4;
						}
						else {
							// If circle velocity is greater, box will gain more velocity
							entityRb2->velocity = 3 * combinedVelocity / 4;
							entityRb->velocity = Vec2(0.f, 0.f);
						}

						/****************************************************************
						* @brief Check for collisions between the second circle and boxes
						*
						* This section checks for collisions between the current circle collider
						* and all BoundingBox components, handling their collision responses.
						****************************************************************/
						for (const auto& [entity3Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
						{
							if (entity3Id == entity2Id || entity3Id == entityId) continue;

							Entity* entity3 = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entity2Id);
							if (entity3->name == "Player") continue;

							BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity3Id);
							//TransformComponent* boxTrans = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity3Id);
							RigidbodyComponent* boxRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity3Id);

							if (!boxRb) {	// One of the box is moving
								if (checkCollisionCB(*entityCircle2, *box, interceptPoint, deltaTime, entityRb2->velocity)) {
									entityRb2->velocity = Vec2(0.0f, 0.0f);
									entityRb->velocity = Vec2(0.0f, 0.0f);
								}
							}
							else {
								if (checkCollisionCB(*entityCircle2, *box, interceptPoint, deltaTime, entityRb2->velocity, boxRb->velocity)) {
									// If there is a collision
									if (boxRb->isStatic) {	// If the box is static
										entityRb2->velocity = Vec2(0.0f, 0.0f);
										entityRb->velocity = Vec2(0.0f, 0.0f);
									}
									else { // If the box is not static
										combinedVelocity = entityRb2->velocity + boxRb->velocity;
										if (entityRb2->velocity.lengthSquared() < boxRb->velocity.lengthSquared()) {
											boxRb->velocity = Vec2(0.f, 0.f);
											entityRb2->velocity = 3 * combinedVelocity / 4;
										}
										else {
											// If circle velocity is greater, box will gain more velocity
											boxRb->velocity = 3 * combinedVelocity / 4;
											entityRb2->velocity = Vec2(0.f, 0.f);
										}

										entityRb->velocity = Vec2(0.0f, 0.0f);
									}
								}
							}

						}
					}
				}
			}
		}

		/****************************************************************
		* @brief Check collisions between this circle and boxes
		*
		* This section checks for collisions between the circle collider and
		* all BoundingBox components, handling their collision responses.
		****************************************************************/
		for (const auto& [entity2Id, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			if(entityId == entity2Id) continue;

			Entity* entity2 = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entity2Id);
			if (entity2->name == "Player") continue;

			BoundingBox* entityBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity2Id);
			TransformComponent* entityTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity2Id);
			RigidbodyComponent* entityRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity2Id);
			
			// Ensure the entity has both BoundingBox and RigidbodyComponent
			if(!entityTrans2) continue;


			// Check collision
			if (!entityRb2) {
				if (checkCollisionCB(*entityCircle, *entityBox, interceptPoint, deltaTime, entityRb->velocity)) {
					calculateNewVelocity(entityCircle->getCenter(), entityRb->velocity, interceptPoint, entityCircle->getRadius(), deltaTime);
				}
			}
			else{
				if (checkCollisionCB(*entityCircle, *entityBox, interceptPoint,  deltaTime, entityRb->velocity, entityRb2->velocity)) {
					// If there is a collision
					if (entityRb2->isStatic) {	// If the box is static
						entityRb->velocity = Vec2(0.0f, 0.0f);

						// Trigger sound on collision
						if (auto* soundComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(entity2Id)) {
							if (!soundComponent->IsSoundPlaying()) {
								soundComponent->Play();
							}
						}
					}
					else { // If the box is not static
						Vec2 combinedVelocity = entityRb->velocity + entityRb2->velocity;
						if (entityRb->velocity.lengthSquared() < entityRb2->velocity.lengthSquared()) {
							entityRb2->velocity = Vec2(0.f, 0.f);
							entityRb->velocity = 3 * combinedVelocity / 4;
						}
						else {
							// If circle velocity is greater, box will gain more velocity
							entityRb2->velocity = 3 * combinedVelocity / 4;
							entityRb->velocity = Vec2(0.f, 0.f);
						}

						// Trigger sound on collision
						if (auto* soundComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(entity2Id)) {
							if (!soundComponent->IsSoundPlaying()) {
								soundComponent->Play();
							}
						}

						/****************************************************************
						* @brief Check for collisions between boxes
						*
						* This section checks for collisions between the collided box
						* and other boxes, handling their collision responses.
						****************************************************************/
						for (const auto& [entity3Id, _boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
						{
							if (entity2Id == entity3Id || entity3Id == entityId) continue;
							BoundingBox* box2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entity3Id);
							//TransformComponent* boxTrans2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity3Id);
							RigidbodyComponent* boxRb2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entity3Id);

							if (!boxRb2) {	// One of the box is moving
								if (checkCollisionBB(*entityBox, *box2, deltaTime, entityRb2->velocity)) {
									entityRb2->velocity = Vec2(0.0f, 0.0f);
									entityRb->velocity = Vec2(0.0f, 0.0f);
								}
							}
							else {
								if (checkCollisionBB(*entityBox, *box2, deltaTime, entityRb2->velocity, boxRb2->velocity)) {
									// If there is a collision
									if (boxRb2->isStatic) {	// If the box is static
										entityRb2->velocity = Vec2(0.0f, 0.0f);
										entityRb->velocity = Vec2(0.0f, 0.0f);
									}
									else { // If the box is not static
										combinedVelocity = entityRb2->velocity + boxRb2->velocity;
										if (entityRb2->velocity.lengthSquared() < boxRb2->velocity.lengthSquared()) {
											boxRb2->velocity = Vec2(0.f, 0.f);
											entityRb2->velocity = combinedVelocity;
										}
										else {
											boxRb2->velocity = combinedVelocity;
											entityRb2->velocity = Vec2(0.f, 0.f);
										}

										entityRb->velocity = Vec2(0.0f, 0.0f);
									}
								}
							}
						}
					}
				}
			}
		}
		// Update Collider's position based on velocity
		entityCircle->setCenter(entityTrans->position + entityCircle->getOffSet());
	}
}

void CircleColliderSystem::Render() {
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);

		DuckEngine::DrawCircle(circle->getCenter() + circle->getOffSet(), circle->getRadius());
	}
}