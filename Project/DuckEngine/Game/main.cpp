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
    LoggerManager& logger = LoggerManager::GetInstance();
    logger.LogInfo("Game initialized.");

    // Possible Scenes
    MaxLoadScene maxLoadScene;
    SpriteMovementScene spriteMovementScene;
    
    // Set Active Scene
    activeScene = &spriteMovementScene;
    
    // Load and Start Active Scene
    activeScene->Load();
    activeScene->Start();

    try {
        logger.LogInfo("Game started.");
    while (engine.Running())
    {
        // Update Active Scene
        activeScene->Update();

        engine.Update();
        engine.StartDraw();

        engine.Draw();

        engine.EndDraw();

    }
    }
    catch (const std::exception& ex) {
        //logger.LogException(ex);
    }
    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}