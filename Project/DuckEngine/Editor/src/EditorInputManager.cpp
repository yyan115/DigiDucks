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
#include "SnapshotManager.h"
#include "ComponentFactory.h"

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
        SnapshotManager::SaveUndoState();

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

    if (InputManager::IsKeyDown(DuckEngine_Input::KEY_CTRL) && InputManager::IsKeyPressed('Z'))
    {
        SnapshotManager::Undo();
    }

    if (InputManager::IsKeyDown(DuckEngine_Input::KEY_CTRL) && InputManager::IsKeyPressed('Y'))
    {
        SnapshotManager::Redo();
    }

	if (UIManager::selectedEntityID != -1
		&& InputManager::IsKeyDown(DuckEngine_Input::KEY_CTRL)
		&& InputManager::IsKeyPressed('D'))
	{
        DuplicateEntityHierarchy(UIManager::selectedEntityID);
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

std::shared_ptr<Entity> EditorInputManager::DuplicateEntityHierarchy(int oldEntityID)
{
	// 1) Get the old entity
	std::shared_ptr<Entity> oldEntity =
		DuckEngine::DUCKENGINE_EntityManager.GetEntity(oldEntityID);
	if (!oldEntity)
	{
		return nullptr;
	}

	std::shared_ptr<Entity> newEntity =
		DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
	newEntity->name = oldEntity->name + "_copy";
	newEntity->layerName = oldEntity->layerName;
	newEntity->prefabName = oldEntity->prefabName;

	nlohmann::json tempJson;
	ComponentFactory::SaveComponentsToJson(oldEntityID, tempJson["components"]);
	ComponentFactory::AddComponentsToEntity(newEntity.get(), tempJson["components"]);

	TransformComponent* oldTransform =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(oldEntityID);
	TransformComponent* newTransform =
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(newEntity->entityID);

	if (oldTransform && newTransform)
	{
		newTransform->SetPosition(oldTransform->GetPosition());
	}

	for (auto& oldChild : oldEntity->childEntities)
	{
		if (!oldChild)
		{
			continue;
		}
		std::shared_ptr<Entity> newChild = DuplicateEntityHierarchy(oldChild->entityID);
		if (newChild)
		{
			newEntity->childEntities.push_back(newChild);
			newEntity->childNames.push_back(newChild->name);
		}
	}

	return newEntity;
}


std::shared_ptr<Entity> EditorInputManager::CloneSingleEntity(const Entity& oldEntity)
{
	nlohmann::json tempJson;
	ComponentFactory::SaveComponentsToJson(oldEntity.entityID, tempJson["components"]);

	std::shared_ptr<Entity> newEntity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

	newEntity->name = oldEntity.name + "_copy";
	newEntity->layerName = oldEntity.layerName;
	newEntity->prefabName = oldEntity.prefabName;

	ComponentFactory::AddComponentsToEntity(newEntity.get(), tempJson["components"]);

	if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(newEntity->entityID))
	{
		transform->localPosition.x += 50.f;
		transform->localPosition.y += 50.f;

		transform->worldPosition = transform->localPosition;

	}

	return newEntity;
}