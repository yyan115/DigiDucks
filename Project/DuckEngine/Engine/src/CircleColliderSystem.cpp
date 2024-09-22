#include "CircleColliderSystem.h"

void CircleColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	// Find player's circle collider
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		// Only 1 circle collider in this scene
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* c_transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* c_rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		circle->setCenter(c_transform->position);
		Vec2 nextPos;

		// Check collision with box collider
		for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);

			nextPos = circle->getCenter() + c_rigidbody->velocity * deltaTime;
			// Check collision
			if(checkCollisionCB(*circle, nextPos, *box)) {
				// If there is a collision, reset the player's velocity
				c_rigidbody->velocity = Vec2(0.0f, 0.0f);
			}
		}

		// Update player's position based on velocity
		//transform->position = transform->position + rigidbody->velocity * deltaTime;
		circle->setCenter(c_transform->position);

	}
}