/******************************************************************************
\file       SceneWindow.cpp
\author     Lucas Yee 2301212 (50%)
\par        l.yee@digipen.edu
\author     Muhammad Zikry Bin Zakaria , 2201751 (50%)
\par        muhammadzikry.b@digipen.edu
\date       November 6, 2024
\brief      Implementation of the SceneWindow class, which manages scene rendering,
            FBO and world coordinate transformations, and entity interaction within
            the editor viewport.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include "SceneWindow.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "DuckEngine.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "EditorInputManager.h"
#include "UIManager.h"
#include "Gizmos.h"
#include "imgui.h"
#include "SoundSystem.h"
#include <iostream>
#include "SnapshotManager.h"

bool SceneWindow::isPlaying = false;
bool SceneWindow::isPaused = false;
int SceneWindow::width = 0;
int SceneWindow::height = 0;
bool SceneWindow::inSceneFBO = false;
Entity* SceneWindow::selectedEntity = nullptr;
Vector2D SceneWindow::initialMousePos;
Vector2D SceneWindow::initialEntityPos;
std::vector<Entity*> SceneWindow::entitiesUnderMouse;
int SceneWindow::currentEntityIndex = -1;
Vector2D SceneWindow::lastMousePos;

Vector2D draggedEntityOriginalPos;

/**************************************************************************
@brief Initializes the scene window by setting up the FBO dimensions.
**************************************************************************/
void SceneWindow::Initialize()
{
    width = WindowManager::GetWindowWidth();
    height = WindowManager::GetWindowHeight();
    GraphicsManager::InitializeFBO(width, height);
}

/**************************************************************************
@brief Renders the scene window, updating the FBO and handling user interactions.
@param newWidth The new width of the scene window.
@param newHeight The new height of the scene window.
**************************************************************************/
void SceneWindow::RenderSceneWindow(int newWidth, int newHeight)
{
    if (newWidth != width || newHeight != height)
    {
        width = newWidth;
        height = newHeight;
        GraphicsManager::InitializeFBO(width, height);
    }

    ImGui::Begin("Scene Window");

    isPlaying = DuckEngine::IsPlaying();
    isPaused = DuckEngine::IsPaused();
    if (ImGui::Button(isPlaying ? "Stop" : "Play"))
    {
        isPlaying = !isPlaying;
        DuckEngine::SetPlaying(isPlaying);

        if (!isPlaying)
        {
            SoundSystem::StopAllSounds();
            GameManager::SetActiveScene(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
            DuckEngine::SetPaused(false);
            CameraManager::ResetToDefault();
        }
        else
        {
            LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
            DuckEngine::DUCKENGINE_SystemManager.StartAll();
            GameManager::SetActiveScene(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
            SoundSystem::ResumeAllSounds();
            DuckEngine::SetPaused(false);
        }
    }

    if (isPlaying)
    {
        ImGui::SameLine();
        if (ImGui::Button(isPaused ? "Continue" : "Pause"))
        {
            isPaused = !isPaused;
            DuckEngine::SetPaused(isPaused);

            if (isPaused)
            {
                SoundSystem::PauseAllSounds();  // Pause all sounds
            }
            else
            {
                SoundSystem::ResumeAllSounds();  // Resume all sounds
            }
        }
    }

    GLuint fboTexture = GraphicsManager::GetFBOTexture();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(intptr_t)fboTexture, windowSize, ImVec2(0, 1), ImVec2(1, 0));

    inSceneFBO = IsMouseInFBO();
    Vector2D worldPos = ConvertScreenToWorld();

    DuckEngine::editorContentRegion = { windowSize.x, windowSize.y };
    DuckEngine::editorMouseWorldPos = worldPos;
    DuckEngine::editorMouseScreenPos = ConvertScreenToFBO();

    // Handle drag-and-drop from AssetsBrowser
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB_PAYLOAD"))
        {
            const char* prefabName = static_cast<const char*>(payload->Data);
            if (prefabName)
            {
                OnPrefabDraggedIntoScene(prefabName, worldPos);
            }
        }
        ImGui::EndDragDropTarget();
    }
   

    if (worldPos.x != lastMousePos.x || worldPos.y != lastMousePos.y)
    {
        entitiesUnderMouse.clear();
        currentEntityIndex = -1;
        lastMousePos = worldPos;

        entitiesUnderMouse = GetEntitiesAtPosition(worldPos);
    }

    if (inSceneFBO && !isPlaying && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (!entitiesUnderMouse.empty())
        {
            SnapshotManager::SaveUndoState();
            selectedEntity = entitiesUnderMouse[0];
            currentEntityIndex = 0;

            UIManager::selectedEntityID = selectedEntity->entityID;
            UIManager::windowStates[WindowType::Inspector] = true;

            EditorInputManager::SetIsDragging(true);
            initialMousePos = worldPos;

            auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);
            if (transform)
            {
                initialEntityPos = transform->GetPosition();
                draggedEntityOriginalPos = initialEntityPos;
            }
        }
        else
        {
            selectedEntity = nullptr;
            UIManager::selectedEntityID = -1;
            UIManager::windowStates[WindowType::Inspector] = false;
        }
    }

    if (inSceneFBO && !isPlaying && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        if (!entitiesUnderMouse.empty())
        {
            currentEntityIndex = (currentEntityIndex + 1) % entitiesUnderMouse.size();
            selectedEntity = entitiesUnderMouse[currentEntityIndex];

            UIManager::selectedEntityID = selectedEntity->entityID;
            UIManager::windowStates[WindowType::Inspector] = true;

            EditorInputManager::SetIsDragging(true);
            initialMousePos = worldPos;

            auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);
            if (transform)
            {
                initialEntityPos = transform->GetPosition();
            }
        }
    }

    // If an entity is selected, render gizmos
    /*if (selectedEntity && !isPlaying) {
        Gizmos::RenderGizmoForSelectedEntity(selectedEntity);
    }*/

    if (selectedEntity && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        HandleEntityDragging();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        if (selectedEntity)
        {
            auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);
            if (transform && transform->GetPosition() == draggedEntityOriginalPos)
            {
                std::cout << "Object didn't moved! so dont save state" << std::endl;
                SnapshotManager::RemoveLatestUndoState();
            }
        }

        selectedEntity = nullptr;
        EditorInputManager::SetIsDragging(false);
    }

    if (selectedEntity) {
        //GraphicsManager::AddToDebugDrawQueue();
        //std::cout << "selected";
    }

    ImGui::End();
}

/**************************************************************************
@brief Checks if the mouse cursor is within the FBO region.
@return True if the mouse is within the FBO, false otherwise.
**************************************************************************/
bool SceneWindow::IsMouseInFBO()
{
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 fboPos = ImGui::GetItemRectMin();
    ImVec2 fboSize = ImGui::GetItemRectSize();

    return mousePos.x >= fboPos.x &&
        mousePos.x <= fboPos.x + fboSize.x &&
        mousePos.y >= fboPos.y &&
        mousePos.y <= fboPos.y + fboSize.y;
}

/**************************************************************************
@brief Converts the current mouse position on the screen to FBO coordinates.
@return The converted FBO coordinates as a Vector2D. Returns {-999.0f, -999.0f}
        if the mouse is outside the FBO area.
**************************************************************************/
Vector2D SceneWindow::ConvertScreenToFBO()
{
    if (!IsMouseInFBO())
    {
        return { -999.0f, -999.0f };
    }
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 fboPos = ImGui::GetItemRectMin();
    ImVec2 fboSize = ImGui::GetItemRectSize();

    // Calculate the relative position within the FBO
    float fboX = mousePos.x - fboPos.x;
    float fboY = mousePos.y - fboPos.y;

    fboX = std::clamp(fboX, 0.0f, fboSize.x);
    fboY = std::clamp(fboY, 0.0f, fboSize.y);

    return Vector2D(fboX, fboY);
}

/**************************************************************************
@brief Converts the mouse position on the screen to world coordinates based on
       camera settings.
@return The world coordinates as a Vector2D.
**************************************************************************/
Vector2D SceneWindow::ConvertScreenToWorld()
{
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 fboPos = ImGui::GetItemRectMin();
    ImVec2 fboSize = ImGui::GetItemRectSize();

    float relativeX = mousePos.x - fboPos.x;
    float relativeY = mousePos.y - fboPos.y;
    float flippedY = fboSize.y - relativeY;
    float normalizedX = (relativeX / fboSize.x) * 2.0f - 1.0f;
    float normalizedY = (flippedY / fboSize.y) * 2.0f - 1.0f;

    Vector2D cameraPos = CameraManager::GetPosition();
    float cameraHeight = static_cast<float>(CameraManager::GetHeight());
    float aspectRatio = CameraManager::GetAR();
    float cameraWidth = cameraHeight * aspectRatio;

    float worldX = normalizedX * (cameraWidth / 2.0f) + cameraPos.x;
    float worldY = normalizedY * (cameraHeight / 2.0f) + cameraPos.y;

    return Vector2D(worldX, worldY);
}

/**************************************************************************
@brief Converts world coordinates to screen coordinates within the FBO.
@param worldPos The world coordinates to convert.
@return The converted screen coordinates as a Vector2D.
**************************************************************************/
Vector2D SceneWindow::ConvertWorldToScreen(const Vector2D& worldPos)
{
    Vector2D cameraPos = CameraManager::GetPosition();
    float cameraHeight = static_cast<float>(CameraManager::GetHeight());
    float aspectRatio = CameraManager::GetAR();
    float cameraWidth = cameraHeight * aspectRatio;

    ImVec2 fboSize = ImGui::GetContentRegionAvail();
    float normalizedX = (worldPos.x - cameraPos.x) / (cameraWidth / 2.0f);
    float normalizedY = (worldPos.y - cameraPos.y) / (cameraHeight / 2.0f);

    float screenX = (normalizedX + 1.0f) * fboSize.x / 2.0f;
    float screenY = (1.0f - normalizedY) * fboSize.y / 2.0f;

    ImVec2 fboPos = ImGui::GetItemRectMin();
    return Vector2D(fboPos.x + screenX, fboPos.y + screenY);
}

/**************************************************************************
@brief Calculates the scale factor between the world and the FBO based on
       the current camera settings and FBO dimensions.
@return The world scale as a Vector2D.
**************************************************************************/
Vector2D SceneWindow::GetWorldScale()
{
    ImVec2 fboSize = ImGui::GetContentRegionAvail();
    Vector2D worldScale((CameraManager::GetHeight() * CameraManager::GetAR()) / fboSize.x, CameraManager::GetHeight() / fboSize.y);
    return worldScale;
}

/**************************************************************************
@brief Handles the dragging action for an entity by updating its position
       based on the initial and current mouse positions in world space.
**************************************************************************/
void SceneWindow::HandleEntityDragging()
{
    Vector2D currentMousePos = ConvertScreenToWorld();
    Vector2D delta = currentMousePos - initialMousePos;

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);

    if (transform)
    {
        transform->SetPosition(initialEntityPos + delta);
    }
}

/**************************************************************************
@brief Retrieves a list of entities at a specific position in the world,
       typically based on the mouse cursor's location.
@param worldPos The position in the world to check for entities.
@return A vector of pointers to entities at the specified world position.
**************************************************************************/
std::vector<Entity*> SceneWindow::GetEntitiesAtPosition(const Vector2D& worldPos)
{
    std::vector<Entity*> foundEntities;

    auto& transformComponents = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TransformComponent>();

    for (auto& [entityID, component] : transformComponents)
    {
        auto* transform = std::static_pointer_cast<TransformComponent>(component).get();

        float left = transform->GetPosition().x - transform->scale.x / 2;
        float right = transform->GetPosition().x + transform->scale.x / 2;
        float top = transform->GetPosition().y - transform->scale.y / 2;
        float bottom = transform->GetPosition().y + transform->scale.y / 2;

        if (worldPos.x >= left && worldPos.x <= right &&
            worldPos.y >= top && worldPos.y <= bottom)
        {
            // Skip entities with the "Background" layer
            Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
            if (entity && entity->layerName == "Background")
            {
                continue; 
            }

            foundEntities.push_back(DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID));
        }
    }

    return foundEntities;
}

/**************************************************************************
@brief Handles the event of a prefab being dragged and dropped into the scene,
       instantiating it at the specified world position.
@param prefabName The name of the prefab being dragged into the scene.
@param position The position in the world where the prefab will be instantiated.
**************************************************************************/
void SceneWindow::OnPrefabDraggedIntoScene(const std::string& prefabName, Vec2 position)
{
    DuckEngine::DUCKENGINE_PrefabManager.InstantiatePrefab(prefabName, position);
}
