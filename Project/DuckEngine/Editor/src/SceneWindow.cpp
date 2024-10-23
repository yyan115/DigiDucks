#include "SceneWindow.h"
#include "GraphicsManager.h"
#include "imgui.h"

bool isPlaying = false;

void SceneWindow::RenderSceneWindow(int width, int height) 
{
    ImGui::Begin("Scene Window");

    if (ImGui::Button(isPlaying ? "Stop" : "Play"))
    {
        // Toggle play state
        isPlaying = !isPlaying;

        if (isPlaying) 
        {
            //StartGame();  // Start the game
        }
        else 
        {
            //StopGame();   // Stop the game
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