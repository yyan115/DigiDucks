#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>


class DUCKENGINE_API InputManager {
public:
    static bool Initialize();

    static void Update();

    static void Exit();

    // I/O callbacks ...
    static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
    static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
    static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
    static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

private:

};
