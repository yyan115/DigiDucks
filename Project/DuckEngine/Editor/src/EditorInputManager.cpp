#include <iostream>
#include "EditorInputManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "WindowManager.h"
#include "imgui.h"
#include "SceneWindow.h"
#include "UIManager.h"
#include "DuckEngine_Input.h"

float cameraSensitivity = 0.05f;
bool EditorInputManager::isDragging = false;

void EditorInputManager::Initialize()
{
    isDragging = false;
}

void EditorInputManager::Update()
{
    if (SceneWindow::GetIsPlaying()) return;

    double scrollOffsetY = InputManager::GetScrollOffsetY();
    if (scrollOffsetY != 0 && SceneWindow::GetInSceneFBO())
    {
        HandleScrollInput(scrollOffsetY);
    }
    else
    {
        scrollOffsetY = 0;
    }

    if (InputManager::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) && SceneWindow::GetInSceneFBO())
    {
        if (!isDragging)
        {
            isDragging = true;
            InputManager::ResetLastMousePosition();
        }

        double deltaX = InputManager::GetMouseDeltaX();
        double deltaY = InputManager::GetMouseDeltaY();

        if (deltaX != 0.0 || deltaY != 0.0)
        {
            HandleMouseDrag(deltaX, -deltaY);
            InputManager::ResetLastMousePosition();
        }
    }
    else
    {
        isDragging = false;
    }

    // Use InputManager to check for Delete key press
    if (UIManager::selectedEntityID != -1 && InputManager::IsKeyPressed(DuckEngine_Input::KEY_DEL))
    {
        // Remove the entity from the scene
        DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(UIManager::selectedEntityID);

        // Clear selection
        UIManager::selectedEntityID = -1;
    }
}

void EditorInputManager::HandleScrollInput(double offsetY)
{
    CameraManager::AdjustHeight(static_cast<int>(-offsetY));
}

void EditorInputManager::HandleMouseDrag(double deltaX, double deltaY)
{
    int windowWidth = WindowManager::GetWindowWidth();
    int windowHeight = WindowManager::GetWindowHeight();

    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    float adjustedDeltaX = static_cast<float>(deltaX) * cameraSensitivity / aspectRatio;
    float adjustedDeltaY = static_cast<float>(deltaY) * cameraSensitivity;

    CameraManager::MoveCamera(adjustedDeltaX, adjustedDeltaY);
}
