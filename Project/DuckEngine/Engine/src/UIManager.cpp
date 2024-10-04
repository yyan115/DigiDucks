/******************************************************************************/
/*!
\file     UIManager.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the UIManager class

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "DuckEngine.h"
#include "DuckEngine_Sound.h"
#include "UIManager.h"
#include "UIDebugConsole.h"
#include "WindowManager.h"
#include "SystemManager.h"
#include "TimeManager.h"
#include "ImageLoader.h"
#include "LevelManager.h"
#include "File.h"
#include "Bounding.h"
#include <Windows.h>
#include "Color.h"
#include <random>
#include <map>
#include <set>
#include <sstream>

// GLOBALS For Spawning of Entities
int selectedEntityID = -1;
std::vector<std::pair<int, std::string>> spawnedEntities;
int entityCounter = 0;
std::set<int> availableNumbers;

enum class WindowType {
    DebugInfo,
    Performance,
    Inspector,
    Count
};


std::unordered_map<WindowType, bool> windowStates = {
    {WindowType::DebugInfo, false},
    {WindowType::Performance, false},
    {WindowType::Inspector, false},
};

enum class AssetCategory {
    Scene,
    GameObject,
    Audio
};

void UIManager::Initialize() {
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    //Initialize platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(WindowManager::getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void RenderSystemTimings(const SystemManager& systemManager) {
    static std::vector<std::pair<std::string, double>> lastSystemPercentages;
    static std::vector<std::pair<std::string, double>> lastManagerPercentages;
    static double lastUpdateTime = 0.0;

    // Current time in seconds
    double currentTime = glfwGetTime();

    // Check if 2 seconds have passed
    if (currentTime - lastUpdateTime >= 2.0) {
        lastUpdateTime = currentTime;

        // Clear previous percentages
        lastSystemPercentages.clear();
        lastManagerPercentages.clear();

        const std::vector<std::pair<std::string, double>>& systemData = systemManager.GetSystemData();
        const std::vector<std::pair<std::string, double>>& managerData = TimeManager::GetManagerData();

        // Store manager percentages
        for (const auto& manager : managerData) {
            double managerPercentage = (manager.second / TimeManager::DT()) * 100.0;
            lastManagerPercentages.push_back({ manager.first, managerPercentage });
        }

        // Store system percentages
        for (const auto& system : systemData) {
            double systemPercentage = (system.second / TimeManager::DT()) * 100.0;
            lastSystemPercentages.push_back({ system.first, systemPercentage });
        } 
    }

    // Display manager timings
    for (const auto& manager : lastManagerPercentages) {
        double managerPercentage = manager.second;

        ImGui::Text("%s", manager.first.c_str());
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << managerPercentage << "%";
        ImGui::ProgressBar(static_cast<float>(managerPercentage / 100.0), ImVec2(-1, 0), ss.str().c_str());

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Manager: %s", manager.first.c_str());
            ImGui::Text("Percentage: %.2f%%", managerPercentage);
            ImGui::EndTooltip();
        }

        ImGui::Spacing();
    }

    // Display system timings
    for (const auto& system : lastSystemPercentages) {
        double systemPercentage = system.second;

        auto spacePos = system.first.find(" ");
        std::string rawName = system.first.c_str();
        rawName = system.first.c_str() + spacePos + 1;
        ImGui::Text("%s", rawName.c_str());
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << systemPercentage << "%";
        ImGui::ProgressBar(static_cast<float>(systemPercentage / 100.0), ImVec2(-1, 0), ss.str().c_str());

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("System: %s", rawName.c_str());
            ImGui::Text("Percentage: %.2f%%", systemPercentage);
            ImGui::EndTooltip();
        }

        ImGui::Spacing();
    }
}


// Render the ImGui windows with a specific size and position to make it adaptive
void UIManager::RenderImGuiWindows(float WidthOffset, float HeightOffset, float PosX, float PosY) {
    // Get the current window size
    int windowWidth = WindowManager::GetWindowWidth();
    int windowHeight = WindowManager::GetWindowHeight();

    // Set the ImGui window to automatically adapt to the window size
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(windowWidth * WidthOffset), static_cast<float>(windowHeight * HeightOffset))); // Set size relative to window size
    ImGui::SetNextWindowPos(ImVec2(windowWidth * PosX, windowHeight * PosY)); // Set position
}

void UIManager::Render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // example window
    //ImGui::ShowDemoWindow();    

    RenderImGuiWindows(0.25f, 0.25f, 0.0f, 20.0f);
    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) { FilePath::PrintPath(); }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { LevelManager::OpenLevelDialog(); }
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

    RenderImGuiWindows(1.f, 0.25f, 0.0f, 0.75f);
    ShowExplorer();

    RenderImGuiWindows(0.2f, 0.35f, 0.0f, 0.25f);
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
    RenderImGuiWindows(0.2f, 0.3f, 0.8f, 0.0f);
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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
            MEMORYSTATUSEX memoryStatus = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
            if (GlobalMemoryStatusEx(&memoryStatus)) {
                ImGui::Text("Total Physical Memory: %u MB", memoryStatus.ullTotalPhys / static_cast<size_t>(1024 * 1024));
                ImGui::Text("Free Physical Memory: %u MB", memoryStatus.ullAvailPhys / static_cast<size_t>(1024 * 1024));
                ImGui::Text("Total Virtual Memory: %u MB", memoryStatus.ullTotalVirtual / static_cast<size_t>(1024 * 1024));
                ImGui::Text("Free Virtual Memory: %u MB", memoryStatus.ullAvailVirtual / static_cast<size_t>(1024 * 1024));
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
    RenderImGuiWindows(0.2f, 0.3f, 0.8f, 0.3f);
    ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
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
            const char* items[] = { "Scene", "GameObject", "Audio" };

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
            case AssetCategory::Audio:
                RenderAudioAssets();
                break;
            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void UIManager::ShowInspector() {
    if (windowStates[WindowType::Inspector] && selectedEntityID != -1) {
        RenderImGuiWindows(0.21f, 0.16f, 0.0f, 0.6f);
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // Access TransformComponent
        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
        BoundingBox* boxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(selectedEntityID);
       
        if (transform) {
            // Display sliders for position, scale, and rotation (angle)
            ImGui::Text("Transform");

            // Position
            ImGui::SliderFloat2("Position", &transform->position.x, -10.0f, 10.0f);

            // Rotation
            ImGui::SliderFloat("Rotation", &transform->angle, -180.0f, 180.0f);

            // Scale
            ImGui::SliderFloat2("Scale", &transform->scale.x, 0.1f, 10.0f);

            if (boxCollider != nullptr)
            {
                boxCollider->setCenter(transform->position);
                boxCollider->setRotation(transform->angle);
                boxCollider->setSize(transform->scale/2);
            }
            

            

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
}


void UIManager::ShowEntitySpawn() {
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Get all entities
    std::vector<Entity> entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    // Iterate through entities and create buttons for each one
    for (size_t i = 0; i < entities.size(); ++i) {
        if (entities[i].entityID == 0) {
            continue;
        }

        // Create a unique label for each button
        std::string entityLabel = "GameObject " + std::to_string(entities[i].entityID);
        if (!entities[i].name.empty()) {
            entityLabel = entities[i].name;
        }

        // Check if this entity is selected
        if (selectedEntityID == entities[i].entityID) {
            // Highlight the selected entity button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
        else {
            // For unselected entities
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));
        }

        // Create a button for each entity
        if (ImGui::Button(entityLabel.c_str())) {
            if (selectedEntityID == entities[i].entityID) {
                // Deselect the entity if it is clicked again
                selectedEntityID = -1;
                windowStates[WindowType::Inspector] = false; // Hide Inspector window
            }
            else {
                // Select the entity
                selectedEntityID = entities[i].entityID;
                windowStates[WindowType::Inspector] = true;  // Show Inspector window
            }
        }

        ImGui::PopStyleColor(3);
        ImGui::Spacing();
    }

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
            case WindowType::Inspector:
				ShowInspector();
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
    if (ImGui::Button("Spawn Crate")) {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> randomPosition(0.0f, 5.0f);


        UIDebugConsole::debugConsole.AddDebugLog("entitiesSpawn");
        Vec2 pos = Vec2(randomPosition(gen), randomPosition(gen));

        int newEntityNumber;
        if (!availableNumbers.empty()) {
            newEntityNumber = *availableNumbers.begin();  // Get the smallest available number
            availableNumbers.erase(availableNumbers.begin());
        }
        else {
            newEntityNumber = ++entityCounter;
        }

        // Create a new square entity
        Entity* square = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("../Resources/Crate.png", pos, {2.0f, 2.0f});
        Vec2 center = Vec2(0.0f, 0.0f);
        Vec2 size = Vec2(1.0f, 1.0f);

        // Create and add the BoundingBox
        DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(square->entityID, center, size);

        // Create and add the RigidbodyComponent
        auto rbComponent = std::make_shared<RigidbodyComponent>();
        rbComponent->isStatic = false;
        DuckEngine::DUCKENGINE_ComponentManager.AddComponent<RigidbodyComponent>(square->entityID, *rbComponent);

        // Add the entity to the vector
        std::string entityName = "GameObject " + std::to_string(newEntityNumber);
        spawnedEntities.emplace_back(square->entityID, entityName);

        
	}

    // Loop through the spawned entities and render buttons for each
    for (size_t i = 0; i < spawnedEntities.size(); ++i) {
        // Display button for the entity
        if (ImGui::Button(spawnedEntities[i].second.c_str())) {
            selectedEntityID = spawnedEntities[i].first;
            windowStates[WindowType::Inspector] = !windowStates[WindowType::Inspector];
        }

        ImGui::SameLine();

        // Button to remove the entity
        std::string removeButtonLabel = "Remove " + spawnedEntities[i].second;
        if (ImGui::Button(removeButtonLabel.c_str())) {
            // Remove the entity from the entity manager
            DuckEngine::DUCKENGINE_EntityManager.RemoveEntity(spawnedEntities[i].first);

            // Recycle the removed number
            std::string entityLabel = spawnedEntities[i].second;
            int removedNumber = std::stoi(entityLabel.substr(entityLabel.find(" ") + 1));
            availableNumbers.insert(removedNumber);

            // Remove the entity from the list
            spawnedEntities.erase(spawnedEntities.begin() + i);
            --i;
        }
    }
}

void UIManager::RenderAudioAssets() {
	// Placeholder for Audio assets
    if (ImGui::Button("Play Sound")) {
		DuckEngine_Sound::PlaySounds("TestSound");
	}

    ImGui::SameLine();
    if (ImGui::Button("Stop Sound")) {
		DuckEngine_Sound::StopSound();
	}

    if (ImGui::Button("Play Sound 2")) {
        DuckEngine_Sound::PlaySounds("TestSound2");
    }
}