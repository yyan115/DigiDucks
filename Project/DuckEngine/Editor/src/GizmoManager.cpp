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
CurrentGizmo GizmoManager::currentGizmo = CurrentGizmo::TRANSLATE;

static bool alreadyClicked = false;

void GizmoManager::Initialize() {
    // Initialization code if needed
}

void GizmoManager::Update() {

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

    selectedEntityID = UIManager::selectedEntityID;

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
    if (!transform) {
        GraphicsManager::entityIsSelected = false;
    }
    else {
        GraphicsManager::gizmoData = { transform->GetPosition(), 3.0f }; // Adjust size as needed
        GraphicsManager::entityIsSelected = true;

        HandleGizmoInteraction();
    }
}

void GizmoManager::SetSelectedEntity(int entityID) {
    selectedEntityID = entityID;
}

void GizmoManager::HandleGizmoInteraction() {
    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);

    // Get mouse position in screen space
    double mouseX = InputManager::GetMouseX();
    double mouseY = InputManager::GetMouseY();

    // Convert to world space
    Vector2D mouseWorldPosition = GraphicsManager::ScreenToWorld(Vector2D(static_cast<float>(mouseX), static_cast<float>(mouseY)));

    if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
        // Check if mouse is over the currently active gizmo handle
        if (IsMouseOverGizmoHandle(mouseWorldPosition, GraphicsManager::gizmoData, activeGizmoHandle)) {
            isDraggingGizmo = true;
            initialMouseWorldPosition = mouseWorldPosition;
            initialObjectPosition = transform->GetPosition();
            initialScale = transform->scale;
            initialRotation = transform->angle;

            std::cout << "Mouse over gizmo handle. Active handle: " << activeGizmoHandle << "\n";
        }
    }

    if (DuckEngine_Input::IsMouseButtonReleased(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
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
            float angleDelta = glm::degrees(currentAngle - startAngle);

            transform->angle = initialRotation + angleDelta;
            //std::cout << "angle: " << transform->angle << "\n";
            break;
        }
        default:
            break;
        }
    }
}

bool GizmoManager::IsMouseOverGizmoHandle(const Vector2D& mouseWorldPosition, const GizmoData& gizmoData, int& outHandleIndex) {
    float handleThickness = 50.f; // Adjust as needed

    if (currentGizmo == CurrentGizmo::TRANSLATE) {
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
    }
    else if (currentGizmo == CurrentGizmo::SCALE) {
        float scaleHandleSize = 50.f * gizmoData.size;

        // X-axis scaling handle visualization
        Vector2D scaleXHandlePosTEST = gizmoData.position + Vector2D(gizmoData.size, 0.0f);
        Vector2D scaleXHandleMin = scaleXHandlePosTEST - Vector2D(scaleHandleSize / 2.0f, scaleHandleSize / 2.0f);
        Vector2D scaleXHandleMax = scaleXHandlePosTEST + Vector2D(scaleHandleSize / 2.0f, scaleHandleSize / 2.0f);

        DuckEngine::DrawRectangle({ 0, 0 }, {5000, 5000 }, 0.f, Color(255.f, 0.f, 0.f, 255.f), true);
        std::cout << "rect drawn\n";

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
    }
    else if (currentGizmo == CurrentGizmo::ROTATE) {
        // Rotation handle (circle)
        float rotationRadius = gizmoData.size * 1.2f;
        float rotationHandleThickness = 50.f;

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
    t = static_cast<float>(fmax(0, fmin(1, t)));

    float closestX = lineStart.x + t * dx;
    float closestY = lineStart.y + t * dy;

    dx = point.x - closestX;
    dy = point.y - closestY;

    return sqrt(dx * dx + dy * dy);
}

bool GizmoManager::IsPointInSquare(const Vector2D& point, const Vector2D& center, float size) {
    return fabs(point.x - center.x) <= size / 2.0f && fabs(point.y - center.y) <= size / 2.0f;
}
