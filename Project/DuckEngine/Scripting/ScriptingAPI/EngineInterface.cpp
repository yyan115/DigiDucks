
#include "EngineInterface.h"
#include "Script.h"
#include "../../Engine/include/API_Test.h"

#pragma warning(disable : 4679)

namespace ScriptAPI
{
    void EngineInterface::HelloWorld()
    {
        System::Console::Out->WriteLine("Hello Managed World!");
		Application::HelloWorld();
    }

    void EngineInterface::Init()
    {
        System::Reflection::Assembly::LoadFrom("ManagedScript.dll");

        updateScriptTypeList();

        // Initialize the scripts list
        scripts = gcnew System::Collections::Generic::List<ScriptList^>();
        for (int i = 0; i < 2; ++i) {
            scripts->Add(gcnew ScriptList());
        }
    }


    namespace
    {
        /* Struct to Hold Assembly and Type Information */
        ref struct Pair {
            System::Reflection::Assembly^ assembly;
            System::Type^ type;
        };

        // Function to Get Exported Types from an Assembly
        System::Collections::Generic::IEnumerable<System::Type^>^ assemblySelectorFunc(System::Reflection::Assembly^ assembly) {
            return assembly->GetExportedTypes();
        }

        // Function to Create a Pair of Assembly and Type
        Pair^ resultSelectorFunc(System::Reflection::Assembly^ assembly, System::Type^ type) {
            Pair^ p = gcnew Pair();
            p->assembly = assembly;
            p->type = type;
            return p;
        }

        // Predicate to Filter Concrete Classes That Subclass Script
        bool predicateFunc(Pair^ pair) {
            return pair != nullptr && pair->type != nullptr &&
                pair->type->IsSubclassOf(Script::typeid) && !pair->type->IsAbstract;
        }

        // Function to Select Type from Pair
        System::Type^ typeSelectorFunc(Pair^ pair) {
            return pair != nullptr ? pair->type : nullptr;
        }
    }

    void EngineInterface::updateScriptTypeList()
    {
        using namespace System;
        using namespace System::Reflection;
        using namespace System::Collections::Generic;

        /* Select Many: Types in Loaded Assemblies */
        IEnumerable<Assembly^>^ assemblies = AppDomain::CurrentDomain->GetAssemblies();
        List<Pair^>^ selectManyResult = gcnew List<Pair^>();
        for each (System::Reflection::Assembly ^ assembly in assemblies)
        {
            auto types = assemblySelectorFunc(assembly); // GetExportedTypes for each assembly
            for each (System::Type ^ type in types)
            {
                Pair^ pair = resultSelectorFunc(assembly, type); // Create Pair
                selectManyResult->Add(pair);
            }
        }

        /* Where: Are concrete Scripts */
        List<Pair^>^ whereResult = gcnew List<Pair^>();
        for each (Pair ^ pair in selectManyResult)
        {
            if (predicateFunc(pair)) // Apply the predicate function to each Pair
            {
                whereResult->Add(pair);
            }
        }

        /* Select: Select all types that match the predicate */
        List<System::Type^>^ localScriptTypeList = gcnew List<System::Type^>();
        for each (Pair ^ pair in whereResult)
        {
            System::Type^ type = typeSelectorFunc(pair);
            if (type != nullptr) // Ensure the selected type is not null
            {
                localScriptTypeList->Add(type);
            }
        }

        // Assign to the class-level scriptTypeList if needed
        EngineInterface::scriptTypeList = localScriptTypeList;
    }


    bool EngineInterface::AddScriptViaName(int entityId, System::String^ scriptName)
    {
        // Check if valid entity
        if (entityId < 0 || entityId > 2)
            return false;

        // Remove any whitespaces just in case
        scriptName = scriptName->Trim();

        // Look for the correct script
        System::Type^ scriptType = nullptr;
        for each (System::Type ^ type in scriptTypeList)
        {
            if (type->FullName == scriptName || type->Name == scriptName)
            {
                scriptType = type;
                break;
            }
        }

        // Failed to get any script
        if (scriptType == nullptr) {
            return false;
        };
        // Create the script
        Script^ script = safe_cast<Script^>(System::Activator::CreateInstance(scriptType));

        System::Console::WriteLine("Added Script");
        
        // Add the script
        scripts[entityId]->Add(script);
        return true;
    }

    void EngineInterface::ExecuteUpdate()
    {
        for each (ScriptList ^ entityScriptList in scripts)
        {
            // Update each script
            for each (Script ^ script in entityScriptList)
            {
				System::Console::WriteLine("Update");
                script->Update();
            }
        }
    }
}