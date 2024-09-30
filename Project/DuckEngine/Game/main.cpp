#include "DuckEngine.h"
#include "MaxLoadScene.h"
#include "SpriteMovementScene.h"
#include "LoggerManager.h"
#include "AssetManager.h"


static DuckEngine engine;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    engine.Initialize();
    DUCKLOG_INFO("Engine initialized.");

    engine.DUCKENGINE_SceneManager.AddScene("MaxLoadScene", std::make_shared<MaxLoadScene>());
    engine.DUCKENGINE_SceneManager.AddScene("SpriteMovementScene", std::make_shared<SpriteMovementScene>());

    engine.DUCKENGINE_SceneManager.SetActiveScene("SpriteMovementScene");

    // load all assets before game loop starts
    AssetManager::LoadAll();

    try {
        DUCKLOG_INFO("Game Started.");
    while (engine.Running())
    {

        engine.Update();
        
        engine.StartDraw();

        engine.Draw();

        engine.EndDraw();

        //THROW_EXCEPTION("Test Error");
    }
    }
    catch (const DetailedException& ex) {
        DUCKLOG_CRASH(ex);
    }
    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}