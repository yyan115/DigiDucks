#include "DuckEngine.h"
#include "MaxLoadScene.h"
#include "SpriteMovementScene.h"

static DuckEngine engine;

Scene* activeScene;

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    engine.Initialize();


    // Possible Scenes
    MaxLoadScene maxLoadScene;
    SpriteMovementScene spriteMovementScene;
    
    // Set Active Scene
    activeScene = &maxLoadScene;
    
    // Load and Start Active Scene
    activeScene->Load();
    activeScene->Start();


    while (engine.Running())
    {
        // Update Active Scene
        activeScene->Update();

        engine.Update();
        engine.StartDraw();

        engine.Draw();

        engine.EndDraw();

    }

    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}