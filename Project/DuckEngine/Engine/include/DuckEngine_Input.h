#pragma once

#include "InputManager.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif


class DUCKENGINE_API DuckEngine_Input
{
public:
    static inline bool IsKeyDown(int key) { return InputManager::IsKeyDown(key); }
    static inline bool IsKeyPressed(int key) { return InputManager::IsKeyPressed(key); }
    static inline bool IsKeyReleased(int key) { return InputManager::IsKeyReleased(key); }

    static inline bool IsMouseButtonDown(int button) { return InputManager::IsMouseButtonDown(button); }
    static inline bool IsMouseButtonPressed(int button) { return InputManager::IsMouseButtonPressed(button); }
    static inline bool IsMouseButtonReleased(int button) { return InputManager::IsMouseButtonReleased(button); }

    static inline double GetMouseX() { return InputManager::GetMouseX(); }
    static inline double GetMouseY() { return InputManager::GetMouseY(); }

    static inline double GetScrollOffsetX() { return InputManager::GetScrollOffsetX(); }
    static inline double GetScrollOffsetY() { return InputManager::GetScrollOffsetY(); }

    static bool WKeyDown;
    static bool AKeyDown;
    static bool SKeyDown;
    static bool DKeyDown;

    // Declare key constants for letters (A-Z)
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

    // Declare key constants for numbers (0-9)
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

    // Declare key constants for common keys
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
};