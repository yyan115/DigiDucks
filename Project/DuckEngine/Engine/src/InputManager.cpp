#include "InputManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

// Define static variables here (without DUCKENGINE_API)
std::unordered_map<int, bool> InputManager::keyStates;
std::unordered_map<int, bool> InputManager::mouseButtonStates;
std::unordered_map<int, bool> InputManager::previousKeyStates;
std::unordered_map<int, bool> InputManager::previousMouseButtonStates;

double InputManager::mouseX = 0.0;
double InputManager::mouseY = 0.0;
double InputManager::scrollX = 0.0;
double InputManager::scrollY = 0.0;
double InputManager::lastMouseX = 0.0;
double InputManager::lastMouseY = 0.0;


bool InputManager::Initialize(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetKeyCallback(window, InputManager::key_cb);
    glfwSetMouseButtonCallback(window, InputManager::mousebutton_cb);
    glfwSetCursorPosCallback(window, InputManager::mousepos_cb);
    glfwSetScrollCallback(window, InputManager::mousescroll_cb);

    return true;
}

void InputManager::Update() {
    // Save previous states
    previousKeyStates = keyStates;
    previousMouseButtonStates = mouseButtonStates;

    // Reset scroll offsets
    scrollX = 0.0;
    scrollY = 0.0;

    // Poll for new events, updating current states
    glfwPollEvents();
}

void InputManager::Exit() {
    // Clean up resources if necessary
}

bool InputManager::IsKeyDown(int key) {
    return keyStates[key];
}

bool InputManager::IsKeyPressed(int key) {
    return keyStates[key] && !previousKeyStates[key];
}

bool InputManager::IsKeyReleased(int key) {
    return !keyStates[key] && previousKeyStates[key];
}

bool InputManager::IsMouseButtonDown(int button) {
    return mouseButtonStates[button];
}

bool InputManager::IsMouseButtonPressed(int button) {
    return mouseButtonStates[button] && !previousMouseButtonStates[button];
}

bool InputManager::IsMouseButtonReleased(int button) {
    return !mouseButtonStates[button] && previousMouseButtonStates[button];
}


void InputManager::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}

void InputManager::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod) {
    if (action == GLFW_PRESS) {
        mouseButtonStates[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        mouseButtonStates[button] = false;
    }
}

void InputManager::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset) {
    scrollX = xoffset;
    scrollY = yoffset;
}

void InputManager::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

// old copied from assignment
//#include <iostream>
//
//#include "DuckEngine.h"
//#include "InputManager.h"
//#include "WindowManager.h"
//#include "UIDebugConsole.h"
//
//bool InputManager::Initialize() {
//
//    glfwSetInputMode(WindowManager::getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//
//    glfwSetKeyCallback(WindowManager::getWindow(), InputManager::key_cb);
//    glfwSetMouseButtonCallback(WindowManager::getWindow(), InputManager::mousebutton_cb);
//    glfwSetCursorPosCallback(WindowManager::getWindow(), InputManager::mousepos_cb);
//    glfwSetScrollCallback(WindowManager::getWindow(), InputManager::mousescroll_cb);
//
//    return true;
//}
//
//void InputManager::Update() {
//    glfwPollEvents();
//}
//
///*  _________________________________________________________________________*/
///*! key_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param int
//the keyboard key that was pressed or released
//
//@parm int
//Platform-specific scancode of the key
//
//@parm int
//GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
//action will be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it,
//for example E-mail and Play keys.
//
//@parm int
//bit-field describing which modifier keys (shift, alt, control)
//were held down
//
//@return none
//
//This function is called when keyboard buttons are pressed.
//When the ESC key is pressed, the close flag of the window is set.
//*/
//void InputManager::key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
//    if (GLFW_PRESS == action) {
//#ifdef _DEBUG
//        std::cout << "Key pressed" << std::endl;
//#endif
//    }
//    else if (GLFW_REPEAT == action) {
//#ifdef _DEBUG
//        std::cout << "Key repeatedly pressed" << std::endl;
//#endif
//    }
//    else if (GLFW_RELEASE == action) {
//#ifdef _DEBUG
//        std::cout << "Key released" << std::endl;
//#endif
//    }
//
//    // key state changes from released to pressed
//    if (GLFW_PRESS == action) {
//        if (GLFW_KEY_ESCAPE == key) {
//            glfwSetWindowShouldClose(pwin, GLFW_TRUE);
//        }
//        if (key == GLFW_KEY_W)
//        {
//            DuckEngine::WKeyDown = true;
//        }
//
//        if (key == GLFW_KEY_A)
//        {
//            DuckEngine::AKeyDown = true;
//        }
//
//        if (key == GLFW_KEY_S)
//        {
//            DuckEngine::SKeyDown = true;
//        }
//
//        if (key == GLFW_KEY_D)
//        {
//            DuckEngine::DKeyDown = true;
//        }
//
//        // less buggy, but doesnt mimic sample as well
//        //if (key == GLFW_KEY_V)
//        //    GLApp::camera2d.camtype_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_Z)
//        //    GLApp::camera2d.zoom_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_H)
//        //    GLApp::camera2d.left_turn_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_K)
//        //    GLApp::camera2d.right_turn_flag = GL_TRUE;
//
//        //if (key == GLFW_KEY_U)
//        //    GLApp::camera2d.move_flag = GL_TRUE;
//
//        // set accordingly
//        //GLApp::camera2d.camtype_flag = (key == GLFW_KEY_V) ? GL_TRUE : GL_FALSE;
//        //GLApp::camera2d.zoom_flag = (key == GLFW_KEY_Z) ? GL_TRUE : GL_FALSE;
//        //GLApp::camera2d.left_turn_flag = (key == GLFW_KEY_H) ? GL_TRUE : GL_FALSE;
//        //GLApp::camera2d.right_turn_flag = (key == GLFW_KEY_K) ? GL_TRUE : GL_FALSE;
//        //GLApp::camera2d.move_flag = (key == GLFW_KEY_U) ? GL_TRUE : GL_FALSE;
//    }
//    else if (GLFW_REPEAT == action) {
//        // key state was and is being pressed (comment out to allow holding)
//        //GLApp::camera2d.camtype_flag    = GL_FALSE;
//        //GLApp::camera2d.zoom_flag       = GL_FALSE;
//        //GLApp::camera2d.left_turn_flag  = GL_FALSE;
//        //GLApp::camera2d.right_turn_flag = GL_FALSE;
//        //GLApp::camera2d.move_flag       = GL_FALSE;
//
//        if (key == GLFW_KEY_W)
//        {
//            DuckEngine::WKeyDown = true;
//        }
//
//        if (key == GLFW_KEY_A)
//        {
//            DuckEngine::AKeyDown = true;
//        }
//
//        if (key == GLFW_KEY_S)
//        {
//            DuckEngine::SKeyDown = true;
//        }
//
//        if (key == GLFW_KEY_D)
//        {
//            DuckEngine::DKeyDown = true;
//        }
//    }
//    else if (GLFW_RELEASE == action) {
//        // key start changes from pressed to released
//        //GLApp::camera2d.camtype_flag = GL_FALSE;
//        //GLApp::camera2d.zoom_flag = GL_FALSE;
//        //GLApp::camera2d.left_turn_flag = GL_FALSE;
//        //GLApp::camera2d.right_turn_flag = GL_FALSE;
//        //GLApp::camera2d.move_flag = GL_FALSE;
//
//
//        if (key == GLFW_KEY_W)
//        {
//            DuckEngine::WKeyDown = false;
//        }
//
//        if (key == GLFW_KEY_A)
//        {
//            DuckEngine::AKeyDown = false;
//        }
//
//        if (key == GLFW_KEY_S)
//        {
//            DuckEngine::SKeyDown = false;
//        }
//
//        if (key == GLFW_KEY_D)
//        {
//            DuckEngine::DKeyDown = false;
//        }
//    }
//}
//
///*  _________________________________________________________________________*/
///*! mousebutton_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param int
//the mouse button that was pressed or released
//GLFW_MOUSE_BUTTON_LEFT and GLFW_MOUSE_BUTTON_RIGHT specifying left and right
//mouse buttons are most useful
//
//@parm int
//action is either GLFW_PRESS or GLFW_RELEASE
//
//@parm int
//bit-field describing which modifier keys (shift, alt, control)
//were held down
//
//@return none
//
//This function is called when mouse buttons are pressed.
//*/
//void InputManager::mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod) {
//    switch (button) {
//    case GLFW_MOUSE_BUTTON_LEFT:
//#ifdef _DEBUG
//        std::cout << "Left mouse button ";
//#endif
//        break;
//    case GLFW_MOUSE_BUTTON_RIGHT:
//#ifdef _DEBUG
//        std::cout << "Right mouse button ";
//#endif
//        break;
//    }
//    switch (action) {
//    case GLFW_PRESS:
//#ifdef _DEBUG
//        std::cout << "pressed!!!" << std::endl;
//#endif
//        break;
//    case GLFW_RELEASE:
//#ifdef _DEBUG
//        std::cout << "released!!!" << std::endl;
//#endif
//        break;
//    }
//}
//
///*  _________________________________________________________________________*/
///*! mousepos_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param double
//new cursor x-coordinate, relative to the left edge of the client area
//
//@param double
//new cursor y-coordinate, relative to the top edge of the client area
//
//@return none
//
//This functions receives the cursor position, measured in screen coordinates but
//relative to the top-left corner of the window client area.
//*/
//void InputManager::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
//#ifdef _DEBUG
//    UIDebugConsole::debugConsole.AddDebugLog("Mouse moved to (%.2f, %.2f)", xpos, ypos);
//#endif
//}
//
///*  _________________________________________________________________________*/
///*! mousescroll_cb
//
//@param GLFWwindow*
//Handle to window that is receiving event
//
//@param double
//Scroll offset along X-axis
//
//@param double
//Scroll offset along Y-axis
//
//@return none
//
//This function is called when the user scrolls, whether with a mouse wheel or
//touchpad gesture. Although the function receives 2D scroll offsets, a simple
//mouse scroll wheel, being vertical, provides offsets only along the Y-axis.
//*/
//void InputManager::mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset) {
//#ifdef _DEBUG
//    std::cout << "Mouse scroll wheel offset: ("
//        << xoffset << ", " << yoffset << ")" << std::endl;
//#endif
//}
