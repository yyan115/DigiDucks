#include "DuckEngine.h"
#include "MaxLoadScene.h"

static DuckEngine engine;

Scene* activeScene;

int main(void)
{
    engine.Initialize();

    // Set Active Scene
    MaxLoadScene maxLoadScene;
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