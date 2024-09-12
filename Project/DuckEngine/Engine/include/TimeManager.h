#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DUCKENGINE_API TimeManager {
public:
    static double FPS();
    static double DT();

    static void UpdateTime(double fpsCalcInt = 1.0);

    static void StartSystemTimer(); // Start time for system
    static double EndSystemTimer(); // End time for system and return elapsed time
    
    // Total loop time tracking
    static void StartTotalTimer();
    static void EndTotalTimer();
    static double GetTotalTime(); // Get the total loop time

private:
    static GLdouble fps;
    static GLdouble delta_time; // time taken to complete most recent game loop

    static double system_start_time; // Stores the start time for systems
    static double total_time_start;    // Start time for the total loop
    static double total_time;          // Total time for the current loop
};