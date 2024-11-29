#include "MovementLogic.h"
#include <iostream>
#include <cmath>

void MovementLogic::Start()
{
	// Initialize the components from the GameLogicComponent's owner entity
	transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(component->GetEntityID());
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
	moveSpeed = 10.f;
	dir = FRONT;
}

void MovementLogic::Update()
{
	
}

void MovementLogic::FixedUpdate()
{
	if (!transform || !rigidbody)
	{
		return;
	}

	// Reset the player's velocity at the start of each fixed update
	rigidbody->velocity = Vec2(0.f, 0.f);

	// Store input state - don't directly modify velocity
	Vector2D inputDirection(0.0f, 0.0f);

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		inputDirection.y += 1.0f;
		if (animator) 
		{
			animator->PlayAnimation("BACK_WALK");
			dir = BACK;
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		inputDirection.y -= 1.0f;
		if (animator)
		{
			animator->PlayAnimation("FRONT_WALK");
			dir = FRONT;
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		inputDirection.x -= 1.0f;
		if (animator)
		{
			animator->PlayAnimation("LEFT_WALK");
			dir = LEFT;
		}
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		inputDirection.x += 1.0f;
		if (animator)
		{
			animator->PlayAnimation("RIGHT_WALK");
			dir = RIGHT;
		}
	}

	// Normalize the input direction if it's not zero
	inputDirection = inputDirection.normalized();

	// Set velocity based on normalized input
	rigidbody->velocity = inputDirection * moveSpeed;


	if (animator)
	{
		if (rigidbody->velocity.x == 0.0f && rigidbody->velocity.y == 0.0f)
		{
			if (dir == FRONT)
				animator->PlayAnimation("FRONT_IDLE");
			else if (dir == BACK)
				animator->PlayAnimation("BACK_IDLE");
			else if (dir == LEFT)
				animator->PlayAnimation("LEFT_IDLE");
			else if (dir == RIGHT)
				animator->PlayAnimation("RIGHT_IDLE");
		}
	}
}

