#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

class UIManager {
public:
    static void Initialize();
    static void Render();
    static void Shutdown();

};
