#pragma once
#include "System.h"
#include "GraphicsManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class ImGuiSystem : public System {
public:
    ImGuiSystem(GraphicsManager* graphicsManager);
    ~ImGuiSystem();

    void Initialize();
    void Update() override;
    void Shutdown();

private:
    GraphicsManager* graphicsManager;
};
