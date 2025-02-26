#include "GameManager.h"
#include "AssetManager.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ProjectSettings.h"

static GameManager gManager;

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
#ifdef _DEBUG
    EnableConsole();
#endif

    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    gManager.DuckEngine.Initialize();

    gManager.InitScenes();
    gManager.SetActiveScene(ProjectSettings::GetStartLevel());
    gManager.DuckEngine.SetupSystems();

    // load all assets before game loop starts
    AssetManager::LoadAll();

    DuckEngine::ToggleFullScreen();

    while (gManager.DuckEngine.Running())
    {
        if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_ALT) && DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER)
            || DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_RIGHT_ALT) && DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER)
            ) {
            DuckEngine::ToggleFullScreen();
        }

        gManager.Update();
        gManager.DuckEngine.Update();
#ifdef _DEBUG
        DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));
#else
        DuckEngine::SetWindowTitle("Quack Kitchen");
#endif
        
        gManager.DuckEngine.StartDraw();

        gManager.DuckEngine.Draw();

        gManager.DuckEngine.EndDraw();

        //THROW_EXCEPTION("Test Error");
    }

    gManager.DuckEngine.Exit();

    return 0;
}