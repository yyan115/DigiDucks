#include "AssetsBrowser.h"
#include "PrefabManager.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;
std::string AssetsBrowser::selectedFolderPath = "../Resources";

// Main function to display the assets explorer UI
void AssetsBrowser::ShowAssets() {
    // Left pane for folder structure
    ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);
    RenderDirectoryTree(); // Render folders dynamically based on the content of "../Resources"
    ImGui::EndChild();

    ImGui::SameLine();

    // Right pane for displaying assets within the selected folder
    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
   
    RenderAssetGrid(selectedFolderPath);  // Display other assets in a grid
    ImGui::Separator();
    RenderPrefabsGrid(); // Display all loaded prefabs
    ImGui::EndChild();
}

// Renders top-level directories in the Resources folder dynamically
void AssetsBrowser::RenderDirectoryTree() {
    const std::string rootPath = "../Resources";

    // Iterate over directories in the root path
    for (const auto& entry : fs::directory_iterator(rootPath)) {
        if (entry.is_directory()) {
            std::string folderName = entry.path().filename().string();
            std::string folderPath = entry.path().string();

            // Display each folder as a selectable item
            if (ImGui::Selectable(folderName.c_str(), selectedFolderPath == folderPath)) {
                selectedFolderPath = folderPath; // Update the selected folder path
            }
            
        }
    }
}

// Render the assets in the right pane as a grid
void AssetsBrowser::RenderAssetGrid(const std::string& path) {
    if (!fs::exists(path)) return;

    int itemsPerRow = 4;
    int itemIndex = 0;
    std::cout << path << std::endl;
    
    // Iterate over files in the selected folder and display them in a grid
    for (const auto& entry : fs::directory_iterator(path)) {
        if (!entry.is_directory()) {
            std::string fileName = entry.path().filename().string();
                //std::cout << entry.path().string() << std::endl;

            ImGui::PushID(itemIndex);
            if (ImGui::Button(fileName.c_str(), ImVec2(100, 100))) {
                // Handle asset selection 
                std::cout << "Selected " << fileName << " in " << path << std::endl;
            }

            if ((itemIndex + 1) % itemsPerRow != 0) {
                ImGui::SameLine();
            }

            ImGui::PopID();
            itemIndex++;
        }
    }
    
}

void AssetsBrowser::RenderPrefabsGrid() {
    ImGui::Text("Prefabs:");
    ImGui::Separator();

    // Get all prefabs currently loaded in PrefabManager
    const auto& prefabs = PrefabManager::GetAllPrefabs();

    for (const auto& [prefabName, prefab] : prefabs) {
        ImGui::Text("%s", prefabName.c_str());

        // Enable drag-and-drop for each prefab
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            // Set the payload to carry the prefab name (or any other unique identifier)
            ImGui::SetDragDropPayload("PREFAB_PAYLOAD", prefabName.c_str(), prefabName.size() + 1);
            ImGui::Text("Dragging %s", prefabName.c_str());
            ImGui::EndDragDropSource();
        }
    }
}