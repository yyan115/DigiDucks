#include "DuckEngine.h"


int main(void)
{
    DuckEngine engine;
    //engine.Start();

    engine.Initialize();

    while (engine.Running()) {

        std::cout << "Running!\n";

        engine.Update();

        engine.Draw();
    }

    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}