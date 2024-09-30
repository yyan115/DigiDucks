#pragma once

#include <vector>
#include <memory>
#include <string>
#include "System.h"


class SystemManager 
{
public:
    // Add a system to the manager
    void AddSystem(std::shared_ptr<System> system);

    // Start all systems
    void StartAll();

    // Update all systems
    void UpdateAll();

    void Exit();

    const std::vector<std::pair<std::string, double>>& GetSystemData() const { return systemData; }   
    double GetTotalTime() const { return totalTime; }

private:
    std::vector<std::shared_ptr<System>> systems;
    std::vector<std::pair<std::string, double>> systemData;
    double totalTime = 0.0;
};
