/******************************************************************************/
/*!
\file       TimeManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the TimeManager class, which handles frame timing,
            calculating delta time, tracking FPS, and managing timers for
            various systems within the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "TimeManager.h"
#include <iostream>
#include <unordered_map>

// Define static members
GLdouble TimeManager::fps = 0.0;
GLdouble TimeManager::delta_time = 0.0;
double TimeManager::system_start_time = 0.0;
double TimeManager::total_time_start = 0.0;
double TimeManager::total_time = 0.0;
std::unordered_map<std::string, double> Manager_start_time;
std::unordered_map<std::string, double> TimeManager::managerData;

double TimeManager::prev_time = glfwGetTime();

/// <summary>
/// Returns the current frames per second (FPS).
/// </summary>
/// <returns>The current FPS as a double.</returns>
double TimeManager::FPS() {
    return fps;
};

/// <summary>
/// Returns the delta time, which is the time taken to complete the most recent game loop.
/// </summary>
/// <returns>The delta time as a double.</returns>
double TimeManager::DT() {
    return delta_time;
};

/// <summary>
/// Updates the time manager's state by calculating the delta time (time between frames) 
/// and the frames per second (FPS). This function must be called once per game loop.
/// FPS is recalculated every "fps_calc_interval" seconds.
/// </summary>
/// <param name="fps_calc_interval">The interval (in seconds) at which FPS is recalculated. Defaults to 1.0 second.</param>
void TimeManager::UpdateTime(double fps_calc_interval) {

    // get elapsed time (in seconds) between previous and current frames
    double curr_time = glfwGetTime();
    delta_time = curr_time - prev_time;
    prev_time = curr_time;

    // fps calculations
    static double count = 0.0; // number of game loop iterations
    static double start_time = glfwGetTime();

    // get elapsed time since very beginning (in seconds) ...
    double elapsed_time = curr_time - start_time;

    ++count;

    // update fps at least every 10 seconds ...
    fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
    fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
    if (elapsed_time > fps_calc_interval) {
        TimeManager::fps = count / elapsed_time;
        start_time = curr_time;
        count = 0.0;
    }
}

/// <summary>
/// Starts the timer for tracking system execution time.
/// </summary>
void TimeManager::StartSystemTimer() {
    system_start_time = glfwGetTime();
}

void TimeManager::StartManagerTimer(const std::string& managerName) {
    // Record the start time for the manager
    Manager_start_time[managerName] = glfwGetTime();
}


/// <summary>
/// Ends the system timer and returns the time taken by the system or process.
/// </summary>
/// <returns>The elapsed time since StartSystemTimer() was called.</returns>
double TimeManager::EndSystemTimer() {
    double end_time = glfwGetTime();
    return end_time - system_start_time;
}

void TimeManager::EndManagerTimer(const std::string& managerName) {
    double end_time = glfwGetTime();

    // Calculate elapsed time if start time is available
    double elapsed_time = end_time - Manager_start_time[managerName];

    // Update the manager's elapsed time in managerData
    managerData[managerName] = elapsed_time;

    // Remove the start time entry
    Manager_start_time.erase(managerName);
}

/// <summary>
/// Starts the timer for tracking total loop time.
/// </summary>
void TimeManager::StartTotalTimer() {
    total_time_start = glfwGetTime();
}

/// <summary>
/// Ends the timer for tracking total loop time and updates the total time.
/// </summary>
void TimeManager::EndTotalTimer() {
    total_time = glfwGetTime() - total_time_start;
}

/// <summary>
/// Returns the total time elapsed in the current game loop.
/// </summary>
/// <returns>The total loop time as a double.</returns>
double TimeManager::GetTotalTime() {
    return total_time;
}

/// <summary>
/// Returns the data for all system managers.
/// </summary>
/// <returns> A vector of pairs, where each pair contains the name of a system manager and the time taken by that manager.</returns>
const std::unordered_map<std::string, double>& TimeManager::GetManagerData() {
    return managerData;
}

void TimeManager::ResetPrevTime()
{
    prev_time = glfwGetTime();
}