// EngineInterface.cpp
#include "EngineInterface.h"

#include "../../Engine/include/API_Test.h"

namespace ScriptAPI
{
    void EngineInterface::HelloWorld()
    {
        System::Console::WriteLine("Hello Managed World!");
		Application::HelloWorld();
    }
}