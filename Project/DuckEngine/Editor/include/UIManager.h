#pragma once

class UIManager {
public:
    static void Initialize();
    static void Render();
    static void Exit();

    
private:
    static void ShowDebugInfo();
    static void ShowInspector();
    static void ShowConsole();
    static void ShowEntitySpawn();
    static void RenderWindows();
    static void ShowPerformance();
    static void SpawnSquare();
};




