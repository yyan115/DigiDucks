#include "DuckEngine.h"
#include "MaxLoadScene.h"
#include "SpriteMovementScene.h"
#include "LoggerManager.h"

static DuckEngine engine;

Scene* activeScene;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    engine.Initialize();
    DUCKLOG_INFO("Engine initialized.");

    // Possible Scenes
    MaxLoadScene maxLoadScene;
    SpriteMovementScene spriteMovementScene;
    
    // Set Active Scene
    activeScene = &spriteMovementScene;
    
    // Load and Start Active Scene
    activeScene->Load();
    activeScene->Start();

    try {
        DUCKLOG_INFO("Game Started.");
    while (engine.Running())
    {
        // Update Active Scene
        activeScene->Update();

        engine.Update();
        
        engine.StartDraw();

        engine.Draw();

        engine.EndDraw();

       //throw std::runtime_error("Test Error");
    }
    }
    catch (const std::exception& ex) {
        DUCKLOG_CRASH(ex.what());
    }
    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}