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
#include "Color.h"
#include "Inspector.h"
#include "GameManager.h"
#include "AssetsBrowser.h"
#include "EditorTheme.h"
#include "HierarchyList.h"

#include <Windows.h>

// Store historical data for performance tracking
std::unordered_map<std::string, std::vector<float>> managerHistory;
std::unordered_map<std::string, std::vector<float>> systemHistory;

int UIManager::selectedEntityID = -1;
std::unordered_map<WindowType, bool> UIManager::windowStates = {
    {WindowType::DebugInfo, false},
    {WindowType::Inspector, false},
    {WindowType::NewScene, false},
};

void UIManager::Initialize() 
{
    GLFWwindow* window = WindowManager::getWindow();
    
    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    // duck theme
    EditorTheme::SetDuckTheme();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    //Initialize platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    glfwSetDropCallback(window, UIManager::FileDropCallback);
    // Enable file drop callback in GLFW
    //glfwSetDropCallback(window, TestDropCallback);
    std::cout << "Drop callback set!" << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;

    // font
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("Resources/Inter.ttf", 18.0f);
    ImGui_ImplOpenGL3_CreateFontsTexture(); // Rebuild font atlas
}

// Render the ImGui windows with a specific size and position to make it adaptive
void UIManager::RenderImGuiWindows(float WidthOffset, float HeightOffset, float PosX, float PosY) 
{
    // Get the current window size
    int windowWidth = WindowManager::GetWindowWidth();
    int windowHeight = WindowManager::GetWindowHeight();

    // Set the ImGui window to automatically adapt to the window size
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(windowWidth * WidthOffset), static_cast<float>(windowHeight * HeightOffset))); // Set size relative to window size
    ImGui::SetNextWindowPos(ImVec2(windowWidth * PosX, windowHeight * PosY)); // Set position
}

void UIManager::StartRender()
{
    TimeManager::StartManagerTimer("Editor Manager");
    
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::Render() 
{
    // Show the main menu bar
    ShowMenuBar();

    // Create the dockspace
    CreateDockSpace();

    // Rendering debug stats
    RenderWindows();
    // Show the different windows
    ShowExplorer();
    ShowHierarchy();
    ShowInspector();

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntityID);
    if (transform)
    {
        GraphicsManager::gizmoData = { transform->position, 3.f };
        GraphicsManager::entityIsSelected = true;
    }
    else {
        GraphicsManager::entityIsSelected = false;
    }
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
    TimeManager::EndManagerTimer("Editor Manager");
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
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                windowStates[WindowType::NewScene] = true;
                std::cout << "wtfd" << std::endl;
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { LevelManager::OpenLevelDialog(); }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) 
            {
                LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
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
        if (ImGui::BeginMenu("Game Object")) {
            if (ImGui::MenuItem("Spawn GameObject")) { 
                DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", { 0.0f, 0.0f }, { 5.0f, 5.0f });
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

void UIManager::ShowDebugInfo() 
{
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

void UIManager::RenderPerformanceGraphs(const SystemManager& systemManager) 
{
    const auto& systemData = systemManager.GetSystemData();
    const auto& managerData = TimeManager::GetManagerData();
    float total_time = static_cast<float>(TimeManager::GetTotalTime()); // Total game loop time

    const int averageFrameCount = 10;

    // Display manager performance as graphs
    for (const auto& manager : managerData) {
        const std::string& managerName = manager.first;
        float managerPercentage = static_cast<float>(manager.second / total_time) * 100.0f;

        auto& history = managerHistory[managerName];
        if (history.size() >= 100) {
            history.erase(history.begin());
        }
        history.push_back(managerPercentage);

        float averagePercentage = 0.0f;
        int count = 0;
        for (int i = static_cast<int>(history.size()) - 1; i >= 0 && count < averageFrameCount; --i, ++count) {
            averagePercentage += history[i];
        }
        // Avoid division by zero
        if (count > 0) { 
            averagePercentage /= count;
        }
        else {
            averagePercentage = 0.0f;
        }

        // Display the graph
        ImGui::Text("%s ( %.2f%% )", managerName.c_str(), averagePercentage);
        ImGui::PlotLines("", history.data(), static_cast<int>(history.size()),
            0, nullptr, 0.0f, 100.0f, ImVec2(0, 60));

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("System: %s", managerName.c_str());
            ImGui::Text("Percentage: %.2f%%", averagePercentage);
            ImGui::EndTooltip();
        }

        ImGui::Spacing();
    }

    // Display system performance as graphs
    ImGui::Text("Component systems");
    for (const auto& system : systemData) {
        const std::string& systemName = system.first;
        float systemPercentage = static_cast<float>(system.second / total_time) * 100.0f;

        auto& history = systemHistory[systemName];
        if (history.size() >= 100) {
            history.erase(history.begin());
        }
        history.push_back(systemPercentage);

        // Display the graph
        auto spacePos = system.first.find(" ");
        std::string rawName = system.first.c_str();
        rawName = system.first.c_str() + spacePos + 1;

        float averagePercentage = 0.0f;
        int count = 0;
        for (int i = static_cast<int>(history.size()) - 1; i >= 0 && count < averageFrameCount; --i, ++count) {
            averagePercentage += history[i];
        }
        // Avoid division by zero
        if (count > 0) {
            averagePercentage /= count;
        }
        else {
            averagePercentage = 0.0f;
        }

        ImGui::Text("%s ( %.2f%% )", rawName.c_str(), averagePercentage);
        ImGui::PlotLines("", history.data(), static_cast<int>(history.size()),
            0, nullptr, 0.0f, 100.0f, ImVec2(0, 60));

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("System: %s", systemName.c_str());
            ImGui::Text("Percentage: %.2f%%", averagePercentage);
            ImGui::EndTooltip();
        }

        ImGui::Spacing();
    }
}

void UIManager::ShowExplorer() 
{
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
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void UIManager::ShowInspector() 
{
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysVerticalScrollbar);
    // Use InspectorRenderer to render components of the selected entity
    InspectorRenderer::RenderLayer(selectedEntityID);
    InspectorRenderer::RenderComponents(selectedEntityID);
    ImGui::End();   
}


void UIManager::ShowHierarchy() {
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    // Render the hierarchy window
    Hierarchy::ShowHierarchy(selectedEntityID);
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
				
				break;
            case WindowType::NewScene:
                CreateNewSceneDialog();
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

void UIManager::SaveScene(const std::string& sceneName)
{
    auto& entityChanges = InspectorRenderer::entityChanges;  

    bool anyChanges = false;

    for (const auto& [entityID, hasChanged] : entityChanges) {
        if (hasChanged) 
        {
            LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());

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

void UIManager::FileDropCallback(GLFWwindow* window, int count, const char** paths) {
    UNREFERENCED_PARAMETER(window);
    const std::string resourcesPath = "Resources/";

    for (int i = 0; i < count; i++) {
        std::string filePath = paths[i];
        std::string extension = std::filesystem::path(filePath).extension().string();
        std::string fileName = std::filesystem::path(filePath).filename().string();

        // Determine the destination folder based on the file extension
        std::string destinationFolder;
        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
            destinationFolder = resourcesPath + "Sprites/";
        }
        else if (extension == ".wav" || extension == ".mp3" || extension == ".ogg") {
            destinationFolder = resourcesPath + "Sounds/";
        }
        else if (extension == ".json") {
            destinationFolder = resourcesPath + "Scenes/";
        }
        else {
            std::cout << "Unsupported file type: " << extension << std::endl;
            continue; // Skip unsupported files
        }

        // Ensure the destination folder exists
        try {
            std::filesystem::create_directories(destinationFolder);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to create directory: " << e.what() << std::endl;
            continue;
        }

        // Copy the file to the destination folder
        std::string destinationPath = destinationFolder + fileName;
        try {
            std::filesystem::copy_file(filePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
            std::cout << "File moved to: " << destinationPath << std::endl;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to copy file: " << e.what() << std::endl;
        }
    }
}


void UIManager::CreateNewSceneDialog() {
    static char sceneName[128] = ""; // Buffer for scene name

     ImGui::OpenPopup("Create New Scene");


    // Render the popup
    if (ImGui::BeginPopupModal("Create New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter Scene Name:");
        ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName));

        // Create button
        if (ImGui::Button("Create")) {
            std::string sceneNameStr(sceneName);

            // Validate the scene name
            if (sceneNameStr.empty()) {
                ImGui::Text("Invalid scene name!");
            }
            else {
                // Try to create the new scene
                if (LevelManager::CreateNewScene(sceneNameStr)) {
                    ImGui::CloseCurrentPopup();
					windowStates[WindowType::NewScene] = false;
                }
                else {
                    ImGui::Text("Error: Scene creation failed or already exists.");
                }
            }
        }

        ImGui::SameLine();

        // Cancel button
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            windowStates[WindowType::NewScene] = false;
        }

        ImGui::EndPopup();
    }
}