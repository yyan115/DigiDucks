#pragma once

#include <vector>
#include <memory>
#include "System.h"


// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API SystemManager 
{
public:
    // Add a system to the manager
    void AddSystem(std::shared_ptr<System> system);

    // Update all systems
    void UpdateAll();

private:
    std::vector<std::shared_ptr<System>> systems;
};
