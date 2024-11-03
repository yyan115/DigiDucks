// EngineInterface.h
#pragma once

#include "Script.h"

namespace ScriptAPI
{
    // ref classes are classes in C#, value classes are structs in C#
    public ref class EngineInterface
    {
    public:
        static void HelloWorld();

        static void Init();

    private:
        using ScriptList = System::Collections::Generic::List<Script^>;
        static System::Collections::Generic::List<ScriptList^>^ scripts;
        static System::Collections::Generic::IEnumerable<System::Type^>^ scriptTypeList;
        static void updateScriptTypeList();
    };
}