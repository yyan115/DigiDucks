/******************************************************************************
\file       EditorInputManager.h
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Declaration of the EditorInputManager class, which manages input
            events for the editor, such as camera manipulation, scroll handling,
            and entity selection.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include <iostream>
#include "EditorInputManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "WindowManager.h"
#include "imgui.h"
#include "SceneWindow.h"
#include "UIManager.h"
#include "DuckEngine_Input.h"
#include "HierarchyList.h"

float cameraSensitivity = 0.05f;
bool EditorInputManager::isDragging = false;

/**************************************************************************
@brief Initializes the editor input manager, setting dragging to false by default.
**************************************************************************/
void EditorInputManager::Initialize()
{
    isDragging = false;
}

/**************************************************************************
@brief Handles frame-based updates for editor input, including scroll handling,
       middle-mouse dragging for camera movement, and delete key functionality.
**************************************************************************/
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

    // Use InputManager to check for Delete key press
    if (UIManager::selectedEntityID != -1 && InputManager::IsKeyPressed(GLFW_KEY_F2))
    {
        Hierarchy::StartRenamingEntity(UIManager::selectedEntityID);
    }
}

/**************************************************************************
@brief Processes scroll input to adjust the camera’s height, effectively zooming
       in or out in the scene view.
@param offsetY The scroll offset in the Y direction.
**************************************************************************/
void EditorInputManager::HandleScrollInput(double offsetY)
{
    CameraManager::AdjustHeight(static_cast<int>(-offsetY));
}

/**************************************************************************
@brief Handles mouse dragging for panning the camera. Adjusts camera movement
       based on the mouse deltas and aspect ratio.
@param deltaX The horizontal change in the mouse position.
@param deltaY The vertical change in the mouse position.
**************************************************************************/
void EditorInputManager::HandleMouseDrag(double deltaX, double deltaY)
{
    int windowWidth = WindowManager::GetWindowWidth();
    int windowHeight = WindowManager::GetWindowHeight();

    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    float adjustedDeltaX = static_cast<float>(deltaX) * cameraSensitivity / aspectRatio;
    float adjustedDeltaY = static_cast<float>(deltaY) * cameraSensitivity;

    CameraManager::MoveCamera(adjustedDeltaX, adjustedDeltaY);
}
