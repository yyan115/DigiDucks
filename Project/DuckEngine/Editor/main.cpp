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
#include "GizmoManager.h"

static DuckEngine engine;
static UIManager uiManager;

void EnableConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
#ifdef EDITOR_DEBUG
    EnableConsole();
    DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));
#endif

    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

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
    uiManager.Initialize();
    SceneWindow::Initialize();
    EditorInputManager::Initialize();
    
    engine.SetupSystems();

    DUCKLOG_INFO("Engine initialized.");

    try {
        DUCKLOG_INFO("Game Started.");
        while (engine.Running())
        {
            TimeManager::StartTotalTimer();  
            
            EditorInputManager::Update();
            
            if (!engine.isPaused)
            {
                engine.Update();
                GameManager::Update();
            }

//#ifdef EDITOR_DEBUG
            DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));
//#else
            //DuckEngine::SetWindowTitle("Quack Kitchen");
//#endif

            GizmoManager::Update();

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

