#include "AssetsBrowser.h"
#include "PrefabManager.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;
std::string AssetsBrowser::selectedFolderPath = "../Resources";
std::string AssetsBrowser::selectedFolderName = "";

// Main function to display the assets explorer UI
void AssetsBrowser::ShowAssets() {
    // Left pane for folder structure
    ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);
    RenderDirectoryTree(); // Render folders dynamically
    ImGui::EndChild();

    ImGui::SameLine();

    // Right pane for displaying assets within the selected folder
    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
    
    if (selectedFolderName.compare("Prefabs") == 0) {
		RenderPrefabsGrid(); // Display all loaded prefabs
	}
	else {
		RenderAssetGrid(selectedFolderPath);  // Display other assets in a grid
	}
    ImGui::EndChild();
}

// Renders top-level directories in the Resources folder dynamically
void AssetsBrowser::RenderDirectoryTree() {
    const std::string rootPath = "../Resources";
    const std::string prefabsPath = rootPath + "/Prefabs";

    // Ensure "Prefabs" directory exists
    if (!fs::exists(prefabsPath)) {
        fs::create_directory(prefabsPath);
    }

    // Iterate over directories in the root path
    for (const auto& entry : fs::directory_iterator(rootPath)) {
        if (entry.is_directory()) {
            std::string folderName = entry.path().filename().string();
            std::string folderPath = entry.path().string();

            // Display each folder as a selectable item
            if (ImGui::Selectable(folderName.c_str(), selectedFolderPath == folderPath)) {
                selectedFolderPath = folderPath; // Update the selected folder path
                selectedFolderName = folderName; // Update the selected folder name
            }
            
        }
    }
}

// Render the assets in the right pane as a grid
void AssetsBrowser::RenderAssetGrid(const std::string& path) {
    if (!fs::exists(path)) return;

    int itemsPerRow = 4;
    int itemIndex = 0;
    
    // Iterate over files in the selected folder and display them in a grid
    for (const auto& entry : fs::directory_iterator(path)) {
        if (!entry.is_directory()) {
            std::string fileName = entry.path().filename().string();

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
    int itemsPerRow = 4;
    int itemIndex = 0;

    // Retrieve all prefabs loaded in PrefabManager
    auto prefabs = PrefabManager::GetAllPrefabs();
    for (const auto& [prefabName, prefab] : prefabs) {
        ImGui::PushID(itemIndex);

        // Display each prefab as a button
        if (ImGui::Button(prefabName.c_str(), ImVec2(100, 100))) {
            // Placeholder for prefab selection action
            std::cout << "Selected prefab: " << prefabName << std::endl;
        }

        // Drag-and-drop source for the prefab
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            ImGui::SetDragDropPayload("PREFAB_PAYLOAD", prefabName.c_str(), prefabName.size() + 1);  // Pass prefab name as payload
            ImGui::Text("Drag %s", prefabName.c_str());
            ImGui::EndDragDropSource();
        }

        if ((itemIndex + 1) % itemsPerRow != 0) {
            ImGui::SameLine();
        }

        ImGui::PopID();
        itemIndex++;
    }
}