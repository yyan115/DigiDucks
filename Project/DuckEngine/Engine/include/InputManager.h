/******************************************************************************/
/*!
\file       InputManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Declares the InputManager class responsible for managing keyboard
            and mouse input, including handling input events and storing input
            states for the game engine.

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

#include <unordered_map>

// Forward declaration to avoid circular dependencies
struct GLFWwindow;

/// <summary>
/// Manages all input handling for the game, including keyboard and mouse input.
/// Tracks the state of keys and mouse buttons and provides utility functions to query them.
/// </summary>
class InputManager {
public:
    /// <summary>
    /// Initializes the input manager by setting up the necessary callbacks and resources.
    /// </summary>
    /// <param name="window">The GLFW window associated with the input manager.</param>
    /// <returns>Returns true if initialization was successful, false otherwise.</returns>
    static bool Initialize(GLFWwindow* window);

    /// <summary>
    /// Updates the input manager's state. This should be called every frame to process input changes.
    /// </summary>
    static void Update();

    /// <summary>
    /// Cleans up any resources used by the input manager.
    /// </summary>
    static void Exit();

    /// <summary>
    /// Returns true while the specified key is held down.
    /// </summary>
    /// <param name="key">The key code to query.</param>
    /// <returns>True if the key is currently held down, false otherwise.</returns>
    static DUCKENGINE_API bool IsKeyDown(int key);

    /// <summary>
    /// Returns true only in the frame when the specified key was pressed.
    /// </summary>
    /// <param name="key">The key code to query.</param>
    /// <returns>True if the key was pressed this frame, false otherwise.</returns>
    static DUCKENGINE_API bool IsKeyPressed(int key);

    /// <summary>
    /// Returns true only in the frame when the specified key was released.
    /// </summary>
    /// <param name="key">The key code to query.</param>
    /// <returns>True if the key was released this frame, false otherwise.</returns>
    static DUCKENGINE_API bool IsKeyReleased(int key);

    /// <summary>
    /// Returns true while the specified mouse button is held down.
    /// </summary>
    /// <param name="button">The mouse button to query.</param>
    /// <returns>True if the mouse button is currently held down, false otherwise.</returns>
    static DUCKENGINE_API bool IsMouseButtonDown(int button);

    /// <summary>
    /// Returns true only in the frame when the specified mouse button was pressed.
    /// </summary>
    /// <param name="button">The mouse button to query.</param>
    /// <returns>True if the mouse button was pressed this frame, false otherwise.</returns>
    static DUCKENGINE_API bool IsMouseButtonPressed(int button);

    /// <summary>
    /// Returns true only in the frame when the specified mouse button was released.
    /// </summary>
    /// <param name="button">The mouse button to query.</param>
    /// <returns>True if the mouse button was released this frame, false otherwise.</returns>
    static DUCKENGINE_API bool IsMouseButtonReleased(int button);

    /// <summary>
    /// Returns the current x-coordinate of the mouse in the window.
    /// </summary>
    /// <returns>The mouse's x-coordinate in window space.</returns>
    inline static DUCKENGINE_API double GetMouseX() { return mouseX; }

    /// <summary>
    /// Returns the current y-coordinate of the mouse in the window.
    /// </summary>
    /// <returns>The mouse's y-coordinate in window space.</returns>
    inline static DUCKENGINE_API double GetMouseY() { return mouseY; }

    /// <summary>
    /// Returns the x offset of the mouse scroll wheel.
    /// </summary>
    /// <returns>The horizontal scroll offset.</returns>
    inline static DUCKENGINE_API double GetScrollOffsetX() { return scrollX; }

    /// <summary>
    /// Returns the y offset of the mouse scroll wheel.
    /// </summary>
    /// <returns>The vertical scroll offset.</returns>
    inline static DUCKENGINE_API double GetScrollOffsetY() { return scrollY; }

    inline static DUCKENGINE_API double GetMouseDeltaX() { return mouseX - lastMouseX; }

    inline static DUCKENGINE_API double GetMouseDeltaY() { return mouseY - lastMouseY; }

    static DUCKENGINE_API bool IsMiddleMouseDown();
    static DUCKENGINE_API void ResetLastMousePosition();


    /// <summary>
    /// Handles keyboard input events. This function is called when a key is pressed, released, or repeated.
    /// </summary>
    /// <param name="pwin">Pointer to the GLFW window receiving the input event.</param>
    /// <param name="key">The key that was pressed or released.</param>
    /// <param name="scancode">The scancode of the key.</param>
    /// <param name="action">The type of action (press, release, or repeat).</param>
    /// <param name="mod">Modifier keys (e.g., Shift, Ctrl) held down at the time of the event.</param>
    static void keyCB(GLFWwindow* pwin, int key, int scancode, int action, int mod);

    /// <summary>
    /// Handles mouse button input events. This function is called when a mouse button is pressed or released.
    /// </summary>
    /// <param name="pwin">Pointer to the GLFW window receiving the input event.</param>
    /// <param name="button">The mouse button that was pressed or released.</param>
    /// <param name="action">The type of action (press or release).</param>
    /// <param name="mod">Modifier keys (e.g., Shift, Ctrl) held down at the time of the event.</param>
    static void mouseButtonCB(GLFWwindow* pwin, int button, int action, int mod);

    /// <summary>
    /// Handles mouse scroll events. This function is called when the mouse scroll wheel is used.
    /// </summary>
    /// <param name="pwin">Pointer to the GLFW window receiving the input event.</param>
    /// <param name="xoffset">The horizontal scroll offset.</param>
    /// <param name="yoffset">The vertical scroll offset.</param>
    static void mouseScrollCB(GLFWwindow* pwin, double xoffset, double yoffset);

    /// <summary>
    /// Handles mouse position events. This function is called when the mouse is moved within the window.
    /// </summary>
    /// <param name="pwin">Pointer to the GLFW window receiving the input event.</param>
    /// <param name="xpos">The new x-coordinate of the mouse in window space.</param>
    /// <param name="ypos">The new y-coordinate of the mouse in window space.</param>
    static void mousePosCB(GLFWwindow* pwin, double xpos, double ypos);

private:
    /// <summary>
    /// Stores the current state of keys, where true represents a key being pressed and false represents a key being released.
    /// </summary>
    static std::unordered_map<int, bool> keyStates;

    /// <summary>
    /// Stores the current state of mouse buttons, where true represents a button being pressed and false represents a button being released.
    /// </summary>
    static std::unordered_map<int, bool> mouseButtonStates;

    /// <summary>
    /// Stores the previous state of keys to detect key presses and releases between frames.
    /// </summary>
    static std::unordered_map<int, bool> previousKeyStates;

    /// <summary>
    /// Stores the previous state of mouse buttons to detect button presses and releases between frames.
    /// </summary>
    static std::unordered_map<int, bool> previousMouseButtonStates;

    /// <summary>
    /// The current x-coordinate of the mouse in window space.
    /// </summary>
    static double mouseX;

    /// <summary>
    /// The current y-coordinate of the mouse in window space.
    /// </summary>
    static double mouseY;

    /// <summary>
    /// The horizontal scroll offset.
    /// </summary>
    static double scrollX;

    /// <summary>
    /// The vertical scroll offset.
    /// </summary>
    static double scrollY;

    /// <summary>
    /// The previous x-coordinate of the mouse (used for calculating movement deltas if necessary).
    /// </summary>
    static double lastMouseX;

    /// <summary>
    /// The previous y-coordinate of the mouse (used for calculating movement deltas if necessary).
    /// </summary>
    static double lastMouseY;
};