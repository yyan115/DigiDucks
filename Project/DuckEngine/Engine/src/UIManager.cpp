#include "UIManager.h"
#include "WindowManager.h"

UIDebugConsole UIManager::debugConsole;

void UIManager::Initialize() {
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Initialize platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(WindowManager::getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::Render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // example window
    //ImGui::ShowDemoWindow();

    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Rendering stats
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate); // Show FPS
    ImGui::End();

    ImGui::Begin("Memory Usage", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::End();

    
    ImGui::Begin("Game Objects", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::End();
    
    ImGui::Begin("Entity Spawn", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    static int spawn[1] = {};
    ImGui::SliderInt("counter", spawn, 0, 300);
    ImGui::End();

    ShowInspector();
    ShowConsole();

    

    // Render ImGui on top of the scene
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Shutdown() {
    // Clean up ImGui resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::ShowConsole() {
    bool consoleOpen = true;
    debugConsole.Render(&consoleOpen);
}

void UIManager::ShowInspector() {
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Simulate a selected object placeholder still
    static float position[3] = { 0.0f, 0.0f, 0.0f };
    static float scale[3] = { 1.0f, 1.0f, 1.0f };
    static float rotation[3] = { 0.0f, 0.0f, 0.0f };

    // Display sliders for position, scale, and rotation
    ImGui::Text("Transform");

    ImGui::SliderFloat3("Position", position, -10.0f, 10.0f);
    ImGui::SliderFloat3("Rotation", rotation, -180.0f, 180.0f);
    ImGui::SliderFloat3("Scale", scale, 0.1f, 10.0f);

    // Buttons for actions
    if (ImGui::Button("Reset Position")) {
        position[0] = position[1] = position[2] = 0.0f;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Rotation")) {
        rotation[0] = rotation[1] = rotation[2] = 0.0f;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Scale")) {
        scale[0] = scale[1] = scale[2] = 1.0f;
    }

    ImGui::End();
}