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

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        Vector2D worldPos = ConvertScreenToWorld();
        std::cout << "Mouse click in world coordinates: (" << worldPos.x << ", " << worldPos.y << ")\n";
    }


    ImGui::End();
}

Vector2D SceneWindow::ConvertScreenToWorld()
{
    // Get mouse position
    ImVec2 mousePos = ImGui::GetMousePos();

    // Get FBO position and size
    ImVec2 fboPos = ImGui::GetItemRectMin();
    ImVec2 fboSize = ImGui::GetItemRectSize();

    // Calculate relative mouse position
    float relativeX = mousePos.x - fboPos.x;
    float relativeY = mousePos.y - fboPos.y;

    // Ensure mouse is within the FBO content area
    if (relativeX < 0 || relativeY < 0 ||
        relativeX > fboSize.x || relativeY > fboSize.y)
    {
        std::cout << "Mouse click is outside the FBO content area!" << std::endl;
        return Vector2D(0, 0);  // Handle as needed
    }

    // Flip Y-axis to align with OpenGL's origin
    float flippedY = fboSize.y - relativeY;

    // Normalize coordinates to NDC space [-1, 1]
    float normalizedX = (relativeX / fboSize.x) * 2.0f - 1.0f;  // [-1, 1]
    float normalizedY = (flippedY / fboSize.y) * 2.0f - 1.0f;   // [-1, 1]

    // Map NDC to world coordinates
    Vector2D cameraPos = CameraManager::GetPosition();
    float cameraHeight = static_cast<float>(CameraManager::GetHeight());
    float aspectRatio = CameraManager::GetAR();
    float cameraWidth = cameraHeight * aspectRatio;

    float worldX = normalizedX * (cameraWidth / 2.0f) + cameraPos.x;
    float worldY = normalizedY * (cameraHeight / 2.0f) + cameraPos.y;

    return Vector2D(worldX, worldY);
}

