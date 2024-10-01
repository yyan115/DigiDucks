#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <unordered_map>  // For storing input states

// Forward declaration to avoid circular dependencies
struct GLFWwindow;

class InputManager {
public:
    // Initialize the input manager (returning success status)
    static bool Initialize(GLFWwindow* window);

    // Update the state of the input manager (called each frame)
    static void Update();

    // Clean up the input manager resources
    static void Exit();

    // Key input query functions
    static DUCKENGINE_API bool IsKeyDown(int key);        // Returns true while the key is held down
    static DUCKENGINE_API bool IsKeyPressed(int key);     // Returns true only in the frame the key was pressed
    static DUCKENGINE_API bool IsKeyReleased(int key);    // Returns true only in the frame the key was released

    // Similarly for mouse buttons
    static DUCKENGINE_API bool IsMouseButtonDown(int button);
    static DUCKENGINE_API bool IsMouseButtonPressed(int button);
    static DUCKENGINE_API bool IsMouseButtonReleased(int button);

    // Mouse position and scroll queries
    inline static DUCKENGINE_API double GetMouseX() { return mouseX; }
    inline  static DUCKENGINE_API double GetMouseY() { return mouseY; }
    inline static DUCKENGINE_API double GetScrollOffsetX() { return scrollX; }
    inline static DUCKENGINE_API double GetScrollOffsetY() { return scrollY; }

    // I/O callback functions
    static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
    static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
    static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
    static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

private:
    // Track input states
    static std::unordered_map<int, bool> keyStates;  // Key states: true if pressed, false if released
    static std::unordered_map<int, bool> mouseButtonStates;

    static std::unordered_map<int, bool> previousKeyStates;
    static std::unordered_map<int, bool> previousMouseButtonStates;

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
