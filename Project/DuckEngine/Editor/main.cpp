#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "LoggerManager.h"
#include "SceneWindow.h"
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "UIManager.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "EditorInputManager.h"

static DuckEngine engine;
static UIManager uiManager;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    engine.Initialize(true);

    glfwMakeContextCurrent(WindowManager::getWindow());

    DuckEngine::SetCameraHeight(20);
    GameManager::InitScenes();
    GameManager::SetActiveScene("SpriteMovementScene");
    SceneWindow::Initialize();
    uiManager.Initialize();
    EditorInputManager::Initialize();

    DUCKLOG_INFO("Engine initialized.");

    try {
        DUCKLOG_INFO("Game Started.");
        while (engine.Running())
        {
            EditorInputManager::Update();

            engine.Update();

            engine.StartDraw();

            engine.Draw();
            uiManager.StartRender();

            uiManager.Render();
            SceneWindow::RenderSceneWindow(WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight());

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

