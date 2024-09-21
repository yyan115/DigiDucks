#include "DuckEngine.h"
#include "LoggerManager.h"

static DuckEngine engine;


int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    engine.Initialize();
    DUCKLOG_INFO("Engine initialized.");

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