/******************************************************************************/
/*!
\file       SystemManager.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the SystemManager class, providing functionality for
            managing and updating systems, as well as tracking performance
            data for each system.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "SystemManager.h"
#include "TimeManager.h"


/************************************************************************
@brief Adds a system to the SystemManager for it to manage. The system is
       stored internally and performance tracking data is initialized for it.
@param system A shared pointer to the system to be added.
*************************************************************************/
void SystemManager::AddSystem(std::shared_ptr<System> system) 
{
    systems.push_back(system);
    systemData.emplace_back(typeid(*system).name(), 0.0);
}

/************************************************************************
@brief Calls the Start method on all systems managed by the SystemManager.
       This initializes each system before the main update loop.
*************************************************************************/
void SystemManager::StartAll()
{
    for (std::shared_ptr<System>& system : systems)
    {
        system->Start();
    }
}

/************************************************************************
@brief Updates all systems by iterating through each system and calling
       their Update method. The performance of each system is tracked by
       recording the time it takes to complete the update.
*************************************************************************/
void SystemManager::UpdateAll() 
{
    for (std::shared_ptr<System>& system : systems) 
    {
        TimeManager::StartSystemTimer();
        system->Update();

        double elapsedTime = TimeManager::EndSystemTimer();  // Get time taken
        systemData[&system - &systems[0]].second = elapsedTime;    // Store time taken
        
    }
}

