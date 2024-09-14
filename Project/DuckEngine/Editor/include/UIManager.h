#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API UIManager {
public:
    static void Initialize();
    static void Render();
    static void Shutdown();

    
private:
    static void ShowInspector();
    static void ShowConsole();
};


