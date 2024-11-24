#include "WindowManager.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "LoggerManager.h"
#include "SceneWindow.h"
#include <imgui_impl_opengl3.h>
#include "UIManager.h"
#include "GameManager.h"
#include "EditorInputManager.h"
#include "TimeManager.h"

static DuckEngine engine;
static UIManager uiManager;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    engine.Initialize(true);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    GLFWwindow* window = WindowManager::getWindow();
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    DuckEngine::SetCameraHeight(20);
    GameManager::InitScenes();
    GameManager::SetActiveScene("GameScene");
    SceneWindow::Initialize();
    uiManager.Initialize();
    EditorInputManager::Initialize();
    
    engine.SetupSystems();

    DUCKLOG_INFO("Engine initialized.");

    try {
        DUCKLOG_INFO("Game Started.");
        while (engine.Running())
        {
            TimeManager::StartTotalTimer();  
            
            EditorInputManager::Update();

            engine.Update();
            
            engine.StartDraw();
            
            engine.Draw();
            
            uiManager.StartRender();
            
            uiManager.Render();

            SceneWindow::RenderSceneWindow(WindowManager::GetWindowWidth(), WindowManager::GetWindowHeight());
            
            uiManager.EndRender();
            
            TimeManager::EndTotalTimer();
            engine.EndDraw();                          
        }
    }

    catch (const DetailedException& ex) {
        DUCKLOG_CRASH(ex);
    }
    std::cout << "Exited!\n";

    uiManager.Exit();
    engine.Exit();
    glfwTerminate();
    return 0;
}

