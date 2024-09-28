#include "SystemManager.h"
#include "TimeManager.h"


// Add a system to the system manager
void SystemManager::AddSystem(std::shared_ptr<System> system) 
{
    systems.push_back(system);
    // Initialize the systemData pair with the system name and 0.0 time
    systemData.emplace_back(typeid(*system).name(), 0.0);
}

void SystemManager::StartAll()
{
    for (std::shared_ptr<System>& system : systems)
    {
        system->Start();
    }
}

// Update all systems by iterating through each one and calling their Update method
void SystemManager::UpdateAll() 
{
    for (std::shared_ptr<System>& system : systems) 
    {
        TimeManager::StartSystemTimer();
        system->Update();

        double elapsedTime = TimeManager::EndSystemTimer();  // Get time taken
        systemData[&system - &systems[0]].second = elapsedTime;    // Store time taken
        totalTime += elapsedTime;	// Add to total time
        
    }
}
