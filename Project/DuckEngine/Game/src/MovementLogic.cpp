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

	if (isMoving)
	{
		// Keyboard input
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

		// Gamepad input
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
		{
			// Left analog stick for movement
			float leftStickX = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
			float leftStickY = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);

			// Apply deadzone to prevent drift
			const float deadzone = 0.05f;
			if (std::abs(leftStickX) > deadzone)
			{
				inputDirection.x += leftStickX;
			}
			if (std::abs(leftStickY) > deadzone)
			{
				// Invert Y axis because GLFW reports positive values when pushing down
				inputDirection.y -= leftStickY;
			}

			// D-pad input as an alternative
			if (DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP))
			{
				inputDirection.y += 1.0f;
			}
			if (DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN))
			{
				inputDirection.y -= 1.0f;
			}
			if (DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT))
			{
				inputDirection.x -= 1.0f;
			}
			if (DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT))
			{
				inputDirection.x += 1.0f;
			}
		}
	}

	// Normalize the input direction if it's not zero
	if (inputDirection.x != 0.0f || inputDirection.y != 0.0f)
	{
		inputDirection = inputDirection.normalized();
	}

	// Set velocity based on normalized input
	rigidbody->velocity = inputDirection * moveSpeed;

	// Handle particle emissions if moving
	if (isMoving && (inputDirection.x != 0.0f || inputDirection.y != 0.0f))
	{
		Vector2D moveSmokePosition = transform->GetPosition();

		// Calculate smoke position based on movement direction
		// Use a single consistent method that works for both keyboard and gamepad

		// Determine the primary and secondary movement directions
		bool movingUp = inputDirection.y > 0.5f;
		bool movingDown = inputDirection.y < -0.5f;
		bool movingLeft = inputDirection.x < -0.5f;
		bool movingRight = inputDirection.x > 0.5f;
		bool movingDiagonal = (std::abs(inputDirection.x) > 0.3f && std::abs(inputDirection.y) > 0.3f);

		if (movingDiagonal)
		{
			// TOP RIGHT
			if (movingUp && movingRight)
			{
				moveSmokePosition.x += -0.4f;
				moveSmokePosition.y += -1.1f;
			}
			// BOTTOM RIGHT
			else if (movingDown && movingRight)
			{
				moveSmokePosition.x += -0.4f;
				moveSmokePosition.y += -1.0f;
			}
			// BOTTOM LEFT
			else if (movingDown && movingLeft)
			{
				moveSmokePosition.x += 0.6f;
				moveSmokePosition.y += -1.0f;
			}
			// TOP LEFT
			else if (movingUp && movingLeft)
			{
				moveSmokePosition.x += 0.6f;
				moveSmokePosition.y += -1.35f;
			}
		}
		else
		{
			// Single direction movement
			if (movingUp)
			{
				moveSmokePosition.y -= 1.3f;
			}
			else if (movingDown)
			{
				moveSmokePosition.y += 0.5f;
			}
			else if (movingLeft)
			{
				moveSmokePosition.x += 0.5f;
				moveSmokePosition.y -= 1.1f;
			}
			else if (movingRight)
			{
				moveSmokePosition.x -= 0.5f;
				moveSmokePosition.y -= 1.1f;
			}
		}

		DuckEngine::Emit("Dust", moveSmokePosition, -rigidbody->velocity);
	}
}