#include "SceneWindow.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "DuckEngine.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "EditorInputManager.h"
#include "imgui.h"
#include <iostream>

bool SceneWindow::isPlaying = false;
int SceneWindow::width = 0;
int SceneWindow::height = 0;
bool SceneWindow::inSceneFBO = false;
Entity* SceneWindow::selectedEntity = nullptr;
Vector2D SceneWindow::initialMousePos;
Vector2D SceneWindow::initialEntityPos;
std::vector<Entity*> SceneWindow::entitiesUnderMouse;
int SceneWindow::currentEntityIndex = -1;
Vector2D SceneWindow::lastMousePos;

void SceneWindow::Initialize()
{
    width = WindowManager::GetWindowWidth();
    height = WindowManager::GetWindowHeight();
    GraphicsManager::InitializeFBO(width, height);
}

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

    if (ImGui::Button(isPlaying ? "Stop" : "Play"))
    {
        isPlaying = !isPlaying;
        DuckEngine::SetPlaying(isPlaying);

        if (!isPlaying)
        {
            GameManager::SetActiveScene("SpriteMovementScene");
        }
    }


    GLuint fboTexture = GraphicsManager::GetFBOTexture();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(intptr_t)fboTexture, windowSize, ImVec2(0, 1), ImVec2(1, 0));

    inSceneFBO = IsMouseInFBO();
    Vector2D worldPos = ConvertScreenToWorld();

    // Handle drag-and-drop from AssetsBrowser
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PREFAB_PAYLOAD"))
        {
            const char* prefabName = static_cast<const char*>(payload->Data);
            if (prefabName)
            {
                // Instantiate the prefab at the current mouse position
                PrefabManager::InstantiatePrefab(prefabName, worldPos);
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
            selectedEntity = entitiesUnderMouse[0];
            currentEntityIndex = 0;

            EditorInputManager::SetIsDragging(true);
            initialMousePos = worldPos;

            auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);
            if (transform)
            {
                initialEntityPos = transform->position;
            }
        }
    }

    if (inSceneFBO && !isPlaying && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        if (!entitiesUnderMouse.empty())
        {
            currentEntityIndex = (currentEntityIndex + 1) % entitiesUnderMouse.size();
            selectedEntity = entitiesUnderMouse[currentEntityIndex];

            EditorInputManager::SetIsDragging(true);
            initialMousePos = worldPos;

            auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);
            if (transform)
            {
                initialEntityPos = transform->position;
            }
        }
    }

    if (selectedEntity && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        HandleEntityDragging();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        selectedEntity = nullptr;
        EditorInputManager::SetIsDragging(false);
    }

    ImGui::End();
}


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

std::vector<Entity*> SceneWindow::GetEntitiesAtPosition(const Vector2D& worldPos)
{
    std::vector<Entity*> foundEntities;

    auto& transformComponents = DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TransformComponent>();

    for (auto& [entityID, component] : transformComponents)
    {
        auto* transform = std::static_pointer_cast<TransformComponent>(component).get();

        float left = transform->position.x - transform->scale.x;
        float right = transform->position.x + transform->scale.x;
        float top = transform->position.y - transform->scale.y;
        float bottom = transform->position.y + transform->scale.y;

        if (worldPos.x >= left && worldPos.x <= right &&
            worldPos.y >= top && worldPos.y <= bottom)
        {
            foundEntities.push_back(DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID));
        }
    }

    return foundEntities;
}
