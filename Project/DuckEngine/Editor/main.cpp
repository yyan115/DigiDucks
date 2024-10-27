#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "LoggerManager.h"
#include "SceneWindow.h"
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "UIManager.h"
#include "GameManager.h"
#include "WindowManager.h"

static DuckEngine engine;
static UIManager uiManager;
static SceneWindow sceneWindow(engine, 1600, 900);



int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    engine.Initialize(true);

    glfwMakeContextCurrent(WindowManager::getWindow());

    DuckEngine::SetCameraHeight(20);
    GameManager::InitScenes();
    GameManager::SetActiveScene("SpriteMovementScene");
    sceneWindow.Initialize();
    uiManager.Initialize();

    DUCKLOG_INFO("Engine initialized.");

    try {
        DUCKLOG_INFO("Game Started.");
        while (engine.Running())
        {
            engine.Update();

            engine.StartDraw();

            engine.Draw();
            uiManager.StartRender();

            uiManager.Render();
            sceneWindow.RenderSceneWindow(WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight());

            uiManager.EndRender();


            engine.EndDraw();
        }
    }

    catch (const DetailedException& ex) {
        DUCKLOG_CRASH(ex);
    }
    std::cout << "Exited!\n";

    uiManager.Exit();
    engine.Exit();

    return 0;
}