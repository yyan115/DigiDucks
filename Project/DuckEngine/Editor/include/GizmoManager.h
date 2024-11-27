/******************************************************************************/
/*!
\file       GizmoManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 28 2024
\brief      Declares the GizmoManager class, providing functionality for 
            manipulating game objects through gizmos, including translation, 
            scaling, and rotation. This class handles gizmo rendering, input 
            processing, and interaction logic.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Vector2.h"
#include "Color.h"

struct GizmoData;

enum CurrentGizmo {
    TRANSLATE = 1,
    SCALE,
    ROTATE
};

class GizmoManager {
public:
    static CurrentGizmo currentGizmo;
    static int activeGizmoHandle; // -1: none, 0: x-move, 1: y-move, 2: x-scale, 3: y-scale, 4: rotate
    static bool isDraggingGizmo;

    /// <summary>
    /// Initializes the GizmoManager, setting up initial states and resources.
    /// </summary>
    static void Initialize();

    /// <summary>
    /// Updates the current gizmo state based on user input and selected entity.
    /// </summary>
    static void Update();

    /// <summary>
    /// Renders the gizmos, displaying handles for translation, scaling, and rotation.
    /// </summary>
    static void Render();

    /// <summary>
    /// Sets the currently selected entity for gizmo interaction.
    /// </summary>
    /// <param name="entityID">The ID of the selected entity.</param>
    static void SetSelectedEntity(int entityID);

    /// <summary>
    /// Checks if the mouse is over a gizmo handle and updates the handle index.
    /// </summary>
    /// <param name="mouseWorldPosition">The mouse position in world space.</param>
    /// <param name="gizmoData">Data about the gizmo's position and size.</param>
    /// <param name="outHandleIndex">Outputs the index of the gizmo handle being hovered.</param>
    /// <returns>True if the mouse is over a gizmo handle, false otherwise.</returns>
    static bool IsMouseOverGizmoHandle(const Vector2D& mouseWorldPosition, const GizmoData& gizmoData, int& outHandleIndex);

private:
    static int selectedEntityID;

    static Vector2D initialMouseWorldPosition;
    static Vector2D initialObjectPosition;
    static Vector2D initialScale;
    static float initialRotation;

    /// <summary>
    /// Handles user interaction with gizmo handles for translation, scaling, and rotation.
    /// </summary>
    static void HandleGizmoInteraction();
};
