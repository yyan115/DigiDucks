#include "DuckEngine.h"
#include "UIManager.h"

static DuckEngine engine;



int main(void)
{
    //engine.Start();
    std::cout << "testing\n";
    engine.Initialize();  
    UIManager uiManager;
    uiManager.Initialize();

    while (engine.Running()) 
    {
        std::cout << "testing\n";
        uiManager.Render();
    }

    std::cout << "Exited!\n";
    uiManager.Exit();

    //cleanup();

    return 0;
}