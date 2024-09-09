#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class DUCKENGINE_API WindowManager {
public:
    static bool Initialize(GLint width, GLint height, std::string title);
    static void Exit();

    static GLFWwindow* getWindow();

    static bool CloseWindow();

    // callbacks
    static void error_cb(int error, char const* description);
    static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);

private:
    static GLFWwindow* ptrWindow;

    GLint width;
    GLint height;

    std::string title;
};