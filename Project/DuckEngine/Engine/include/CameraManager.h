/******************************************************************************/
/*!
\file       CameraManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Declares the CameraManager class, responsible for controlling the
            camera position, height, and aspect ratio, used in rendering scenes
            from the correct viewpoint in the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include "Matrix3x3.h"
#include "Vector2.h"

/// <summary>
/// Manages the camera's position and height in world space, as well as the window's aspect ratio.
/// Provides functions for initializing and updating the camera's properties.
/// </summary>
class DUCKENGINE_API CameraManager {
public:

    /// <summary>
    /// Initializes the camera's position and height.
    /// </summary>
    /// <param name="posX">The initial x-coordinate of the camera in world space.</param>
    /// <param name="posY">The initial y-coordinate of the camera in world space.</param>
    /// <param name="height">The initial height of the camera's view in the world.</param>
    static void Initialize(const float posX, const float posY, const int height);

    /// <summary>
    /// Sets the camera's position in world space.
    /// </summary>
    /// <param name="posX">The new x-coordinate of the camera in world space.</param>
    /// <param name="posY">The new y-coordinate of the camera in world space.</param>
    static void SetPosition(const float posX, const float posY);
    static void LerpCameraTo(const float posX, const float posY);

    /// <summary>
    /// Sets the camera's height, which determines the vertical size of the camera's view.
    /// </summary>
    /// <param name="height">The new height of the camera's view.</param>
    static void SetHeight(const int height);

    /// <summary>
    /// Gets the current position of the camera in world space.
    /// </summary>
    /// <returns>The camera's position as a 2D vector.</returns>
    static inline Vector2D GetPosition() { return position; };
    static Vector2D GetPreviousPosition() { return previousPosition; };

    /// <summary>
    /// Gets the window's current aspect ratio (width/height).
    /// </summary>
    /// <returns>The window's aspect ratio.</returns>
    static inline float GetAR() { return windowAspectRatio; };

    /// <summary>
    /// Gets the current height of the camera's view.
    /// </summary>
    /// <returns>The height of the camera's view.</returns>
    static inline float GetHeight() { return static_cast<float>(cameraHeight); };

    /// <summary>
    /// Adjusts the height of the camera's view by a specified delta value.
    /// </summary>
    /// <param name="delta">The amount to adjust the camera's height by.</param>
    static void AdjustHeight(int delta);

    /// <summary>
    /// Moves the camera by specified x and y delta values.
    /// </summary>
    /// <param name="deltaX">The amount to move the camera along the X-axis.</param>
    /// <param name="deltaY">The amount to move the camera along the Y-axis.</param>
    static void MoveCamera(float deltaX, float deltaY);

    /// <summary>
    /// Updates the camera's internal state. This method may include calculations or adjustments based on the current camera properties.
    /// </summary>
    static void Update();

    static void ResetToDefault();
private:
    /// <summary>
    /// The camera's position in world space as a 2D vector.
    /// </summary>
    static Vector2D position;
    static Vector2D previousPosition;

    /// <summary>
    /// The camera's height, representing the vertical size of the camera's view.
    /// </summary>
    static int cameraHeight;

    /// <summary>
    /// The window's aspect ratio, representing the width/height ratio.
    /// </summary>
    static float windowAspectRatio;

    static int defaultCameraHeight;

    static Vector2D targetCameraPosition; // Target camera position
    static float cameraLerpSpeed;        // Speed of interpolation
};
