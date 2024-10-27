#include "EditorInputManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool EditorInputManager::isDragging = false;
float cameraSensitivity = 0.07f;

void EditorInputManager::Initialize()
{
    isDragging = false;
}

void EditorInputManager::Update()
{
    double scrollOffsetY = InputManager::GetScrollOffsetY();
    if (scrollOffsetY != 0)
    {
        HandleScrollInput(scrollOffsetY);
    }

    if (InputManager::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE))
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
