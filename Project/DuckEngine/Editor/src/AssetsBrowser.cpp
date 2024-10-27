#include "AssetsBrowser.h"
#include <filesystem>
#include "imgui.h"

namespace fs = std::filesystem;

// Show the main explorer UI
void AssetsBrowser::ShowAssets() 
{

    static AssetCategory currentCategory = AssetCategory::Scene;
    const char* items[] = { "Scene", "GameObject", "Audio" };

    // Dropdown for asset categories
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

    // Define the root directory for assets
    static const std::string assetRootDir = "../Resources";

    // Render the file system hierarchy based on the selected category
    RenderDirectory(assetRootDir, currentCategory);
}

// Recursive function to render directories and files
void AssetsBrowser::RenderDirectory(const std::string& path, AssetCategory category) {
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            // Create a tree node for each directory
            if (ImGui::TreeNode(entry.path().filename().string().c_str())) {
                RenderDirectory(entry.path().string(), category);
                ImGui::TreePop();
            }
        }
        else {
            // Filter files based on the current category
            std::string extension = entry.path().extension().string();
            bool showFile = false;
            switch (category) {
            case AssetCategory::Scene:
                showFile = (extension == ".json");  // Adjust extension as needed
                break;
            case AssetCategory::GameObject:
                showFile = (extension == ".object");  // Adjust extension as needed
                break;
            case AssetCategory::Audio:
                showFile = (extension == ".wav" || extension == ".mp3");  // Adjust as needed
                break;
            }

            if (showFile) {
                // Display file
                ImGui::Selectable(entry.path().filename().string().c_str());
            }
        }
    }
}
