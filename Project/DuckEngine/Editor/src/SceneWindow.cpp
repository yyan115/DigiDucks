#include "GraphicsManager.h"
#include "DuckEngine.h"
#include "SceneWindow.h"
#include "imgui.h"
#include "GameManager.h"

bool isPlaying = false;

SceneWindow::SceneWindow(DuckEngine& engine) : engine(engine) {}

void SceneWindow::RenderSceneWindow(int width, int height) 
{
    ImGui::Begin("Scene Window");

    bool gamePlaying = engine.IsPlaying();

    if (ImGui::Button(gamePlaying ? "Stop" : "Play"))
    {
        // Toggle play state
        gamePlaying = !gamePlaying;

        engine.SetPlaying(gamePlaying);

        if (gamePlaying)
        {
        }
        else 
        {
            GameManager::SetActiveScene("SpriteMovementScene");
        }
    }

    // Fetch FBO texture
    GLuint fboTexture = GraphicsManager::GetFBOTexture();
    if (fboTexture == 0)
    {
        GraphicsManager::InitializeFBO(width, height);
    }

    // Display the texture in the ImGui window
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(intptr_t)fboTexture, windowSize, ImVec2(0, 1), ImVec2(1, 0));


    ImGui::End();
}