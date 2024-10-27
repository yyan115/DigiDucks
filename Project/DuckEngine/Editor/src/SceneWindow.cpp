#include "GraphicsManager.h"
#include "WindowManager.h"
#include "DuckEngine.h"
#include "SceneWindow.h"
#include "imgui.h"
#include "GameManager.h"

bool isPlaying = false;

SceneWindow::SceneWindow(DuckEngine& engine, int width, int height)
    : engine(engine), width(width), height(height) 
{
    GraphicsManager::InitializeFBO(width, height);
}

void SceneWindow::Initialize()
{
    int initialWidth = WindowManager::GetWindowWidth();
    int initialHeight = WindowManager::GetWindowHeight();
    GraphicsManager::InitializeFBO(initialWidth, initialHeight);
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

    bool gamePlaying = engine.IsPlaying();

    if (ImGui::Button(gamePlaying ? "Stop" : "Play"))
    {
        gamePlaying = !gamePlaying;
        engine.SetPlaying(gamePlaying);

        if (!gamePlaying)
        {
            GameManager::SetActiveScene("SpriteMovementScene");
        }
    }

    GLuint fboTexture = GraphicsManager::GetFBOTexture();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(intptr_t)fboTexture, windowSize, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}
