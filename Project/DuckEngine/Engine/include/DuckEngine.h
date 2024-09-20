#ifndef DUCKENGINE_H
#define DUCKENGINE_H

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "InputManager.h"

class DUCKENGINE_API DuckEngine
{
public:
    static ComponentManager DUCKENGINE_ComponentManager;
    static EntityManager DUCKENGINE_EntityManager;
    static SystemManager DUCKENGINE_SystemManager;

    void Initialize();
    void Update();
    void StartDraw();
    void Draw();
    void EndDraw();
    void Exit();

    bool Running();

    static bool WKeyDown;
    static bool AKeyDown;
    static bool SKeyDown;
    static bool DKeyDown;

    static int GetWindowWidth();
    static int GetWindowHeight();
    static float DeltaTime();

    static void SetBackgroundColor(const float r, const float g, const float b, const float a);

    static inline bool IsKeyPressed(int key) { return InputManager::IsKeyPressed(key); }
    static inline bool IsKeyReleased(int key) { return InputManager::IsKeyReleased(key); }
    static inline bool IsMouseButtonPressed(int button) { return InputManager::IsMouseButtonPressed(button); }
    static inline bool IsMouseButtonReleased(int button) { return InputManager::IsMouseButtonReleased(button); }
    static inline double GetMouseX() { return InputManager::GetMouseX(); }
    static inline double GetMouseY() { return InputManager::GetMouseY(); }
    static inline double GetScrollOffsetX() { return InputManager::GetScrollOffsetX(); }
    static inline double GetScrollOffsetY() { return InputManager::GetScrollOffsetY(); }

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

#endif // DUCKENGINE_H
