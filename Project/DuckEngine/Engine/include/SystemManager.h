#pragma once

#include <vector>
#include <memory>
#include <string>
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

    // Start all systems
    void StartAll();

    // Update all systems
    void UpdateAll();

    const std::vector<std::pair<std::string, double>>& GetSystemData() const { return systemData; }   
    double GetTotalTime() const { return totalTime; }

private:
    std::vector<std::shared_ptr<System>> systems;
    std::vector<std::pair<std::string, double>> systemData; // Pair of system name and time taken
    double totalTime = 0.0;
};
