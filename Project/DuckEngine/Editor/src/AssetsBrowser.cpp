/******************************************************************************/
/*!
\file     AssetsBrowser.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the AssetsBrowser class
		  which is responsible for displaying the assets explorer UI in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "AssetsBrowser.h"
#include "PrefabManager.h"
#include "imgui.h"
#include "DuckEngine.h"
#include "LevelManager.h"

#include <filesystem>
#include <iostream>
#include <algorithm> // For std::transform

static char searchQuery[128] = ""; // Buffer to store the search query
std::string queryLower = ""; // Lowercase version of the search query

namespace fs = std::filesystem;
std::string AssetsBrowser::selectedFolderPath = "../Resources/Scenes";
std::string AssetsBrowser::selectedFolderName = "Scenes";

// Main function to display the assets explorer UI
void AssetsBrowser::ShowAssets() {
    // Render the search bar at the top
    ImGui::InputTextWithHint("##SearchBar", "Search assets...", searchQuery, IM_ARRAYSIZE(searchQuery));
    queryLower = searchQuery;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), 
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    // Left pane for folder structure
    ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);
    RenderDirectoryTree(); 
    ImGui::EndChild();

    ImGui::SameLine();

    // Right pane for displaying assets within the selected folder
    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
    
    if (selectedFolderName.compare("Prefabs") == 0) {
        // Display all loaded prefabs
		RenderPrefabsGrid(); 
	}
	else {
        // Display other assets in a grid
		RenderAssetGrid(selectedFolderPath);  
	}
    ImGui::EndChild();
}

// Renders top-level directories in the Resources folder
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

std::string NormalizePath(const std::string& path) {
    std::string normalizedPath = path;
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
    return normalizedPath;
}

// Render the assets in the right pane as a grid
void AssetsBrowser::RenderAssetGrid(const std::string& path) {
    if (!fs::exists(path)) return;

	// Calculate how many items can fit in one row
    float contentWidth = ImGui::GetContentRegionAvail().x;
    float itemWidth = 120.0f; // Width of each asset cell
    float itemPadding = 20.0f; // Padding between items
    int itemsPerRow = static_cast<int>(contentWidth / (itemWidth + itemPadding));
    if (itemsPerRow < 1) itemsPerRow = 1;

    int itemIndex = 0;

    static std::string selectedAsset = "";

    // Iterate over files in the selected folder
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_directory()) continue;

        std::string fileName = entry.path().filename().string();
        std::string fileNameLower = fileName;
        std::string truncatedFileName = fileName;        
        std::transform(fileNameLower.begin(), fileNameLower.end(), fileNameLower.begin(), 
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        // Filter assets based on the search query
        if (!queryLower.empty() && fileNameLower.find(queryLower) == std::string::npos) {
            continue; // Skip files that don't match the query
        }

        std::string fileExtension = entry.path().extension().string();
        std::string normalizedPath = NormalizePath(entry.path().string());
        ImGui::PushID(normalizedPath.c_str());

        ImGui::BeginGroup();
        // Check if the file is a texture
        if (fileExtension == ".png" || fileExtension == ".jpg" || fileExtension == ".jpeg") {
            
            auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(normalizedPath);

            // If texture is valid, display it as an image
            if (texture) {
                ImGui::Image((void*)(intptr_t)(*texture), ImVec2(100, 100), ImVec2(0,1), ImVec2(1,0));
            }
            else {
                DuckEngine::DUCKENGINE_AssetManager.LoadTexture(normalizedPath); // Load the texture if not already loaded
                ImGui::Button(fileName.c_str(), ImVec2(100, 100));
            }

            // Set up drag-and-drop source for sprites
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                ImGui::SetDragDropPayload("SPRITE_PAYLOAD", normalizedPath.c_str(), entry.path().string().size() + 1);
                ImGui::Text("Drag %s", fileName.c_str());
                ImGui::EndDragDropSource();
            }

            // Open context menu on right-click
            if (ImGui::BeginPopupContextItem(("Replace##" + normalizedPath).c_str())) {
                // Replace the asset with the new file
                if (ImGui::MenuItem("Replace texture")) {
                    std::string newFilePath = LevelManager::OpenFileDialog("texture");
                    if (!newFilePath.empty()) {
                        ReplaceAsset(normalizedPath, newFilePath); 
                    }
                }
                ImGui::EndPopup();
            }
            
        }
        else if (fileExtension == ".ogg" || fileExtension == ".mp3" || fileExtension == ".wav") {

            ImGui::Button(fileName.c_str(), ImVec2(100, 100));

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload("SOUND_PAYLOAD", normalizedPath.c_str(), entry.path().string().size() + 1);
                ImGui::Text("Drag %s", fileName.c_str());
                ImGui::EndDragDropSource();
            }
        }
        else {
            // Non-texture files displayed as buttons
            ImGui::Button(fileName.c_str(), ImVec2(100, 100));
        }

        // Truncate file name
        if (truncatedFileName.length() > 15) truncatedFileName = truncatedFileName.substr(0, 12) + "...";
        ImGui::TextWrapped("%s", truncatedFileName.c_str());
        ImGui::EndGroup();

        if ((itemIndex + 1) % itemsPerRow != 0) {
            ImGui::SameLine();
        }

        ImGui::PopID();
        itemIndex++;
    }
    
}

// Render theprefabs in the right pane as a grid
void AssetsBrowser::RenderPrefabsGrid() {
    // Calculate how many items can fit in one row
    float contentWidth = ImGui::GetContentRegionAvail().x;
    float itemWidth = 120.0f; // Width of each asset cell
    float itemPadding = 20.0f; // Padding between items
    int itemsPerRow = static_cast<int>(contentWidth / (itemWidth + itemPadding));
    if (itemsPerRow < 1) itemsPerRow = 1;
    int itemIndex = 0;

    // Retrieve all prefabs loaded in PrefabManager
    auto prefabs = PrefabManager::GetAllPrefabs();
    for (const auto& [prefabName, prefab] : prefabs) {
        ImGui::PushID(itemIndex);

        ImGui::BeginGroup();
        // Retrieve and display prefab texture
        if (auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(prefab->texturePath)) {
            ImGui::Image((void*)(intptr_t)(*texture), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
        }
        else {
            ImGui::Button(prefabName.c_str(), ImVec2(100, 100)); // Fallback button if no texture is found
        }

        // Drag-and-drop source for the prefab
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("PREFAB_PAYLOAD", prefabName.c_str(), prefabName.size() + 1);
            ImGui::Text("Drag %s", prefabName.c_str());
            ImGui::EndDragDropSource();
        }
		std::string truncatedPrefabName = prefabName;
        if (truncatedPrefabName.length() > 15) truncatedPrefabName = truncatedPrefabName.substr(0, 12) + "...";
        ImGui::TextWrapped("%s", truncatedPrefabName.c_str());
        ImGui::EndGroup();

        if ((itemIndex + 1) % itemsPerRow != 0) {
            ImGui::SameLine();
        }

        ImGui::PopID();
        itemIndex++;
    }
}

void AssetsBrowser::ReplaceAsset(const std::string& oldPath, const std::string& newPath) {
    // Unload the current asset if loaded
    if (DuckEngine::DUCKENGINE_AssetManager.IsTextureLoaded(oldPath)) {
        DuckEngine::DUCKENGINE_AssetManager.UnloadTexture(oldPath);
    }

    // Replace the original file with the new file
    try {
        std::filesystem::copy(newPath, oldPath, std::filesystem::copy_options::overwrite_existing);
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error replacing asset: " << e.what() << std::endl;
        return;
    }

    // Reload the texture to update the in-memory reference
    DuckEngine::DUCKENGINE_AssetManager.ReloadTexture(oldPath, oldPath);

    // Update only entities that use this texture
    for (auto& entity : DuckEngine::DUCKENGINE_EntityManager.GetEntities()) {
        int entityID = entity.entityID;
        if (DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SpriteRendererComponent>(entityID)) {
            auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityID);
            if (spriteRenderer && spriteRenderer->texturePath == oldPath) {
                spriteRenderer->texture = *DuckEngine::DUCKENGINE_AssetManager.GetTexture(oldPath);
            }
        }
    }

    std::cout << "Asset replaced successfully: " << oldPath << " with " << newPath << std::endl;
}
