#include "TimeManager.h"

// Define static members
GLdouble TimeManager::fps = 0.0;
GLdouble TimeManager::delta_time = 0.0;
double TimeManager::system_start_time = 0.0;
double TimeManager::total_time_start = 0.0;
double TimeManager::total_time = 0.0;

double TimeManager::FPS() {
    return fps;
};

double TimeManager::DT() {
    return delta_time;
};

/*  _________________________________________________________________________*/
/*! update_time

@param double
fps_calc_interval: the interval (in seconds) at which fps is to be
calculated

This function must be called once per game loop. It uses GLFW's time functions
to compute:
1. the interval in seconds between each frame
2. the frames per second every "fps_calc_interval" seconds
*/
void TimeManager::UpdateTime(double fps_calc_interval) {
    // get elapsed time (in seconds) between previous and current frames
    static double prev_time = glfwGetTime();
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

// Start timer for system updates
void TimeManager::StartSystemTimer() {
    system_start_time = glfwGetTime();
}

// End timer and calculate time taken by system
double TimeManager::EndSystemTimer() {
    double end_time = glfwGetTime();
    return end_time - system_start_time;
}

// Total loop time tracking
void TimeManager::StartTotalTimer() {
    total_time_start = glfwGetTime();
}

void TimeManager::EndTotalTimer() {
    total_time = glfwGetTime() - total_time_start;
}

double TimeManager::GetTotalTime() {
    return total_time;
}