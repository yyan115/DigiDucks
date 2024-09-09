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

private:
    static GLdouble fps;
    static GLdouble delta_time; // time taken to complete most recent game loop
};