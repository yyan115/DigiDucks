#pragma once

class UIManager {
public:
    static void Initialize();
    static void Render();
    static void Shutdown();

    
private:
    static void ShowInspector();
    static void ShowConsole();
};


