#include "SystemManager.h"

// Add a system to the system manager
void SystemManager::AddSystem(std::shared_ptr<System> system) 
{
    systems.push_back(system);
}

// Update all systems by iterating through each one and calling their Update method
void SystemManager::UpdateAll() 
{
    for (auto& system : systems) 
    {
        system->Update();
    }
}
