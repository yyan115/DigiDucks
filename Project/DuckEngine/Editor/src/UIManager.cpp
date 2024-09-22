//---------------------------------------------------------
// File:    UIManager.cpp
//authors:	Muhammad Zikry Bin Zakaria
// email:	muhammadzikry.b\@digipen.edu
// 
//
// Brief:     Contains the definition that is used to manage the UI in the editor
// 
// Copyright © 2024 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "DuckEngine.h"
#include "UIManager.h"
#include "UIDebugConsole.h"
#include "WindowManager.h"
#include "SystemManager.h"
#include "TimeManager.h"
#include "ImageLoader.h"
#include <Windows.h>
#include "Color.h"
#include <random>
#include <map>

enum class WindowType {
    DebugInfo,
    Performance,
    Count
};


std::unordered_map<WindowType, bool> windowStates = {
    {WindowType::DebugInfo, false},
    {WindowType::Performance, false},
};

enum class AssetCategory {
    Scene,
    GameObject,
    Texture
};

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
            if (ImGui::MenuItem("Show Debug Info", NULL, windowStates[WindowType::DebugInfo])) {
                windowStates[WindowType::DebugInfo] = !windowStates[WindowType::DebugInfo];
            }
            if (ImGui::MenuItem("Show Performance", NULL, windowStates[WindowType::Performance])) {
                windowStates[WindowType::Performance] = !windowStates[WindowType::Performance];
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Rendering stats
    RenderWindows();

    ShowInspector();
    ShowExplorer();
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

void UIManager::ShowDebugInfo() {
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Create the tab bar
    if (ImGui::BeginTabBar("MyTabBar")) {

        // First tab: Debugger
        if (ImGui::BeginTabItem("Debugger")) {
            // Display the frame stats
            ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);
            ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

            // OpenGL Version Information
            const GLubyte* renderer = glGetString(GL_RENDERER); // Get GPU vendor
            const GLubyte* version = glGetString(GL_VERSION);   // Get OpenGL version
            ImGui::Text("GPU Renderer: %s", renderer);
            ImGui::Text("OpenGL Version: %s", version);

            // CPU Information
            ImGui::Text("\n\nCPU Information");
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            ImGui::Text("Page Size: %u", sysInfo.wProcessorArchitecture);
            ImGui::Text("Number of Cores: %u", sysInfo.dwNumberOfProcessors);
            ImGui::Text("Processor Type: %u", sysInfo.dwProcessorType);

            // Memory Information
            ImGui::Text("\n\nMemory Information");
            MEMORYSTATUSEX memoryStatus;
            memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
            if (GlobalMemoryStatusEx(&memoryStatus)) {
                ImGui::Text("Memory Status: %u MB", memoryStatus.dwMemoryLoad / (1024 * 1024));
                ImGui::Text("Total Physical Memory: %u MB", memoryStatus.ullTotalPhys / (1024 * 1024));
                ImGui::Text("Free Physical Memory: %u MB", memoryStatus.ullAvailPhys / (1024 * 1024));
                ImGui::Text("Total Virtual Memory: %u MB", memoryStatus.ullTotalVirtual / (1024 * 1024));
                ImGui::Text("Free Virtual Memory: %u MB", memoryStatus.ullAvailVirtual / (1024 * 1024));
            }
            else {
                std::cerr << "Failed to retrieve memory information." << std::endl;
            }
            ImGui::EndTabItem();
        }

        // Second tab: Game Info
        if (ImGui::BeginTabItem("Game Info")) {
            ImGui::Text("Total Entities: %d", DuckEngine::DUCKENGINE_EntityManager.GetEntities().size());
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
	ImGui::End();
}

void UIManager::ShowPerformance() {
    ImGui::Begin("Performance", nullptr);
    RenderSystemTimings(DuckEngine::DUCKENGINE_SystemManager);
    ImGui::End();
}   

void UIManager::ShowExplorer() {
    bool consoleOpen = true;
    ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    // Create the tab bar
    if (ImGui::BeginTabBar("MyTabBar")) {

        // First tab: Console
        if (ImGui::BeginTabItem("Console")) {
            UIDebugConsole::debugConsole.Render(&consoleOpen);
            ImGui::EndTabItem();
        }

        // Second tab: Assets
        if (ImGui::BeginTabItem("Assets")) {

            static AssetCategory currentCategory = AssetCategory::Scene;
            const char* items[] = { "Scene", "GameObject", "Texture" };

            ImGui::Text("Category:   ");
            ImGui::SameLine();

            if (ImGui::BeginCombo("##Category", items[static_cast<int>(currentCategory)])) {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                    bool is_selected = (static_cast<int>(currentCategory) == n);
                    if (ImGui::Selectable(items[n], is_selected)) {
                        currentCategory = static_cast<AssetCategory>(n);
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
				ImGui::EndCombo();
			}

            // Handle the selected category
            switch (currentCategory) {
            case AssetCategory::Scene:
                RenderSceneAssets();
                break;
            case AssetCategory::GameObject:
                RenderGameObjectAssets();
                break;
            case AssetCategory::Texture:
                RenderTextureAssets();
                break;
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void UIManager::ShowInspector() {
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Get first box entity
    int entityID = DuckEngine::DUCKENGINE_EntityManager.GetEntities().front().EntityID;

    // Access TransformComponent
    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID);

    if (transform) {
        // Display sliders for position, scale, and rotation (angle)
        ImGui::Text("Transform");

        // Position
        ImGui::SliderFloat2("Position", &transform->position.x, -10.0f, 10.0f);

        // Rotation
        ImGui::SliderFloat("Rotation", &transform->angle, -180.0f, 180.0f);

        // Scale
        ImGui::SliderFloat2("Scale", &transform->scale.x, 0.1f, 10.0f);

        // Buttons for reset actions
        if (ImGui::Button("Reset Position")) {
            transform->position = Vec2(0.0f, 0.0f);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Rotation")) {
            transform->angle = 0.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Scale")) {
            transform->scale = Vec2(1.0f, 1.0f);
        }
    }
    else {
        ImGui::Text("No TransformComponent found for this entity.");
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
        UIDebugConsole::debugConsole.AddDebugLog("Spawning entities");
        int entitiesToSpawn = spawnCount - lastSpawnCount;
        for (int i = 0; i < entitiesToSpawn; i++) {
            DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("../Resources/oldman.png", { 5.0f, 0.0f }, { 2.0f, 2.0f });
        }
    }

    // If the slider value has decreased, remove entities
    if (spawnCount < lastSpawnCount) {
        UIDebugConsole::debugConsole.AddDebugLog("Removing entities");
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

void UIManager::RenderWindows() {
    for (const auto& [window, isVisible] : windowStates) {
        if (isVisible) {
            switch (window) {
            case WindowType::DebugInfo:
                ShowDebugInfo();
                break;
            case WindowType::Performance:
                ShowPerformance();
                break;
            default:
                break;
            }
        }
    }
}

void UIManager::RenderSceneAssets() {
    if (ImGui::Button("Max Load Scene")) {
        // Access SceneManager from DuckEngine and switch scene
        DuckEngine::DUCKENGINE_SceneManager.SetActiveScene("MaxLoadScene"); // Switch to MaxLoadScene
    }
    ImGui::SameLine();
    if (ImGui::Button("Main Scene")) {
        // Access SceneManager from DuckEngine and switch scene
        DuckEngine::DUCKENGINE_SceneManager.SetActiveScene("SpriteMovementScene"); // Switch to MainScene
    }
}

void UIManager::RenderGameObjectAssets() {
	// Placeholder for GameObject assets
	ImGui::Text("No GameObjects to display.");
}

void UIManager::RenderTextureAssets() {
	// Placeholder for Texture assets
	ImGui::Text("No Textures to display.");
}





// Random number generator for position, scale, rotation, and velocity
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> randomPosition(-500.0f, 500.0f);  // Position between -500 and 500
std::uniform_real_distribution<float> randomScale(50.0f, 500.0f);      // Scale between 50 and 500
std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);    // Rotation between 0 and 360 degrees
std::uniform_real_distribution<float> randomVelocity(-1.0f, 1.0f);  // Velocity between -100 and 100


// Map to store entity velocities (entityID -> (velocityX, velocityY))
std::map<int, std::pair<float, float>> entityVelocities;

void UIManager::SpawnSquare() {
    // Generate random position, scale, rotation, and velocity
    Vec2 pos = Vec2(randomPosition(gen), randomPosition(gen));
    float scaleX = randomScale(gen);
    float scaleY = randomScale(gen);
    float rotation = randomRotation(gen);
    float velocityX = randomVelocity(gen);
    float velocityY = randomVelocity(gen);

    // Create a new square entity
    Entity& square = DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

    // Add transform component with randomized values
    DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square.EntityID);

    // Store the velocity in the map
    entityVelocities[square.EntityID] = std::make_pair(velocityX, velocityY);

    UIDebugConsole::debugConsole.AddDebugLog("Spawned square at position (%.2f", pos.x);

    // Add sprite renderer component
    SpriteRendererComponent* sr = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square.EntityID, false);
    sr->texture = ImageLoader::LoadTexture("../Resources/oldman.png");


    // Print for debugging
    //std::cout << "Spawned square at position (" << pos.x << ", " << pos.y << "), scale (" << scaleX << ", " << scaleY
    //    << "), rotation " << rotation << ", velocity (" << velocityX << ", " << velocityY << ")" << std::endl;
}
