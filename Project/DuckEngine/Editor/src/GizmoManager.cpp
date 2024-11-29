/******************************************************************************/
/*!
\file       GizmoManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 28 2024
\brief      Implements the GizmoManager class, managing gizmo interactions
            such as translation, scaling, and rotation of game objects.
            Handles user input, rendering of gizmos, and maintaining state
            for active gizmo handles and dragging operations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/

/******************************************************************************/

#include "GraphicsManager.h"
#include "GizmoManager.h"
#include "CameraManager.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "UIManager.h" // Include to access selectedEntityID
#include "SceneWindow.h"
#include "SnapshotManager.h"

// Initialize static member variables
int GizmoManager::selectedEntityID = -1;
bool GizmoManager::isDraggingGizmo = false;
int GizmoManager::activeGizmoHandle = -1;
Vector2D GizmoManager::initialMouseWorldPosition;
Vector2D GizmoManager::initialObjectPosition;
Vector2D GizmoManager::initialScale;
float GizmoManager::initialRotation = 0.0f;
CurrentGizmo GizmoManager::currentGizmo = CurrentGizmo::TRANSLATE;

static bool alreadyClicked = false;

DrawOptions xAxisLineArrow;       // X-axis line arrow
DrawOptions yAxisLineArrow;       // Y-axis line arrow

const float PI = 3.14159265f;
const float RAD2DEG = 180.0f / PI;
const float DEG2RAD = PI / 180.0f;

CurrentGizmo previousGizmo;

void GizmoManager::Initialize() {

}

void GizmoManager::Update() {

    previousGizmo = GizmoManager::currentGizmo;

    if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_7)) {
        GizmoManager::currentGizmo = CurrentGizmo::TRANSLATE;
    }
    else if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_8)) {
        GizmoManager::currentGizmo = CurrentGizmo::SCALE;
    }
    else if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_9)) {
        GizmoManager::currentGizmo = CurrentGizmo::ROTATE;
    }

    GraphicsManager::currentGizmo = GizmoManager::currentGizmo;
}

void GizmoManager::Render() {

    if (!SceneWindow::GetIsPlaying()) {
        selectedEntityID = UIManager::selectedEntityID;

        auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
        if (!transform) {
            GraphicsManager::entityIsSelected = false;
        }
        else {
            float biggestScale = transform->scale.x > transform->scale.y ? transform->scale.x : transform->scale.y;
            GraphicsManager::gizmoData = { transform->GetPosition(), biggestScale }; // Adjust size as needed
            GraphicsManager::entityIsSelected = true;

            //if (currentGizmo == CurrentGizmo::TRANSLATE) {
            //    GraphicsManager::AddToDrawQueue(xAxisLineArrow);
            //    GraphicsManager::AddToDrawQueue(yAxisLineArrow);
            //}
            //else if (currentGizmo == CurrentGizmo::SCALE) {
            //    GraphicsManager::AddToDrawQueue(xAxisLineArrow);
            //    GraphicsManager::AddToDrawQueue(yAxisLineArrow);
            //}

            HandleGizmoInteraction();
        }
    }
    else {
        GraphicsManager::entityIsSelected = false;
    }
}

void GizmoManager::SetSelectedEntity(int entityID) {
    selectedEntityID = entityID;
}

//Vector2D previousPosition;

void GizmoManager::HandleGizmoInteraction() {
    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);

    Vector2D mouseWorldPosition = DuckEngine::editorMouseWorldPos;

    // DRAWS MOUSE POS, FOR DEBUGGING ONLY
    //DrawOptions drawoptionMousePos;

    //drawoptionMousePos.color = Color(255.f, 255.f, 255.f, 255.f); // Red color
    //drawoptionMousePos.relativeToCamera = true;
    //drawoptionMousePos.rotation = 0.f;
    //drawoptionMousePos.scale = 1.f; // Set thickness
    //drawoptionMousePos.useColor = true;
    //drawoptionMousePos.useTexture = false;
    //drawoptionMousePos.translation = mouseWorldPosition;

    //GraphicsManager::AddToDrawQueue(drawoptionMousePos);

    if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
        // Check if mouse is over the currently active gizmo handle
        if (IsMouseOverGizmoHandle(mouseWorldPosition, GraphicsManager::gizmoData, activeGizmoHandle)) {
            isDraggingGizmo = true;
            initialMouseWorldPosition = mouseWorldPosition;
            initialObjectPosition = transform->GetPosition();
            initialScale = transform->scale;
            initialRotation = transform->angle;

            SnapshotManager::SaveUndoState();
        }
        else {
            // DO NOTHING
        }
    }

    if (DuckEngine_Input::IsMouseButtonReleased(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {

        if (isDraggingGizmo) {
            auto* transformComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
            if (transformComponent) {
                if (transformComponent->GetPosition() == initialObjectPosition && transformComponent->scale == initialScale && transformComponent->angle == initialRotation) {
                    std::cout << "Object didn't moved! so dont save state" << std::endl;
                    SnapshotManager::RemoveLatestUndoState();
                }
            }
        }

        isDraggingGizmo = false;
        activeGizmoHandle = -1;
    }

    if (isDraggingGizmo && activeGizmoHandle != -1 && DuckEngine_Input::IsMouseButtonDown(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
        Vector2D delta = mouseWorldPosition - initialMouseWorldPosition;

        switch (activeGizmoHandle) {
        case 0: // x-axis move
            transform->SetPosition(initialObjectPosition + Vector2D(delta.x, 0.0f));
            break;
        case 1: // y-axis move
            transform->SetPosition(initialObjectPosition + Vector2D(0.0f, delta.y));
            break;
        case 2: { // x-axis scale
            Vector2D newScale = initialScale + Vector2D(delta.x, 0.0f);
            if (newScale.x < 0.1f) newScale.x = 0.1f; // Prevent negative or too small scale
            transform->scale = newScale;
            break;
        }
        case 3: { // y-axis scale
            Vector2D newScale = initialScale + Vector2D(0.0f, delta.y);
            if (newScale.y < 0.1f) newScale.y = 0.1f;
            transform->scale = newScale;
            break;
        }
        case 4: { // rotation
            Vector2D center = GraphicsManager::gizmoData.position;
            Vector2D startDir = initialMouseWorldPosition - center;
            Vector2D currentDir = mouseWorldPosition - center;

            float startAngle = atan2(startDir.y, startDir.x);
            float currentAngle = atan2(currentDir.y, currentDir.x);

            float angleDeltaRadians = currentAngle - startAngle;
            // Convert radians to degrees
            float angleDeltaDegrees = angleDeltaRadians * RAD2DEG;

            transform->angle = initialRotation + angleDeltaDegrees;
            //std::cout << "angle: " << transform->angle << "\n";
            break;
        }
        default:
            break;
        }
    }
}

bool GizmoManager::IsMouseOverGizmoHandle(const Vector2D& mouseWorldPosition, const GizmoData& gizmoData, int& outHandleIndex) {

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
    if (!transform) return false;

    Vector2D transformSize = transform->scale; // Size of the game object

    float THICKNESS = 1.5f; // Adjust thickness as needed
    Vector2D halfTransformSize = transformSize * 0.5f;

    //// Calculate half sizes of the transform
    //float halfEntityWidth = transformSize.x * 0.5f;
    //float halfEntityHeight = transformSize.y * 0.5f;

    // NOTE: SCALE'S LENGTH IS 0.5F LONGER!!!!!

    //// ------------------------------
    //// X-Axis Translate Handle Rectangle
    //// ------------------------------
    //{
    //    // Length of the protruding part
    //    float xHandleLength = gizmoData.size - halfEntityWidth;
    //    float halfXHandleLength = xHandleLength * 0.5f;

    //    // Set scale (length and thickness)
    //    xAxisLineArrow.scale = Vector2D(xHandleLength, THICKNESS);

    //    // Set translation (center of the protruding part)
    //    xAxisLineArrow.translation = gizmoData.position + Vector2D(halfEntityWidth + halfXHandleLength, 0.0f);

    //    xAxisLineArrow.color = Color(255.f, 0.f, 0.f, 255.f); // Red color for X-axis
    //    xAxisLineArrow.relativeToCamera = true;
    //    xAxisLineArrow.rotation = 0.f; // No rotation needed
    //    xAxisLineArrow.useColor = true;
    //    xAxisLineArrow.useTexture = false;
    //}

    //// ------------------------------
    //// Y-Axis Translate Handle Rectangle
    //// ------------------------------
    //{
    //    // Length of the protruding part
    //    float yHandleLength = gizmoData.size - halfEntityHeight;
    //    float halfYHandleLength = yHandleLength * 0.5f;

    //    // Set scale (thickness and length)
    //    yAxisLineArrow.scale = Vector2D(THICKNESS, yHandleLength);

    //    // Set translation (center of the protruding part)
    //    yAxisLineArrow.translation = gizmoData.position + Vector2D(0.0f, halfEntityHeight + halfYHandleLength);

    //    yAxisLineArrow.color = Color(0.f, 255.f, 0.f, 255.f); // Green color for Y-axis
    //    yAxisLineArrow.relativeToCamera = true;
    //    yAxisLineArrow.rotation = 0.f; // No rotation needed
    //    yAxisLineArrow.useColor = true;
    //    yAxisLineArrow.useTexture = false;
    //}


    if (currentGizmo == CurrentGizmo::TRANSLATE) {
        // X-axis move handle (rectangle for protruding line)
        {
            Vector2D xStart = gizmoData.position + Vector2D(halfTransformSize.x, 0.0f); // Start at the edge of the object
            Vector2D xEnd = gizmoData.position + Vector2D(gizmoData.size, 0.0f);       // End of the handle

            // Define rectangle bounds
            float halfThickness = THICKNESS / 2.0f;

            float minX = xStart.x;
            float maxX = xEnd.x;
            float minY = xStart.y - halfThickness;
            float maxY = xStart.y + halfThickness;

            // Check if mouse is within rectangle
            if (mouseWorldPosition.x >= minX && mouseWorldPosition.x <= maxX &&
                mouseWorldPosition.y >= minY && mouseWorldPosition.y <= maxY) {
                outHandleIndex = 0; // x-axis move handle
                return true;
            }
        }

        // Y-axis move handle (rectangle for protruding line)
        {
            Vector2D yStart = gizmoData.position + Vector2D(0.0f, halfTransformSize.y); // Start at the edge of the object
            Vector2D yEnd = gizmoData.position + Vector2D(0.0f, gizmoData.size);        // End of the handle

            // Define rectangle bounds
            float halfThickness = THICKNESS / 2.0f;

            float minX = yStart.x - halfThickness;
            float maxX = yStart.x + halfThickness;
            float minY = yStart.y;
            float maxY = yEnd.y;

            // Check if mouse is within rectangle
            if (mouseWorldPosition.x >= minX && mouseWorldPosition.x <= maxX &&
                mouseWorldPosition.y >= minY && mouseWorldPosition.y <= maxY) {
                outHandleIndex = 1; // y-axis move handle
                return true;
            }
        }
    }
    else if (currentGizmo == CurrentGizmo::SCALE) {
        // X-axis scaling handle (rectangle for protruding line)
        {
            Vector2D xStart = gizmoData.position + Vector2D(halfTransformSize.x, 0.0f); // Start at the edge of the object
            Vector2D xEnd = gizmoData.position + Vector2D(gizmoData.size + 0.5f, 0.0f);       // End of the handle

            float halfThickness = THICKNESS / 2.0f;

            float minX = xStart.x;
            float maxX = xEnd.x;
            float minY = xStart.y - halfThickness;
            float maxY = xStart.y + halfThickness;

            // Check if mouse is within rectangle
            if (mouseWorldPosition.x >= minX && mouseWorldPosition.x <= maxX &&
                mouseWorldPosition.y >= minY && mouseWorldPosition.y <= maxY) {
                outHandleIndex = 2; // x-axis scale handle
                return true;
            }
        }

        // Y-axis scaling handle (rectangle for protruding line)
        {
            Vector2D yStart = gizmoData.position + Vector2D(0.0f, halfTransformSize.y); // Start at the edge of the object
            Vector2D yEnd = gizmoData.position + Vector2D(0.0f, gizmoData.size + 0.5f);        // End of the handle

            float halfThickness = THICKNESS / 2.0f;

            float minX = yStart.x - halfThickness;
            float maxX = yStart.x + halfThickness;
            float minY = yStart.y;
            float maxY = yEnd.y;

            // Check if mouse is within rectangle
            if (mouseWorldPosition.x >= minX && mouseWorldPosition.x <= maxX &&
                mouseWorldPosition.y >= minY && mouseWorldPosition.y <= maxY) {
                outHandleIndex = 3; // y-axis scale handle
                return true;
            }
        }
    }
    else if (currentGizmo == CurrentGizmo::ROTATE) {
        // Rotation handle (circle)
        float rotationRadius = gizmoData.size * 1.2f;
        float rotationHandleThickness = 0.8f;

        // Calculate distance manually
        float dx = mouseWorldPosition.x - gizmoData.position.x;
        float dy = mouseWorldPosition.y - gizmoData.position.y;
        float distanceToCenter = sqrt(dx * dx + dy * dy);

        if (fabs(distanceToCenter - rotationRadius) <= rotationHandleThickness) {
            outHandleIndex = 4; // rotation handle
            return true;
        }
    }

    return false;
}