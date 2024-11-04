#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include "../../extern/dotnet/include/coreclrhost.h"

#include <shlwapi.h>                // GetModuleFileNameA(), PathRemoveFileSpecA()
#pragma comment(lib, "shlwapi.lib") // Needed for <shlwapi.h>
#include <sstream>
#include <ostream>
#include <string>

class DUCKENGINE_API Application
{
public:
	void Run();
	static void HelloWorld();

private:
    void startScriptEngine();
    // References to CoreCLR key components
    HMODULE coreClr = nullptr;
    void* hostHandle = nullptr;
    unsigned int domainId = 0;


    // Function Pointers to CoreCLR functions
    coreclr_initialize_ptr      initializeCoreClr = nullptr;
    coreclr_create_delegate_ptr createManagedDelegate = nullptr;
    coreclr_shutdown_ptr        shutdownCoreClr = nullptr;
    // Helper Functions
    template<typename FunctType>
    FunctType getCoreClrFuncPtr(const std::string& functionName)
    {
        auto fPtr = reinterpret_cast<FunctType>(GetProcAddress(coreClr, functionName.c_str()));
        if (!fPtr)
            throw std::runtime_error("Unable to get pointer to function.");
        return fPtr;
    }

    // Helper Functions
    template<typename FunctionType>
    FunctionType GetFunctionPtr(const std::string_view& assemblyName, const std::string_view& typeName, const std::string_view& functionName)
    {
        FunctionType managedDelegate = nullptr;
        int result = createManagedDelegate
        (
            hostHandle,
            domainId,
            assemblyName.data(),
            typeName.data(),
            functionName.data(),
            reinterpret_cast<void**>(&managedDelegate)
        );
        // Check if it failed
        if (result < 0)
        {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0') << std::setw(8)
                << "[DotNetRuntime] Failed to get pointer to function \""
                << typeName << "." << functionName << "\" in assembly (" << assemblyName << "). "
                << "Error 0x" << result << "\n";
            throw std::runtime_error(oss.str());
        }
        return managedDelegate;
    }

    std::string buildTpaList(const std::string& directory);

	void stopScriptEngine();
};

