/******************************************************************************/
/*!
\file       GizmoManager.h
\author     Your Name
\brief      Declares the GizmoManager class responsible for handling gizmo interaction,
            including drawing gizmos and processing user input for moving, scaling,
            and rotating game objects.

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

    static void Initialize();
    static void Update();
    static void Render();

    // Call this function to update the selected entity ID
    static void SetSelectedEntity(int entityID);
    static bool IsMouseOverGizmoHandle(const Vector2D& mouseWorldPosition, const GizmoData& gizmoData, int& outHandleIndex);
private:
    static int selectedEntityID;

    static Vector2D initialMouseWorldPosition;
    static Vector2D initialObjectPosition;
    static Vector2D initialScale;
    static float initialRotation;

    static void HandleGizmoInteraction();
};
