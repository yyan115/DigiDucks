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
    
    
    void Start();

    void Initialize();
    void Update();
    void Draw();
    void Exit();

    bool Running();
};

#endif // DUCKENGINE_H
