#pragma once

class UIManager {
public:
    static void Initialize();
    static void Render();
    static void Exit();

    
private:
    static void ShowInspector();
    static void ShowConsole();
    static void ShowEntitySpawn();
};


