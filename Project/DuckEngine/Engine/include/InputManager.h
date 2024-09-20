#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <unordered_map>  // For storing input states

// Forward declaration to avoid circular dependencies
struct GLFWwindow;

class DUCKENGINE_API InputManager {
public:
    // Initialize the input manager (returning success status)
    static bool Initialize(GLFWwindow* window);

    // Update the state of the input manager (called each frame)
    static void Update();

    // Clean up the input manager resources
    static void Exit();

    // Key input query functions
    static bool IsKeyPressed(int key);
    static bool IsKeyReleased(int key);
    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonReleased(int button);

    // Mouse position and scroll queries
    inline static double GetMouseX() { return mouseX; }
    inline  static double GetMouseY() { return mouseY; }
    inline static double GetScrollOffsetX() { return scrollX; }
    inline static double GetScrollOffsetY() { return scrollY; }

    // I/O callback functions
    static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
    static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
    static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
    static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

private:
    // Track input states
    static std::unordered_map<int, bool> keyStates;  // Key states: true if pressed, false if released
    static std::unordered_map<int, bool> mouseButtonStates;

    // Mouse position and scroll offsets
    static double mouseX;
    static double mouseY;
    static double scrollX;
    static double scrollY;

    // Previous mouse position (optional if you want to track movement deltas)
    static double lastMouseX;
    static double lastMouseY;
};

// old
//#pragma once
//
//#ifdef DUCKENGINE_EXPORTS
//#define DUCKENGINE_API __declspec(dllexport)
//#else
//#define DUCKENGINE_API __declspec(dllimport)
//#endif
//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <string>
//
//
//class DUCKENGINE_API InputManager {
//public:
//    static bool Initialize();
//
//    static void Update();
//
//    static void Exit();
//
//    // I/O callbacks ...
//    static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
//    static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
//    static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
//    static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);
//
//private:
//
//};
