#include "GameManager.h"
#include "LoggerManager.h"
#include "AssetManager.h"

static GameManager gManager;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    gManager.DuckEngine.Initialize();
    DUCKLOG_INFO("Engine initialized.");

    gManager.InitScenes();
    gManager.SetActiveScene("SpriteMovementScene");

    // load all assets before game loop starts
    AssetManager::LoadAll();

    try {
        DUCKLOG_INFO("Game Started.");
    while (gManager.DuckEngine.Running())
    {

        gManager.DuckEngine.Update();
        
        gManager.DuckEngine.StartDraw();

        gManager.DuckEngine.Draw();

        gManager.DuckEngine.EndDraw();

        //THROW_EXCEPTION("Test Error");
    }
    }
    catch (const DetailedException& ex) {
        DUCKLOG_CRASH(ex);
    }
    std::cout << "Exited!\n";

    gManager.DuckEngine.Exit();

    //cleanup();

    return 0;
}