/******************************************************************************/
/*!
\file       InputManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the InputManager class, processing input events and
            updating the state of keys and mouse buttons for use in the
            game engine's input handling system.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "InputManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

#define UNREFERENCED_PARAMETER(P) (P)

std::unordered_map<int, bool> InputManager::keyStates;
std::unordered_map<int, bool> InputManager::mouseButtonStates;
std::unordered_map<int, bool> InputManager::previousKeyStates;
std::unordered_map<int, bool> InputManager::previousMouseButtonStates;
std::unordered_map<int, std::unordered_map<int, bool>> InputManager::gamepadButtonStates;
std::unordered_map<int, std::unordered_map<int, bool>> InputManager::previousGamepadButtonStates;
std::unordered_map<int, std::unordered_map<int, float>> InputManager::gamepadAxisStates;

double InputManager::mouseX = 0.0;
double InputManager::mouseY = 0.0;
double InputManager::scrollX = 0.0;
double InputManager::scrollY = 0.0;
double InputManager::lastMouseX = 0.0;
double InputManager::lastMouseY = 0.0;

/// <summary>
/// Initializes the input manager by setting up GLFW callbacks for keyboard and mouse input.
/// </summary>
/// <param name="window">The GLFW window to associate with the input manager.</param>
/// <returns>Returns true if initialization was successful.</returns>
bool InputManager::Initialize(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetKeyCallback(window, InputManager::keyCB);
    glfwSetMouseButtonCallback(window, InputManager::mouseButtonCB);
    glfwSetCursorPosCallback(window, InputManager::mousePosCB);
    glfwSetScrollCallback(window, InputManager::mouseScrollCB);

    // Set up joystick/gamepad callback
    glfwSetJoystickCallback(joystickCB);

    // Check for already connected gamepads
    for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
        if (glfwJoystickPresent(jid)) {
            // Initialize gamepad state maps for this joystick
            gamepadButtonStates[jid] = std::unordered_map<int, bool>();
            previousGamepadButtonStates[jid] = std::unordered_map<int, bool>();
            gamepadAxisStates[jid] = std::unordered_map<int, float>();
        }
    }

    return true;
}

/// <summary>
/// Updates the input manager's state by saving the previous states of keys and mouse buttons
/// and polling for new input events.
/// </summary>
void InputManager::Update() {
    // Update previous states
    previousKeyStates = keyStates;
    previousMouseButtonStates = mouseButtonStates;
    previousGamepadButtonStates = gamepadButtonStates;

    // Update gamepad states
    for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
        if (glfwJoystickPresent(jid)) {
            // Get gamepad button states
            int buttonCount;
            const unsigned char* buttons = glfwGetJoystickButtons(jid, &buttonCount);
            for (int i = 0; i < buttonCount; i++) {
                gamepadButtonStates[jid][i] = (buttons[i] == GLFW_PRESS);
            }

            // Get gamepad axis states
            int axisCount;
            const float* axes = glfwGetJoystickAxes(jid, &axisCount);
            for (int i = 0; i < axisCount; i++) {
                gamepadAxisStates[jid][i] = axes[i];
            }
        }
    }

    // Reset scroll values
    scrollX = 0.0;
    scrollY = 0.0;

    // Poll for new events, updating current states
    glfwPollEvents();

    if (!IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) 
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
}

/// <summary>
/// Cleans up any resources used by the input manager. This function is currently a placeholder for future use.
/// </summary>
void InputManager::Exit() {
    // Clean up resources if necessary
}

/// <summary>
/// Checks if the specified key is currently held down.
/// </summary>
/// <param name="key">The key code to check.</param>
/// <returns>Returns true if the key is held down.</returns>
bool InputManager::IsKeyDown(int key) {
    return keyStates[key];
}

/// <summary>
/// Checks if the specified key was pressed during the current frame.
/// </summary>
/// <param name="key">The key code to check.</param>
/// <returns>Returns true if the key was pressed this frame.</returns>
bool InputManager::IsKeyPressed(int key) {
    return keyStates[key] && !previousKeyStates[key];
}

/// <summary>
/// Checks if the specified key was released during the current frame.
/// </summary>
/// <param name="key">The key code to check.</param>
/// <returns>Returns true if the key was released this frame.</returns>
bool InputManager::IsKeyReleased(int key) {
    return !keyStates[key] && previousKeyStates[key];
}

/// <summary>
/// Checks if the specified mouse button is currently held down.
/// </summary>
/// <param name="button">The mouse button to check.</param>
/// <returns>Returns true if the mouse button is held down.</returns>
bool InputManager::IsMouseButtonDown(int button) {
    return mouseButtonStates[button];
}

/// <summary>
/// Checks if the specified mouse button was pressed during the current frame.
/// </summary>
/// <param name="button">The mouse button to check.</param>
/// <returns>Returns true if the mouse button was pressed this frame.</returns>
bool InputManager::IsMouseButtonPressed(int button) {
    return mouseButtonStates[button] && !previousMouseButtonStates[button];
}

/// <summary>
/// Checks if the specified mouse button was released during the current frame.
/// </summary>
/// <param name="button">The mouse button to check.</param>
/// <returns>Returns true if the mouse button was released this frame.</returns>
bool InputManager::IsMouseButtonReleased(int button) {
    return !mouseButtonStates[button] && previousMouseButtonStates[button];
}

/// <summary>
/// Callback function to handle keyboard input events. Updates the key state based on the event action.
/// </summary>
/// <param name="pwin">Pointer to the GLFW window receiving the event.</param>
/// <param name="key">The key that was pressed or released.</param>
/// <param name="scancode">The scancode of the key.</param>
/// <param name="action">The action performed (press or release).</param>
/// <param name="mod">Modifier keys held down during the event.</param>
void InputManager::keyCB(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
    UNREFERENCED_PARAMETER(scancode);
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(pwin);

    if (action == GLFW_PRESS) {
        keyStates[key] = true;
//#ifdef _DEBUG
//        std::cout << "Key Pressed: " << key << std::endl;
//#endif
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
//#ifdef _DEBUG
//        std::cout << "Key Released: " << key << std::endl;
//#endif
    }
}

/// <summary>
/// Callback function to handle mouse button input events. Updates the mouse button state based on the event action.
/// </summary>
/// <param name="pwin">Pointer to the GLFW window receiving the event.</param>
/// <param name="button">The mouse button that was pressed or released.</param>
/// <param name="action">The action performed (press or release).</param>
/// <param name="mod">Modifier keys held down during the event.</param>
void InputManager::mouseButtonCB(GLFWwindow* pwin, int button, int action, int mod) {
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(pwin);

    if (action == GLFW_PRESS) {
        mouseButtonStates[button] = true;
//#ifdef _DEBUG
//        std::cout << "Mouse Button Pressed: " << button << std::endl;
//#endif
    }
    else if (action == GLFW_RELEASE) {
        mouseButtonStates[button] = false;
//#ifdef _DEBUG
//        std::cout << "Mouse Button Released: " << button << std::endl;
//#endif
    }
}

/// <summary>
/// Callback function to handle mouse scroll events. Updates the scroll offsets based on the event data.
/// </summary>
/// <param name="pwin">Pointer to the GLFW window receiving the event.</param>
/// <param name="xoffset">The horizontal scroll offset.</param>
/// <param name="yoffset">The vertical scroll offset.</param>
void InputManager::mouseScrollCB(GLFWwindow* pwin, double xoffset, double yoffset) {
    UNREFERENCED_PARAMETER(pwin);

    scrollX = xoffset;
    scrollY = yoffset;

//#ifdef _DEBUG
//    std::cout << "Mouse Scroll: X Offset = " << xoffset << ", Y Offset = " << yoffset << std::endl;
//#endif
}

/// <summary>
/// Callback function to handle mouse position events. Updates the mouse position based on the event data.
/// </summary>
/// <param name="pwin">Pointer to the GLFW window receiving the event.</param>
/// <param name="xpos">The new x-coordinate of the mouse.</param>
/// <param name="ypos">The new y-coordinate of the mouse.</param>
void InputManager::mousePosCB(GLFWwindow* pwin, double xpos, double ypos) {
    UNREFERENCED_PARAMETER(pwin);

    lastMouseX = mouseX;
    lastMouseY = mouseY;
    mouseX = xpos;
    mouseY = ypos;
}

bool InputManager::IsMiddleMouseDown() 
{ 
    return mouseButtonStates[GLFW_MOUSE_BUTTON_MIDDLE];
}

void InputManager::ResetLastMousePosition()
{
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

bool InputManager::IsGamepadConnected(int gamepadIndex) {
    return glfwJoystickPresent(gamepadIndex) == GLFW_TRUE;
}

bool InputManager::IsGamepadButtonDown(int gamepadIndex, int button) {
    if (!IsGamepadConnected(gamepadIndex)) return false;
    auto& buttonStates = gamepadButtonStates[gamepadIndex];
    return buttonStates.find(button) != buttonStates.end() && buttonStates[button];
}

bool InputManager::IsGamepadButtonPressed(int gamepadIndex, int button) {
    if (!IsGamepadConnected(gamepadIndex)) return false;
    auto& currentStates = gamepadButtonStates[gamepadIndex];
    auto& prevStates = previousGamepadButtonStates[gamepadIndex];
    
    bool currentlyPressed = currentStates.find(button) != currentStates.end() && currentStates[button];
    bool previouslyPressed = prevStates.find(button) != prevStates.end() && prevStates[button];
    
    return currentlyPressed && !previouslyPressed;
}

bool InputManager::IsGamepadButtonReleased(int gamepadIndex, int button) {
    if (!IsGamepadConnected(gamepadIndex)) return false;
    auto& currentStates = gamepadButtonStates[gamepadIndex];
    auto& prevStates = previousGamepadButtonStates[gamepadIndex];
    
    bool currentlyPressed = currentStates.find(button) != currentStates.end() && currentStates[button];
    bool previouslyPressed = prevStates.find(button) != prevStates.end() && prevStates[button];
    
    return !currentlyPressed && previouslyPressed;
}

float InputManager::GetGamepadAxisValue(int gamepadIndex, int axis) {
    if (!IsGamepadConnected(gamepadIndex)) return 0.0f;
    auto& axisStates = gamepadAxisStates[gamepadIndex];
    return axisStates.find(axis) != axisStates.end() ? axisStates[axis] : 0.0f;
}

void InputManager::joystickCB(int jid, int event) {
    if (event == GLFW_CONNECTED) {
        // Initialize state maps for the newly connected gamepad
        gamepadButtonStates[jid] = std::unordered_map<int, bool>();
        previousGamepadButtonStates[jid] = std::unordered_map<int, bool>();
        gamepadAxisStates[jid] = std::unordered_map<int, float>();
    }
    else if (event == GLFW_DISCONNECTED) {
        // Clean up state maps for the disconnected gamepad
        gamepadButtonStates.erase(jid);
        previousGamepadButtonStates.erase(jid);
        gamepadAxisStates.erase(jid);
    }
}

