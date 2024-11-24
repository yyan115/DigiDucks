#include "GameManager.h"
#include "AssetManager.h"

static GameManager gManager;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    gManager.DuckEngine.Initialize();

    gManager.InitScenes();
    gManager.SetActiveScene("SpriteMovementScene");
    gManager.DuckEngine.SetupSystems();

    // load all assets before game loop starts
    AssetManager::LoadAll();


    while (gManager.DuckEngine.Running())
    {
        gManager.DuckEngine.Update();
        
        gManager.DuckEngine.StartDraw();

        gManager.DuckEngine.Draw();

        gManager.DuckEngine.EndDraw();

        //THROW_EXCEPTION("Test Error");
    }

    gManager.DuckEngine.Exit();

    return 0;
}