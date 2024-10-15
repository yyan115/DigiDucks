#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "LoggerManager.h"
#include "SceneWindow.h"
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "UIManager.h"

static DuckEngine engine;
static SceneWindow sceneWindow;


int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    engine.Initialize(true);

    DuckEngine::SetCameraHeight(20);
    DuckEngine::DUCKENGINE_LevelManager.LoadLevel("../Resources/SpriteMovementScene.json");

    DUCKLOG_INFO("Engine initialized.");

    try {
        DUCKLOG_INFO("Game Started.");
        while (engine.Running())
        {
            engine.Update();

            engine.StartDraw();
            engine.StartImguiRender();

            engine.Draw();

            sceneWindow.RenderSceneWindow(900, 600);

            engine.EndImguiRender();
            engine.EndDraw();
        }
    }

    catch (const DetailedException& ex) {
        DUCKLOG_CRASH(ex);
    }
    std::cout << "Exited!\n";

    engine.Exit();

    return 0;
}