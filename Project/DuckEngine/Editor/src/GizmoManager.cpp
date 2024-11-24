/******************************************************************************/
/*!
\file       GizmoManager.cpp
\author     Your Name
\brief      Implements the GizmoManager class responsible for handling gizmo interaction,
            including drawing gizmos and processing user input for moving, scaling,
            and rotating game objects.

*/
/******************************************************************************/

#include "GizmoManager.h"
#include "CameraManager.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "DuckEngine.h"
#include "UIManager.h" // Include to access selectedEntityID
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// Initialize static member variables
int GizmoManager::selectedEntityID = -1;
bool GizmoManager::isDraggingGizmo = false;
int GizmoManager::activeGizmoHandle = -1;
Vector2D GizmoManager::initialMouseWorldPosition;
Vector2D GizmoManager::initialObjectPosition;
Vector2D GizmoManager::initialScale;
float GizmoManager::initialRotation = 0.0f;

void GizmoManager::Initialize() {
    // Initialization code if needed
}

void GizmoManager::Update() {
    selectedEntityID = UIManager::selectedEntityID;

    if (selectedEntityID == -1)
        return;

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
    if (!transform)
        return;

    HandleGizmoInteraction();
}

void GizmoManager::Render() {
    selectedEntityID = UIManager::selectedEntityID;

    if (selectedEntityID == -1)
        return;

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
    if (!transform)
        return;

    GraphicsManager::gizmoData = { transform->GetPosition(), 3.0f }; // Adjust size as needed
    GraphicsManager::entityIsSelected = true;
    //GraphicsManager::DrawGizmo();
}

void GizmoManager::SetSelectedEntity(int entityID) {
    selectedEntityID = entityID;
}

void GizmoManager::HandleGizmoInteraction() {
    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
    if (!transform)
        return;

    // Get mouse position in screen space
    double mouseX = InputManager::GetMouseX();
    double mouseY = InputManager::GetMouseY();

    // Convert to world space
    Vector2D mouseWorldPosition = GraphicsManager::ScreenToWorld(Vector2D(mouseX, mouseY));

    if (InputManager::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        // Check if mouse is over gizmo handle
        if (IsMouseOverGizmoHandle(mouseWorldPosition, GraphicsManager::gizmoData, activeGizmoHandle)) {
            isDraggingGizmo = true;
            initialMouseWorldPosition = mouseWorldPosition;
            initialObjectPosition = transform->GetPosition();
            initialScale = transform->scale;
            initialRotation = transform->angle;
        }
    }

    if (InputManager::IsMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
        isDraggingGizmo = false;
        activeGizmoHandle = -1;
    }

    if (isDraggingGizmo && activeGizmoHandle != -1 && InputManager::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
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
            float angleDelta = glm::degrees(currentAngle - startAngle);

            transform->angle = initialRotation + angleDelta;
            break;
        }
        default:
            break;
        }
    }
}

bool GizmoManager::IsMouseOverGizmoHandle(const Vector2D& mouseWorldPosition, const GizmoData& gizmoData, int& outHandleIndex) {
    float handleThickness = 0.2f; // Adjust as needed

    // X-axis move handle (line)
    Vector2D xStart = gizmoData.position;
    Vector2D xEnd = gizmoData.position + Vector2D(gizmoData.size, 0.0f);
    if (IsPointNearLine(mouseWorldPosition, xStart, xEnd, handleThickness)) {
        outHandleIndex = 0; // x-axis move handle
        return true;
    }

    // Y-axis move handle (line)
    Vector2D yStart = gizmoData.position;
    Vector2D yEnd = gizmoData.position + Vector2D(0.0f, gizmoData.size);
    if (IsPointNearLine(mouseWorldPosition, yStart, yEnd, handleThickness)) {
        outHandleIndex = 1; // y-axis move handle
        return true;
    }

    // Scaling handles (squares)
    float scaleHandleSize = 0.2f * gizmoData.size;

    // X-axis scaling handle
    Vector2D scaleXHandlePos = gizmoData.position + Vector2D(gizmoData.size, 0.0f);
    if (IsPointInSquare(mouseWorldPosition, scaleXHandlePos, scaleHandleSize)) {
        outHandleIndex = 2; // x-axis scale handle
        return true;
    }

    // Y-axis scaling handle
    Vector2D scaleYHandlePos = gizmoData.position + Vector2D(0.0f, gizmoData.size);
    if (IsPointInSquare(mouseWorldPosition, scaleYHandlePos, scaleHandleSize)) {
        outHandleIndex = 3; // y-axis scale handle
        return true;
    }

    // Rotation handle (circle)
    float rotationRadius = gizmoData.size * 1.2f;
    float rotationHandleThickness = 0.2f; // Adjust as needed

    // Calculate distance manually
    float dx = mouseWorldPosition.x - gizmoData.position.x;
    float dy = mouseWorldPosition.y - gizmoData.position.y;
    float distanceToCenter = sqrt(dx * dx + dy * dy);

    if (fabs(distanceToCenter - rotationRadius) <= rotationHandleThickness) {
        outHandleIndex = 4; // rotation handle
        return true;
    }

    return false;
}


bool GizmoManager::IsPointNearLine(const Vector2D& point, const Vector2D& lineStart, const Vector2D& lineEnd, float thickness) {
    float distance = DistancePointToLineSegment(point, lineStart, lineEnd);
    return distance <= thickness;
}

float GizmoManager::DistancePointToLineSegment(const Vector2D& point, const Vector2D& lineStart, const Vector2D& lineEnd) {
    float dx = lineEnd.x - lineStart.x;
    float dy = lineEnd.y - lineStart.y;

    if (dx == 0 && dy == 0) {
        // It's a point, not a line segment
        dx = point.x - lineStart.x;
        dy = point.y - lineStart.y;
        return sqrt(dx * dx + dy * dy);
    }

    float t = ((point.x - lineStart.x) * dx + (point.y - lineStart.y) * dy) / (dx * dx + dy * dy);
    t = fmax(0, fmin(1, t));

    float closestX = lineStart.x + t * dx;
    float closestY = lineStart.y + t * dy;

    dx = point.x - closestX;
    dy = point.y - closestY;

    return sqrt(dx * dx + dy * dy);
}

bool GizmoManager::IsPointInSquare(const Vector2D& point, const Vector2D& center, float size) {
    return fabs(point.x - center.x) <= size / 2.0f && fabs(point.y - center.y) <= size / 2.0f;
}
