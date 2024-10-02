/******************************************************************************/
/*!
\file       CameraManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the CameraManager class, handling the initialization
			and updating of the camera's position and height for rendering
			scenes in the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CameraManager.h"
#include "WindowManager.h"

// Initialize static member variables for camera position, height, and window aspect ratio
Vector2D CameraManager::position;
int CameraManager::cameraHeight;
float CameraManager::windowAspectRatio;

/// <summary>
/// Initializes the camera with a specified position and height. 
/// Also calculates the window's aspect ratio based on the current window dimensions.
/// </summary>
/// <param name="posX">The initial x-coordinate of the camera in world space.</param>
/// <param name="posY">The initial y-coordinate of the camera in world space.</param>
/// <param name="height">The initial height of the camera's view.</param>
void CameraManager::Initialize(const float posX, const float posY, const int height) {
	CameraManager::position = { posX, posY };
	CameraManager::cameraHeight = height;
	CameraManager::windowAspectRatio = static_cast<float>(WindowManager::GetWindowWidth() / WindowManager::GetWindowHeight());
}

/// <summary>
/// Sets the camera's position to the specified x and y coordinates in world space.
/// </summary>
/// <param name="x">The new x-coordinate of the camera in world space.</param>
/// <param name="y">The new y-coordinate of the camera in world space.</param>
void CameraManager::SetPosition(const float x, const float y) {
	CameraManager::position = { x, y };
}

/// <summary>
/// Sets the camera's height to the specified value, determining the vertical size of the camera's view.
/// </summary>
/// <param name="height">The new height of the camera's view.</param>
void CameraManager::SetHeight(const int height) {
	CameraManager::cameraHeight = height;
}

/// <summary>
/// Updates the camera's internal state. This function may be used to perform any calculations 
/// or adjustments necessary as the camera's position or height changes over time.
/// </summary>
void CameraManager::Update() {
	// Empty for now.
}