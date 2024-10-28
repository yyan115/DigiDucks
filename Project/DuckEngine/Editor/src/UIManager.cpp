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
#include "GraphicsManager.h"
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
#include "Inspector.h"
#include "GameManager.h"
#include "AssetsBrowser.h"
#include "EditorTheme.h"


// GLOBALS For Spawning of Entities
int selectedEntityID = -1;
std::vector<std::pair<int, std::string>> spawnedEntities;
int entityCounter = 0;
std::set<int> availableNumbers;

// Store historical data for performance tracking
std::unordered_map<std::string, std::vector<float>> managerHistory;
std::unordered_map<std::string, std::vector<float>> systemHistory;

enum class WindowType {
    DebugInfo,
    Inspector,
    Count
};


std::unordered_map<WindowType, bool> windowStates = {
    {WindowType::DebugInfo, false},
    {WindowType::Inspector, false},
};



void UIManager::Initialize() {
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
    
    // dark theme
    //ImGui::StyleColorsDark();
    // duck theme
    EditorTheme::SetDuckTheme();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    GLFWwindow* window = static_cast<GLFWwindow*>(WindowManager::getWindow());

    //Initialize platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
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

void UIManager::StartRender()
{
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::Render() {

    // example window
    //ImGui::ShowDemoWindow();    
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Show the main menu bar
    ShowMenuBar();

    // Create the dockspace
    CreateDockSpace();

    // Rendering debug stats
    RenderWindows();

    // Show the different windows
    ShowExplorer();
    ShowHierarchy();
}

void UIManager::EndRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }  
}

void UIManager::Exit() {
    // Clean up ImGui resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::ShowMenuBar()
{
    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) { FilePath::PrintPath(); }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { LevelManager::OpenLevelDialog(); }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) 
            {
                LevelManager::SaveSceneChanges(GameManager::ActiveSceneName);
            }
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
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void UIManager::CreateDockSpace()
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void UIManager::ShowDebugInfo() {
    //RenderImGuiWindows(0.2f, 0.3f, 0.8f, 0.0f);
	ImGui::Begin("Debug Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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

        // Second tab: Perforamnce
        if (ImGui::BeginTabItem("Performance")) {
            RenderPerformanceGraphs(DuckEngine::DUCKENGINE_SystemManager);
            ImGui::EndTabItem();
        }

        // third tab: Game Info
        if (ImGui::BeginTabItem("Game Info")) {
            ImGui::Text("Total Entities: %d", DuckEngine::DUCKENGINE_EntityManager.GetEntities().size());
            ImGui::EndTabItem();
        }

        

        ImGui::EndTabBar();
    }
	ImGui::End();
}

void UIManager::RenderPerformanceGraphs(const SystemManager& systemManager) {
    const auto& systemData = systemManager.GetSystemData();
    const auto& managerData = TimeManager::GetManagerData();
    float deltaTime = static_cast<float>(TimeManager::DT());  // Get the current frame time (or delta time)

    const int averageFrameCount = 10;

    // Display manager performance as graphs
    ImGui::Text("Manager Performance");
    for (const auto& manager : managerData) {
        const std::string& managerName = manager.first;
        float managerPercentage = static_cast<float>(manager.second / deltaTime) * 100.0f;

        auto& history = managerHistory[managerName];
        if (history.size() >= 100) {
            history.erase(history.begin());
        }
        history.push_back(managerPercentage);

        // Display the graph
        ImGui::Text("%s", managerName.c_str());
        ImGui::PlotLines("", history.data(), static_cast<int>(history.size()),
            0, nullptr, 0.0f, 100.0f, ImVec2(0, 60));

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();

            // Calculate average
            float averagePercentage = 0.0f;
            int count = 0;
            for (size_t i = history.size() - 1; i >= 0 && count < averageFrameCount; --i, ++count) {
                averagePercentage += history[i];
            }
            averagePercentage /= count;

            ImGui::Text("Manager: %s", managerName.c_str());
            ImGui::Text("Percentage: %.2f%%", averagePercentage);
            ImGui::EndTooltip();
        }

        ImGui::Spacing();
    }

    // Display system performance as graphs
    ImGui::Text("System Performance");
    for (const auto& system : systemData) {
        const std::string& systemName = system.first;
        float systemPercentage = static_cast<float>(system.second / deltaTime) * 100.0f;

        auto& history = systemHistory[systemName];
        if (history.size() >= 100) {
            history.erase(history.begin());
        }
        history.push_back(systemPercentage);

        // Display the graph
        auto spacePos = system.first.find(" ");
        std::string rawName = system.first.c_str();
        rawName = system.first.c_str() + spacePos + 1;
        ImGui::Text("%s", rawName.c_str());
        ImGui::PlotLines("", history.data(), static_cast<int>(history.size()),
            0, nullptr, 0.0f, 100.0f, ImVec2(0, 60));

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();

            // Calculate average
            float averagePercentage = 0.0f;
            int count = 0;
            for (size_t i = history.size() - 1; i >= 0 && count < averageFrameCount; --i, ++count) {
                averagePercentage += history[i];
            }
            averagePercentage /= count;

            ImGui::Text("System: %s", systemName.c_str());
            ImGui::Text("Percentage: %.2f%%", averagePercentage);
            ImGui::EndTooltip();
        }

        ImGui::Spacing();
    }
}

void UIManager::ShowExplorer() {
    ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    // Create the tab bar
    if (ImGui::BeginTabBar("MyTabBar")) {

        // First tab: Console
        if (ImGui::BeginTabItem("Console")) {
            RenderConsole();
            ImGui::EndTabItem();
        }

        // Second tab: Assets
        if (ImGui::BeginTabItem("Assets")) {

            AssetsBrowser::ShowAssets();

   //         static AssetCategory currentCategory = AssetCategory::Scene;
   //         const char* items[] = { "Scene", "GameObject", "Audio" };

   //         ImGui::Text("Category:   ");
   //         ImGui::SameLine();

   //         if (ImGui::BeginCombo("##Category", items[static_cast<int>(currentCategory)])) {
   //             for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
   //                 bool is_selected = (static_cast<int>(currentCategory) == n);
   //                 if (ImGui::Selectable(items[n], is_selected)) {
   //                     currentCategory = static_cast<AssetCategory>(n);
   //                 }

   //                 if (is_selected) {
   //                     ImGui::SetItemDefaultFocus();
   //                 }
   //             }
			//	ImGui::EndCombo();
			//}

   //         // Handle the selected category
   //         switch (currentCategory) {
   //         case AssetCategory::Scene:
   //             RenderSceneAssets();
   //             break;
   //         case AssetCategory::GameObject:
   //             RenderGameObjectAssets();
   //             break;
   //         case AssetCategory::Audio:
   //             RenderAudioAssets();
   //             break;
   //         }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void UIManager::ShowInspector() 
{
    if (windowStates[WindowType::Inspector] && selectedEntityID != -1) 
    {
        ImGui::Begin("Inspector", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysVerticalScrollbar);

        // Use InspectorRenderer to render components of the selected entity
        InspectorRenderer::RenderComponents(selectedEntityID);

        ImGui::End();
    }
}


void UIManager::ShowHierarchy() {
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Get all entities
    std::vector<Entity> entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    for (size_t i = 0; i < entities.size(); ++i) {
        if (entities[i].entityID == 0) {
            continue;
        }

        // Create a unique label for each node
        std::string entityLabel = "GameObject " + std::to_string(entities[i].entityID);
        if (!entities[i].name.empty()) 
        {
            entityLabel = entities[i].name;
        }
        else
        {
            entities[i].name = entityLabel;
        }
        

        // Begin a tree node for each entity
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (selectedEntityID == entities[i].entityID) {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool nodeOpen = ImGui::TreeNodeEx(entityLabel.c_str(), nodeFlags);

        // Check if this entity node is selected
        if (ImGui::IsItemClicked()) {
            if (selectedEntityID == entities[i].entityID) {
                selectedEntityID = -1;  // Deselect the entity
                windowStates[WindowType::Inspector] = false; // Hide Inspector window
            }
            else {
                selectedEntityID = entities[i].entityID;  // Select the entity
                windowStates[WindowType::Inspector] = true;  // Show Inspector window
            }
        }

        // If the node is open, display children or other properties here
        if (nodeOpen) {
            ImGui::Text("Entity ID: %d", entities[i].entityID); // Example of displaying extra information
            ImGui::TreePop();
        }
    }

    ImGui::End();
}


// Get color based on log level
ImVec4 GetColorByLevel(const std::string& level) {
    if (level == "INFO") {
        return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green for info
    }
    else if (level == "WARNING") {
        return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow for warning
    }
    else if (level == "ERROR") {
        return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red for error
    }
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);      // Default white color
}

// Render the console
void UIManager::RenderConsole() {
    // Input field for commands
    static char inputBuf[256] = "";
    if (ImGui::InputText("Command", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        // Add input text to the log
        UIDebugConsole::debugConsole.AddDebugLog(inputBuf);

        // Execute command
        if (strcmp(inputBuf, "clear") == 0) {
            UIDebugConsole::debugConsole.Clear();  // Clear the log
        }
        else {
            UIDebugConsole::debugConsole.AddLog("WARNING", "Unknown command");
        }

        inputBuf[0] = '\0';
    }

    ImGui::Separator();

    // Display log area
    ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Retrieve and render log entries from UIDebugConsole
    const auto& logEntries = UIDebugConsole::debugConsole.GetLogEntries();
    for (const auto& [level, message] : logEntries) {
        ImVec4 color = GetColorByLevel(level);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(message.c_str());
        ImGui::PopStyleColor();
    }

    // Scroll to the bottom if needed
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
}


void UIManager::RenderWindows() {
    for (const auto& [window, isVisible] : windowStates) {
        if (isVisible) {
            switch (window) {
            case WindowType::DebugInfo:
                ShowDebugInfo();
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

void UIManager::SaveScene(const std::string& sceneName)
{
    auto& entityChanges = InspectorRenderer::entityChanges;  

    bool anyChanges = false;

    for (const auto& [entityID, hasChanged] : entityChanges) {
        if (hasChanged) 
        {
            LevelManager::SaveSceneChanges(GameManager::ActiveSceneName);

            entityChanges[entityID] = false;
            anyChanges = true;
        }
    }

    if (anyChanges) {
        std::cout << "Scene saved: " << sceneName << std::endl;
    }
    else {
        std::cout << "No changes to save." << std::endl;
    }
}