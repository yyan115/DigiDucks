#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "UIDebugConsole.h"

class UIManager {
public:
    static void Initialize();
    static void Render();
    static void Shutdown();

    static UIDebugConsole debugConsole;
private:
    static void ShowInspector();
    static void ShowConsole();
};


