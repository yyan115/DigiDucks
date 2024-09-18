#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "DuckEngine.h"
#include "UIManager.h"
#include "UIDebugConsole.h"
#include "WindowManager.h"
#include "SystemManager.h"
#include "TimeManager.h"


void UIManager::Initialize() {
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    //Initialize platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(WindowManager::getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void RenderSystemTimings(const SystemManager& systemManager) {

    const std::vector<std::pair<std::string, double>>& systemData = systemManager.GetSystemData();
    double totalTime = systemManager.GetTotalTime();

    if (totalTime > 0.0) {
        std::vector<float> systemPercentages;
        std::vector<const char*> systemNames;

        for (const auto& system : systemData) {
            double percentage = (system.second / totalTime) * 100.0;
            systemPercentages.push_back(static_cast<float>(percentage));
            systemNames.push_back(system.first.c_str());
        }

        // Start drawing the histogram
        ImGui::PlotHistogram("##Systems", systemPercentages.data(), static_cast<int>(systemPercentages.size()), 0, "System Graphs", 0.0f, 100.0f, ImVec2(-25, 150));

            for (size_t i = 0; i < systemPercentages.size(); ++i) {
                // If the bar is hovered, show a tooltip with the system name
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("System: %s", systemNames[i]);
                    ImGui::Text("Percentage: %.2f%%", systemPercentages[i]);
                    ImGui::EndTooltip();
                }
            }
        
    }
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
        if (ImGui::BeginMenu("Debug")) {
            if (ImGui::MenuItem("Show Debug Info")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Rendering stats
    ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate); // Show FPS
    ImGui::End();

    ImGui::Begin("Memory Usage", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    RenderSystemTimings(DuckEngine::DUCKENGINE_SystemManager);
    ImGui::End();

    
    ImGui::Begin("Game Objects", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("Total Entities: %d", DuckEngine::DUCKENGINE_EntityManager.GetEntities().size());
    ImGui::End();
      
    ShowInspector();
    ShowConsole();
    ShowEntitySpawn();

    // Render ImGui on top of the scene
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Exit() {
    // Clean up ImGui resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::ShowConsole() {
    bool consoleOpen = true;
    UIDebugConsole::debugConsole.Render(&consoleOpen);
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

void UIManager::ShowEntitySpawn() {
    static int lastSpawnCount = 0;  // Keep track of the last spawn count

    ImGui::Begin("Entity Spawn", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    static int spawnCount = 0;  // Slider value
    ImGui::SliderInt("Entity Count", &spawnCount, 0, 300);  // Control entity count

    // If the slider value has increased, spawn new entities
    if (spawnCount > lastSpawnCount) {
        UIDebugConsole::debugConsole.AddLog("Spawning entities");
        int entitiesToSpawn = spawnCount - lastSpawnCount;
        for (int i = 0; i < entitiesToSpawn; i++) {
            //SpawnSquare(engine);  // Spawn square using the SpawnSquare
        }
    }

    // If the slider value has decreased, remove entities
    if (spawnCount < lastSpawnCount) {
        UIDebugConsole::debugConsole.AddLog("Removing entities");
        int entitiesToRemove = lastSpawnCount - spawnCount;
        for (int i = 0; i < entitiesToRemove; i++) {
            // Remove the last spawned entity
            if (!DuckEngine::DUCKENGINE_EntityManager.GetEntities().empty()) {
                DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(DuckEngine::DUCKENGINE_EntityManager.GetEntities().back().EntityID);
            }
        }
    }

    lastSpawnCount = spawnCount;  // Update the last spawn count

    ImGui::End();
}

