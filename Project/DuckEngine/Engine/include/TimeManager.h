/******************************************************************************/
/*!
\file       TimeManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Declares the TimeManager class, which handles time-related
            operations including delta time calculation, FPS tracking, and
            managing system timers for the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>

/// <summary>
/// Manages timing and frame rate calculations for the game engine, including tracking FPS, delta time,
/// and total time elapsed in the game loop.
/// </summary>
class TimeManager {
public:
    /// <summary>
    /// Returns the current frames per second (FPS) based on the last time calculation.
    /// </summary>
    /// <returns>The current FPS as a double.</returns>
    static double FPS();

    /// <summary>
    /// Returns the delta time, which is the time taken to complete the most recent game loop.
    /// </summary>
    /// <returns>The delta time as a double.</returns>
    static DUCKENGINE_API double DT();

    /// <summary>
    /// Updates the time manager's internal state, calculating FPS and delta time. This should be called every frame.
    /// </summary>
    /// <param name="fpsCalcInt">Optional interval for calculating FPS, default is 1 second.</param>
    static void UpdateTime(double fpsCalcInt = 1.0);

    /// <summary>
    /// Starts the system timer for measuring elapsed time in a specific system or process.
    /// </summary>
    static void StartSystemTimer();
    static DUCKENGINE_API void StartManagerTimer(const std::string& managerName);
    /// <summary>
    /// Ends the system timer and returns the elapsed time since StartSystemTimer was called.
    /// </summary>
    /// <returns>The elapsed time in seconds.</returns>
    static double EndSystemTimer();
    static DUCKENGINE_API void EndManagerTimer(const std::string& managerName = "");
    /// <summary>
    /// Starts the timer for tracking total loop time.
    /// </summary>
    static DUCKENGINE_API void StartTotalTimer();

    /// <summary>
    /// Ends the timer for tracking total loop time.
    /// </summary>
    static DUCKENGINE_API void EndTotalTimer();

    /// <summary>
    /// Returns the total time elapsed in the current game loop.
    /// </summary>
    /// <returns>The total loop time as a double.</returns>
    static DUCKENGINE_API double GetTotalTime();

    /// <summary>
    /// Returns the data for the time manager.
    /// </summary>
    /// <returns>A vector of pairs containing the name of the manager and the time taken by the manager.</returns>
    static DUCKENGINE_API const std::unordered_map<std::string, double>& GetManagerData();


private:
    /// <summary>
    /// Stores the frames per second (FPS) value.
    /// </summary>
    static GLdouble fps;

    /// <summary>
    /// Stores the delta time, which is the time taken to complete the most recent game loop.
    /// </summary>
    static GLdouble delta_time;

    /// <summary>
    /// Stores the start time for systems, used in StartSystemTimer and EndSystemTimer.
    /// </summary>
    static double system_start_time;
    /// <summary>
    /// Stores the start time for tracking total loop time.
    /// </summary>
    static double total_time_start;

    /// <summary>
    /// Stores the total time elapsed in the current loop.
    /// </summary>
    static double total_time;

    /// <summary>
    /// Stores the data for all system managers.
    /// </summary>
    static std::unordered_map<std::string, double> managerData;
};
