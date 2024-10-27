#include "GraphicsManager.h"
#include "WindowManager.h"
#include "DuckEngine.h"
#include "SceneWindow.h"
#include "imgui.h"
#include "GameManager.h"
#include "CameraManager.h"

bool isPlaying = false;
int SceneWindow::width = 0;
int SceneWindow::height = 0;
bool SceneWindow::inSceneFBO = false;

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

    bool gamePlaying = DuckEngine::IsPlaying();

    if (ImGui::Button(gamePlaying ? "Stop" : "Play"))
    {
        gamePlaying = !gamePlaying;
        DuckEngine::SetPlaying(gamePlaying);

        if (!gamePlaying)
        {
            GameManager::SetActiveScene("SpriteMovementScene");
        }
    }

    GLuint fboTexture = GraphicsManager::GetFBOTexture();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(intptr_t)fboTexture, windowSize, ImVec2(0, 1), ImVec2(1, 0));

    // Check if the mouse is within the FBO content area
    inSceneFBO = IsMouseInFBO();

    if (inSceneFBO && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Vector2D worldPos = ConvertScreenToWorld();
        std::cout << "Mouse click in world coordinates: (" << worldPos.x << ", " << worldPos.y << ")\n";
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