/******************************************************************************/
/*!
\file       SystemManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the SystemManager class responsible for managing and
            updating all systems in the game engine. It includes functionality
            to add systems, start them, update them, and track system data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <vector>
#include <memory>
#include <string>
#include "System.h"

/************************************************************************
@brief The SystemManager class is responsible for managing and orchestrating
       multiple systems in the game engine. It provides methods to add, start,
       and update all systems, as well as track system data for performance
       monitoring.
*************************************************************************/
class SystemManager 
{
public:

    /************************************************************************
    @brief Adds a system to the SystemManager for it to manage.
    @param system A shared pointer to the system to be added.
    *************************************************************************/
    void AddSystem(std::shared_ptr<System> system);

    /************************************************************************
    @brief Starts all systems managed by the SystemManager by calling their
           Start method.
    *************************************************************************/
    void StartAll();

    /************************************************************************
    @brief Updates all systems managed by the SystemManager by calling their
           Update method.
    *************************************************************************/
    void UpdateAll();

    void FixedUpdateAll();

    /************************************************************************
    @brief Cleans up and exits all systems.
    *************************************************************************/
    void RenderAll();

    /************************************************************************
    @brief Retrieves the performance data for all systems.
    @return A constant reference to a vector of pairs containing system names
            and their corresponding execution times.
    *************************************************************************/
    const std::vector<std::pair<std::string, double>>& GetSystemData() const { return systemData; }   

private:
    std::vector<std::shared_ptr<System>> systems;
    std::vector<std::pair<std::string, double>> systemData;
};
