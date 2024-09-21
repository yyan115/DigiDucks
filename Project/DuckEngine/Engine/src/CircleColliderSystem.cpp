#include "CircleColliderSystem.h"

void CircleColliderSystem::Update() {
	float deltaTime = DuckEngine::DeltaTime();
	// Find player's circle collider
	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		// Only 1 circle collider in this scene
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);
		TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		RigidbodyComponent* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityId);

		circle->setCenter(transform->position);
		Vec2 nextPos;

		// Check collision with box collider
		for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
		{
			BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);

			nextPos = circle->getCenter() + rigidbody->velocity * deltaTime;
			// Check collision
			if(checkCollisionCB(*circle, nextPos, *box)) {
				// If there is a collision, reset the player's velocity
				rigidbody->velocity = Vec2(0.0f, 0.0f);
			}
		}

		// Update player's position based on velocity
		//transform->position = transform->position + rigidbody->velocity * deltaTime;
		circle->setCenter(transform->position);

	}
}