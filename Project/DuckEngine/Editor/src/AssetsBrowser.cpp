#include "AssetsBrowser.h"
#include "PrefabManager.h"
#include "imgui.h"
#include "DuckEngine.h"
#include "LevelManager.h"
#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;
std::string AssetsBrowser::selectedFolderPath = "../Resources/Scenes";
std::string AssetsBrowser::selectedFolderName = "Scenes";

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

std::string NormalizePath(const std::string& path) {
    std::string normalizedPath = path;
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
    return normalizedPath;
}

// Render the assets in the right pane as a grid
void AssetsBrowser::RenderAssetGrid(const std::string& path) {
    if (!fs::exists(path)) return;

    int itemsPerRow = 4;
    int itemIndex = 0;
    static std::string selectedAsset = ""; // Store the selected asset's file path

    // Iterate over files in the selected folder and display them in a grid
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_directory()) continue;  // Skip directories in the right pane

        std::string fileName = entry.path().filename().string();
        std::string fileExtension = entry.path().extension().string();
        std::string normalizedPath = NormalizePath(entry.path().string());  // Normalize the path
        ImGui::PushID(itemIndex);
        std::cout << "File: " << normalizedPath << std::endl;  // Verify normalized path output

        // Check if the file is a texture (image file)
        if (fileExtension == ".png" || fileExtension == ".jpg" || fileExtension == ".jpeg") {
            auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(normalizedPath);

            // If texture is valid, display it as an image
            if (texture) {
                ImGui::Image((void*)(intptr_t)(*texture), ImVec2(100, 100), ImVec2(0,1), ImVec2(1,0)); // Display thumbnail
            }
            else {
                ImGui::Button(fileName.c_str(), ImVec2(100, 100)); // Fallback if texture is not loaded
            }
        }
        else {
            // Non-texture files can still be displayed as buttons
            ImGui::Button(fileName.c_str(), ImVec2(100, 100));
        }

        // Set up drag-and-drop source for sprites
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("SPRITE_PAYLOAD", entry.path().string().c_str(), entry.path().string().size() + 1); // Payload is the texture path
            ImGui::Text("Drag %s", fileName.c_str());
            ImGui::EndDragDropSource();
        }

        // Open context menu on right-click
        //if (ImGui::BeginPopupContextItem()) {
        //    if (ImGui::MenuItem("Replace Asset")) {
        //        std::string newFilePath = LevelManager::OpenFileDialog("texture");
        //        if (!newFilePath.empty()) {
        //            ReplaceAsset(entry.path().string(), newFilePath); // Replace the asset with the new file
        //        }
        //    }
        //    ImGui::EndPopup();
        //}

        if ((itemIndex + 1) % itemsPerRow != 0) {
            ImGui::SameLine();
        }

        ImGui::PopID();
        itemIndex++;
    }
    
}

void AssetsBrowser::RenderPrefabsGrid() {
    int itemsPerRow = 4;
    int itemIndex = 0;

    // Retrieve all prefabs loaded in PrefabManager
    auto prefabs = PrefabManager::GetAllPrefabs();
    for (const auto& [prefabName, prefab] : prefabs) {
        ImGui::PushID(itemIndex);

        // Retrieve and display prefab texture
        if (auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(prefab->texturePath)) {
            ImGui::Image((void*)(intptr_t)(*texture), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0)); // Display texture thumbnail
        }
        else {
            ImGui::Button(prefabName.c_str(), ImVec2(100, 100)); // Fallback button if no texture is found
        }

        // Drag-and-drop source for the prefab
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
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

void AssetsBrowser::ReplaceAsset(const std::string& oldPath, const std::string& newPath) {
    // Unload the current asset in memory
    std::string fileName = fs::path(oldPath).filename().string();
    if (DuckEngine::DUCKENGINE_AssetManager.IsTextureLoaded(fileName)) {
        DuckEngine::DUCKENGINE_AssetManager.UnloadTexture(fileName); // Unload the texture to free file
    }

    // Copy new asset to a temporary file location
    std::string tempPath = oldPath + ".tmp";
    try {
        fs::copy(newPath, tempPath, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error copying to temp file: " << e.what() << std::endl;
        return;
    }

    // Remove the original file after unloading, then move the temp file to the original path
    try {
        fs::remove(oldPath);
        fs::rename(tempPath, oldPath);
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error replacing asset: " << e.what() << std::endl;
        return;
    }

    // Reload the texture to update the in-memory reference
    DuckEngine::DUCKENGINE_AssetManager.ReloadTexture(fileName, oldPath);

    // Refresh entities or components using this asset
    auto entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entity.entityID)) {
            if (spriteRenderer->texturePath == oldPath) {
                spriteRenderer->texture = *DuckEngine::DUCKENGINE_AssetManager.GetTexture(fileName);
            }
        }
    }

    std::cout << "Asset replaced successfully: " << oldPath << " with " << newPath << std::endl;
}