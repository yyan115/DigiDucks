/******************************************************************************/
/*!
\file       DuckEngine_Input.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Provides simplified access to input management functions,
            allowing users to check the state of keys and mouse buttons,
            and retrieve mouse positions and scroll offsets.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "InputManager.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/// <summary>
/// Provides a simplified interface to query input states for keys and mouse buttons.
/// Wraps functionality provided by the InputManager, offering inline functions for key and mouse state queries.
/// Also declares constants for commonly used keys.
/// </summary>
class DUCKENGINE_API DuckEngine_Input {
public:
    /// <summary>
    /// Checks if the specified key is currently held down.
    /// </summary>
    /// <param name="key">The key code to check.</param>
    /// <returns>Returns true if the key is held down.</returns>
    static inline bool IsKeyDown(int key) { return InputManager::IsKeyDown(key); }

    /// <summary>
    /// Checks if the specified key was pressed during the current frame.
    /// </summary>
    /// <param name="key">The key code to check.</param>
    /// <returns>Returns true if the key was pressed this frame.</returns>
    static inline bool IsKeyPressed(int key) { return InputManager::IsKeyPressed(key); }

    /// <summary>
    /// Checks if the specified key was released during the current frame.
    /// </summary>
    /// <param name="key">The key code to check.</param>
    /// <returns>Returns true if the key was released this frame.</returns>
    static inline bool IsKeyReleased(int key) { return InputManager::IsKeyReleased(key); }

    /// <summary>
    /// Checks if the specified mouse button is currently held down.
    /// </summary>
    /// <param name="button">The mouse button to check.</param>
    /// <returns>Returns true if the mouse button is held down.</returns>
    static inline bool IsMouseButtonDown(int button) { return InputManager::IsMouseButtonDown(button); }

    /// <summary>
    /// Checks if the specified mouse button was pressed during the current frame.
    /// </summary>
    /// <param name="button">The mouse button to check.</param>
    /// <returns>Returns true if the mouse button was pressed this frame.</returns>
    static inline bool IsMouseButtonPressed(int button) { return InputManager::IsMouseButtonPressed(button); }

    /// <summary>
    /// Checks if the specified mouse button was released during the current frame.
    /// </summary>
    /// <param name="button">The mouse button to check.</param>
    /// <returns>Returns true if the mouse button was released this frame.</returns>
    static inline bool IsMouseButtonReleased(int button) { return InputManager::IsMouseButtonReleased(button); }

    /// <summary>
    /// Returns the current x-coordinate of the mouse in window space.
    /// </summary>
    /// <returns>The mouse's x-coordinate.</returns>
    static inline double GetMouseX() { return InputManager::GetMouseX(); }

    /// <summary>
    /// Returns the current y-coordinate of the mouse in window space.
    /// </summary>
    /// <returns>The mouse's y-coordinate.</returns>
    static inline double GetMouseY() { return InputManager::GetMouseY(); }

    /// <summary>
    /// Returns the horizontal scroll offset of the mouse wheel.
    /// </summary>
    /// <returns>The horizontal scroll offset.</returns>
    static inline double GetScrollOffsetX() { return InputManager::GetScrollOffsetX(); }

    /// <summary>
    /// Returns the vertical scroll offset of the mouse wheel.
    /// </summary>
    /// <returns>The vertical scroll offset.</returns>
    static inline double GetScrollOffsetY() { return InputManager::GetScrollOffsetY(); }

    /// <summary>
    /// Checks if a gamepad is currently connected at the specified index.
    /// </summary>
    /// <param name="gamepadIndex">The index of the gamepad (0-15)</param>
    /// <returns>Returns true if the gamepad is connected.</returns>
    static inline bool IsGamepadConnected(int gamepadIndex) { return InputManager::IsGamepadConnected(gamepadIndex); }

    /// <summary>
    /// Checks if the specified gamepad button is currently held down.
    /// </summary>
    /// <param name="gamepadIndex">The index of the gamepad (0-15)</param>
    /// <param name="button">The gamepad button to check.</param>
    /// <returns>Returns true if the button is held down.</returns>
    static inline bool IsGamepadButtonDown(int gamepadIndex, int button) 
    {
        bool gamepadConnected = DuckEngine_Input::IsGamepadConnected(gamepadIndex);
        if (gamepadConnected)
        {
			return InputManager::IsGamepadButtonDown(gamepadIndex, button);
        }
        return false;
    }

    /// <summary>
    /// Checks if the specified gamepad button was pressed during the current frame.
    /// </summary>
    /// <param name="gamepadIndex">The index of the gamepad (0-15)</param>
    /// <param name="button">The gamepad button to check.</param>
    /// <returns>Returns true if the button was pressed this frame.</returns>
    static inline bool IsGamepadButtonPressed(int gamepadIndex, int button) { return InputManager::IsGamepadButtonPressed(gamepadIndex, button); }

    /// <summary>
    /// Checks if the specified gamepad button was released during the current frame.
    /// </summary>
    /// <param name="gamepadIndex">The index of the gamepad (0-15)</param>
    /// <param name="button">The gamepad button to check.</param>
    /// <returns>Returns true if the button was released this frame.</returns>
    static inline bool IsGamepadButtonReleased(int gamepadIndex, int button) { return InputManager::IsGamepadButtonReleased(gamepadIndex, button); }

    /// <summary>
    /// Gets the current value of the specified gamepad axis.
    /// </summary>
    /// <param name="gamepadIndex">The index of the gamepad (0-15)</param>
    /// <param name="axis">The axis to query</param>
    /// <returns>The axis value between -1.0 and 1.0</returns>
    static inline float GetGamepadAxisValue(int gamepadIndex, int axis) { return InputManager::GetGamepadAxisValue(gamepadIndex, axis); }

    // Constants for key codes (A-Z)
    static const int KEY_A;
    static const int KEY_B;
    static const int KEY_C;
    static const int KEY_D;
    static const int KEY_E;
    static const int KEY_F;
    static const int KEY_G;
    static const int KEY_H;
    static const int KEY_I;
    static const int KEY_J;
    static const int KEY_K;
    static const int KEY_L;
    static const int KEY_M;
    static const int KEY_N;
    static const int KEY_O;
    static const int KEY_P;
    static const int KEY_Q;
    static const int KEY_R;
    static const int KEY_S;
    static const int KEY_T;
    static const int KEY_U;
    static const int KEY_V;
    static const int KEY_W;
    static const int KEY_X;
    static const int KEY_Y;
    static const int KEY_Z;

    // Constants for key codes (0-9)
    static const int KEY_0;
    static const int KEY_1;
    static const int KEY_2;
    static const int KEY_3;
    static const int KEY_4;
    static const int KEY_5;
    static const int KEY_6;
    static const int KEY_7;
    static const int KEY_8;
    static const int KEY_9;

    // Constants for common key codes
    static const int KEY_SPACE;
    static const int KEY_ENTER;
    static const int KEY_ESCAPE;
    static const int KEY_LEFT;
    static const int KEY_RIGHT;
    static const int KEY_UP;
    static const int KEY_DOWN;
    static const int KEY_LEFT_SHIFT;
    static const int KEY_RIGHT_SHIFT;
    static const int KEY_BACKSPACE;
    static const int KEY_DEL;
    static const int KEY_CTRL;
    static const int KEY_LEFT_ALT;
    static const int KEY_RIGHT_ALT;


    // Constants for mouse buttons
    static const int MOUSE_BUTTON_LEFT;
    static const int MOUSE_BUTTON_RIGHT;
    static const int MOUSE_BUTTON_MIDDLE;

    // Constants for scroll directions
    static const double SCROLL_UP;
    static const double SCROLL_DOWN;

    // Constants for gamepad buttons (Xbox controller layout)
    static const int GAMEPAD_BUTTON_A;           // A button (Xbox) / Cross button (PlayStation)
    static const int GAMEPAD_BUTTON_B;           // B button (Xbox) / Circle button (PlayStation)
    static const int GAMEPAD_BUTTON_X;           // X button (Xbox) / Square button (PlayStation)
    static const int GAMEPAD_BUTTON_Y;           // Y button (Xbox) / Triangle button (PlayStation)
    static const int GAMEPAD_BUTTON_LEFT_BUMPER; // Left bumper (LB)
    static const int GAMEPAD_BUTTON_RIGHT_BUMPER;// Right bumper (RB)
    static const int GAMEPAD_BUTTON_BACK;        // Back button
    static const int GAMEPAD_BUTTON_START;       // Start button
    static const int GAMEPAD_BUTTON_GUIDE;       // Guide button (Xbox button)
    static const int GAMEPAD_BUTTON_LEFT_THUMB;  // Left thumbstick press
    static const int GAMEPAD_BUTTON_RIGHT_THUMB; // Right thumbstick press
    static const int GAMEPAD_BUTTON_DPAD_UP;     // D-pad up
    static const int GAMEPAD_BUTTON_DPAD_RIGHT;  // D-pad right
    static const int GAMEPAD_BUTTON_DPAD_DOWN;   // D-pad down
    static const int GAMEPAD_BUTTON_DPAD_LEFT;   // D-pad left

    // Constants for gamepad axes
    static const int GAMEPAD_AXIS_LEFT_X;        // Left stick X axis
    static const int GAMEPAD_AXIS_LEFT_Y;        // Left stick Y axis
    static const int GAMEPAD_AXIS_RIGHT_X;       // Right stick X axis
    static const int GAMEPAD_AXIS_RIGHT_Y;       // Right stick Y axis
    static const int GAMEPAD_AXIS_LEFT_TRIGGER;  // Left trigger
    static const int GAMEPAD_AXIS_RIGHT_TRIGGER; // Right trigger

    // Constants for gamepad indices
    static const int GAMEPAD_1;                  // First gamepad
    static const int GAMEPAD_2;                  // Second gamepad
    static const int GAMEPAD_3;                  // Third gamepad
    static const int GAMEPAD_4;                  // Fourth gamepad
};
