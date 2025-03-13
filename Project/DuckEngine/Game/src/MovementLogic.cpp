/******************************************************************************/
/*!
\file       MovementLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 13 2025
\brief      Declartion of all Movement Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "MovementLogic.h"
#include <iostream>
#include <cmath>

/****************************************************************
* @brief Start function for the Movement Logic
* ****************************************************************/
void MovementLogic::Start()
{
	// Initialize the components from the GameLogicComponent's owner entity
	transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(component->GetEntityID());
	rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(component->GetEntityID());
	animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(component->GetEntityID());
	moveSpeed = 10.f;
}

/****************************************************************
* @brief Update function for the Movement Logic
* ****************************************************************/
void MovementLogic::Update()
{
	
}

/****************************************************************
* @brief FixedUpdate function for the Movement Logic
* ****************************************************************/
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

	//Vector2D moveSmokePosition;

	if (isMoving)
	{
		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
		{
			inputDirection.y += 1.0f;
		}
		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
		{
			inputDirection.y -= 1.0f;
		}
		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
		{
			inputDirection.x -= 1.0f;
		}
		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
		{
			inputDirection.x += 1.0f;
		}
	}

	// Normalize the input direction if it's not zero
	inputDirection = inputDirection.normalized();

	// Set velocity based on normalized input
	rigidbody->velocity = inputDirection * moveSpeed;

	if (isMoving)
	{
		Vector2D moveSmokePosition = transform->GetPosition();

		// DIAGONALS

		// TOP RIGHT
		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D)) {
			moveSmokePosition.x += -0.4f;
			moveSmokePosition.y += -1.1f;
		}

		// BOTTOM RIGHT
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D)) {
			moveSmokePosition.x += -0.4f;
			moveSmokePosition.y += -1.0f;
		}

		// BOTTOM LEFT
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S)) {
			moveSmokePosition.x += 0.6f;
			moveSmokePosition.y += -1.0f;
		}

		// TOP LEFT
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W)) {
			moveSmokePosition.x += 0.6f;
			moveSmokePosition.y += -1.35f;
		}

		// TOP DOWN LEFT RIGHT
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W)) {
			moveSmokePosition.y -= 1.3f;
		}
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S)) {
			moveSmokePosition.y += 0.5f;
		}
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A)) {
			moveSmokePosition.x += 0.5f;
			moveSmokePosition.y -= 1.1f;
		}
		else if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D)) {
			moveSmokePosition.x -= 0.5f;
			moveSmokePosition.y -= 1.1f;
		}

		DuckEngine::Emit("Dust", moveSmokePosition, -rigidbody->velocity);
	}
}

