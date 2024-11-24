#include "MovementLogic.h"
#include <iostream>
#include <cmath>

BoundingCircle* circleCollider;

void MovementLogic::Start()
{
	// Initialize the components from the GameLogicComponent's owner entity
	transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(component->GetEntityID());
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
}

void MovementLogic::Update()
{
	
}

void MovementLogic::FixedUpdate()
{
	if (!transform || !rigidbody || !animator)
	{
		return;
	}

	float moveSpeed = 10.0f;

	// Reset the player's velocity at the start of each fixed update
	rigidbody->velocity = Vec2(0.f, 0.f);

	// Store input state - don't directly modify velocity
	Vector2D inputDirection(0.0f, 0.0f);

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		inputDirection.y += 1.0f;
		animator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		inputDirection.y -= 1.0f;
		animator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		inputDirection.x -= 1.0f;
		animator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		inputDirection.x += 1.0f;
		animator->PlayAnimation("WalkAnimation");
	}

	// Normalize the input direction if it's not zero
	if (inputDirection.x != 0.0f || inputDirection.y != 0.0f)
	{
		float length = std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
		inputDirection.x /= length;
		inputDirection.y /= length;
	}

	// Set velocity based on normalized input
	rigidbody->velocity = inputDirection * moveSpeed;


	if (rigidbody->velocity.x == 0.0f && rigidbody->velocity.y == 0.0f)
	{
		animator->PlayAnimation("IdleAnimation");
	}
}