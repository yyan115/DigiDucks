// EngineInterface.cpp
#include "EngineInterface.h"

#include "../../Engine/include/API_Test.h"
#include "../../Engine/include/EntityManager.h"

namespace ScriptAPI
{
    namespace
    {
		/* Select Many */
        ref struct Pair
        {
			System::Reflection::Assembly^ assembly;
			System::Type^ type;
        };


		System::Collections::Generic::IEnumerable<System::Type^>^ selectorFunc(System::Reflection::Assembly^ assembly)
		{
			return assembly->GetExportedTypes();
		}

		Pair^ resultSelectorFunc(System::Reflection::Assembly^ assembly, System::Type^ type)
		{
			Pair^ pair = gcnew Pair();
			pair->assembly = assembly;
			pair->type = type;
			return pair;
		}

		/* Where */ 
		bool predicateFunc(Pair^ pair) {
			return pair->type->IsSubclassOf(Script::typeid) && !pair->type->IsAbstract;
		}

		/* Select */
		System::Type^ selectorFunc(Pair^ pair) {
			return pair->type;
		}
    }


    void EngineInterface::HelloWorld()
    {
        System::Console::WriteLine("Hello Managed World!");
		Application::HelloWorld();
    }

    void EngineInterface::Init()
    {
		scripts = gcnew System::Collections::Generic::List<ScriptList^>();
		EntityManager entities;
		int currentNumEntities = entities.GetEntityCount();
		for (int i = 0; i < currentNumEntities; i++) {
			scripts->Add(gcnew ScriptList());
		}
		System::Console::WriteLine(currentNumEntities);

    }

	void EngineInterface::updateScriptTypeList()
	{
		using namespace System;
		using namespace System::Reflection;
		using namespace System::Linq;
		using namespace System::Collections::Generic;

		/* Select Many: Types in Loaded Assemblies */
		IEnumerable<Assembly^>^ assemblies = AppDomain::CurrentDomain->GetAssemblies();

		Func<Assembly^, IEnumerable<Type^>^>^ collectionSelector = gcnew Func<Assembly^, IEnumerable<Type^>^>(selectorFunc);

		Func<Assembly^, Type^, Pair^>^ resultSelector = gcnew Func<Assembly^, Type^, Pair^>(resultSelectorFunc);

		IEnumerable<Pair^>^ selectManyResult = Enumerable::SelectMany(assemblies, collectionSelector, resultSelector);

		/* Where: Are concrete Scripts */
		Func<Pair^, bool>^ predicate = gcnew Func<Pair^, bool>(predicateFunc);

		IEnumerable<Pair^>^ whereResult = Enumerable::Where(selectManyResult, predicate);

		/* Select: Select them all */
		Func<Pair^, Type^>^ selector = gcnew Func<Pair^, Type^>(selectorFunc);

		scriptTypeList = Enumerable::Select(whereResult, selector);
	}

	bool EngineInterface::AddScriptViaName(int entityId, System::String^ scriptName)
	{
		// Check if valid entity
		EntityManager entities;
		int currentNumEntities = entities.GetEntityCount();
		if (entityId < 0 || entityId > currentNumEntities)
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
		if (scriptType == nullptr)
			return false;
		// Create the script
		Script^ script = safe_cast<Script^>(System::Activator::CreateInstance(scriptType));

		// Add the script
		scripts[entityId]->Add(script);
		return true;
	}
}