#ifndef DUCKENGINE_H
#define DUCKENGINE_H

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class DUCKENGINE_API DuckEngine
{
public:
    static ComponentManager DUCKENGINE_ComponentManager;
    static EntityManager DUCKENGINE_EntityManager;
    static SystemManager DUCKENGINE_SystemManager;
    
    void Initialize();
    void Update();
    void StartDraw();
    void Draw();
    void EndDraw();
    void Exit();

    bool Running();

    static bool WKeyDown;
    static bool AKeyDown;
    static bool SKeyDown;
    static bool DKeyDown;

    static int GetWindowWidth();
    static int GetWindowHeight();

    static void SetBackgroundColor(const float r, const float g, const float b, const float a);
};

// Expose window dimension getters as global functions - REMOVED FOR NOW, BUT INTERESTING OPTION
//DUCKENGINE_API int GetWindowWidth();
//DUCKENGINE_API int GetWindowHeight();

#endif // DUCKENGINE_H
