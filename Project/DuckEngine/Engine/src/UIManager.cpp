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
    ImGui::ShowDemoWindow();

    bool consoleOpen = true;
    debugConsole.Render(&consoleOpen);

    // Rendering stats
    ImGui::Begin("Rendering Stats", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate); // Show FPS
    ImGui::End();

    ImGui::Begin("Memory Usage", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::End();


    ImGui::Begin("Game Objects", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::End();

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
