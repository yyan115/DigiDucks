#include "GameManager.h"
#include "AssetManager.h"
#include "DuckEngine_Input.h"

static GameManager gManager;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    gManager.DuckEngine.Initialize();

    gManager.InitScenes();
    gManager.SetActiveScene("MainMenu");
    gManager.DuckEngine.SetupSystems();

    // load all assets before game loop starts
    AssetManager::LoadAll();


    while (gManager.DuckEngine.Running())
    {
        if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_ALT) && DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER)
            || DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_RIGHT_ALT) && DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER)
            ) {
            DuckEngine::ToggleFullScreen();
        }

        gManager.Update();
        gManager.DuckEngine.Update();
        
        gManager.DuckEngine.StartDraw();

        gManager.DuckEngine.Draw();

        gManager.DuckEngine.EndDraw();

        //THROW_EXCEPTION("Test Error");
    }

    gManager.DuckEngine.Exit();

    return 0;
}