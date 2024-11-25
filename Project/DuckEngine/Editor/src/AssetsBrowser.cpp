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
#include "FileExtensions.h"
#include "PrefabEditor.h"

#include <filesystem>
#include <iostream>
#include <algorithm> // For std::transform

static char searchQuery[128] = ""; // Buffer to store the search query
std::string queryLower = ""; // Lowercase version of the search query

namespace fs = std::filesystem;
std::string AssetsBrowser::selectedFolderPath = "Resources/Scenes";
std::string AssetsBrowser::selectedFolderName = "Scenes";

static bool showErrorPopup = false;
static std::string errorMessage = "";

// Main function to display the assets explorer UI
void AssetsBrowser::ShowAssets() {
	// Render the search bar at the top
	ImGui::InputTextWithHint("##SearchBar", "Search assets...", searchQuery, IM_ARRAYSIZE(searchQuery));
	queryLower = searchQuery;
	std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), 
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });

	ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Open in Explorer").x - ImGui::GetStyle().FramePadding.x * 2);

	// Render the File Explorer button
	if (ImGui::Button("Open in Explorer")) {
		// Open the selected folder path in File Explorer
		std::string pathToOpen = selectedFolderPath;
		std::replace(pathToOpen.begin(), pathToOpen.end(), '/', '\\');

		// Ensure the path exists before trying to open it
		if (fs::exists(pathToOpen)) {
			ShellExecuteA(nullptr, "open", pathToOpen.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
		}
		else {
			std::cerr << "Error: Path does not exist - " << pathToOpen << std::endl;
		}
	}

	// Left pane for folder structure
	ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);
	RenderDirectoryTree(); 
	ImGui::EndChild();

	ImGui::SameLine();

	// Right pane for displaying assets within the selected folder
	ImGui::BeginChild("RightPane", ImVec2(0, 0), true);    
	// Render the assets grid based on the selected folder
	if (selectedFolderName.compare("Prefabs") == 0) RenderPrefabsGrid();
	else RenderAssetGrid(selectedFolderPath);
	ImGui::EndChild();

	PrefabEditor::Render();
}

// Renders top-level directories in the Resources folder
void AssetsBrowser::RenderDirectoryTree() {
	const std::string rootPath = "Resources";
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

			if (folderName == "EditorIcons") continue; // Skip the "EditorIcons" directory

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

void AssetsBrowser::RenderAssetGrid(const std::string& path) {
	if (!fs::exists(path)) return;

	// Calculate how many items can fit in one row
	float contentWidth = ImGui::GetContentRegionAvail().x;
	float itemWidth = 128.0f; // Width of each asset cell
	float itemPadding = 20.0f; // Padding between items
	int itemsPerRow = static_cast<int>(contentWidth / (itemWidth + itemPadding));
	if (itemsPerRow < 1) itemsPerRow = 1;

	std::vector<fs::directory_entry> directories;
	std::vector<fs::directory_entry> files;

	// Separate directories and files
	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.is_directory()) {
			directories.push_back(entry);
		}
		else {
			files.push_back(entry);
		}
	}

	int itemIndex = 0;

	ImGui::Separator();
	// Render directories first
	for (const auto& entry : directories) {
		std::string fileName = entry.path().filename().string();
		std::string normalizedPath = NormalizePath(entry.path().string());

		ImGui::PushID(normalizedPath.c_str());
		ImGui::BeginGroup();

		// Load the folder icon
		auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("duck_folder_icon");
		if (texture) {
			// Render the icon as a clickable image button
			if (ImGui::ImageButton(fileName.c_str(), (void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0))) {
				// Navigate into the subfolder when clicked
				selectedFolderPath = normalizedPath;
				selectedFolderName = fileName;
			}
		}

		// Calculate text width and center-align
		float textWidth = ImGui::CalcTextSize((fileName + "/").c_str()).x;
		float offsetX = (128 - textWidth) * 0.5f; // Center within 128px icon width
		if (offsetX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

		// Display the folder name below the icon
		ImGui::TextWrapped("%s", ("/" + fileName).c_str());
		ImGui::EndGroup();

		itemIndex++;
		if (itemIndex % itemsPerRow != 0) {
			ImGui::SameLine();
		}
		else {
			ImGui::NewLine();
		}

		ImGui::PopID();
	}
	
	// Reset layout for files
	if (itemIndex % itemsPerRow != 0) {
		ImGui::NewLine();
	}
	itemIndex = 0;
	ImGui::Separator();

	// Render files
	for (const auto& entry : files) {
		std::string fileName = entry.path().filename().string();
		std::string truncatedFileName = fileName;
		std::string fileExtension = entry.path().extension().string();
		std::string normalizedPath = NormalizePath(entry.path().string());

		// Filter assets based on the search query
		std::string fileNameLower = fileName;
		std::transform(fileNameLower.begin(), fileNameLower.end(), fileNameLower.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });

		if (!queryLower.empty() && fileNameLower.find(queryLower) == std::string::npos) {
			continue; // Skip files that don't match the query
		}

		// Find the parent directory
		std::string canonicalPath = fs::canonical(entry.path()).string();
		std::string parentDir = "";
		for (const auto& [dir, extensions] : folderAllowedExtensions) {
			std::string canonicalParent = fs::canonical("Resources/" + dir).string();
			if (canonicalPath.find(canonicalParent) == 0) {
				parentDir = dir;
				break;
			}
		}

		// Find the icon for the file extension
		std::string iconName = "";
		auto it = fileIcons.find(fileExtension);
		if (it != fileIcons.end()) {
			iconName = it->second;
		}

		ImGui::PushID(normalizedPath.c_str());
		ImGui::BeginGroup();

		// Validate file extension based on parent directory
		if (!parentDir.empty()) {
			const auto& allowedExtensions = folderAllowedExtensions[parentDir];
			if (std::find(allowedExtensions.begin(), allowedExtensions.end(), fileExtension) != allowedExtensions.end()) {
				// Render based on parent directory type
				if (parentDir == "Sprites") {
					// Load the texture if not already loaded
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(normalizedPath);
					if (texture) ImGui::Image((void*)(intptr_t)(*texture), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
					else {
						DuckEngine::DUCKENGINE_AssetManager.LoadTexture(normalizedPath);
						ImGui::Button(fileName.c_str(), ImVec2(128, 128));
					}
					// Drag/drop source for the sprite
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
				else if (parentDir == "Sounds") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName(iconName);
					if (texture) ImGui::Image((void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("SOUND_PAYLOAD", normalizedPath.c_str(), entry.path().string().size() + 1);
						ImGui::Text("Drag %s", fileName.c_str());
						ImGui::EndDragDropSource();
					}
				}
				else if (parentDir == "Scenes") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName(iconName);
					if (texture) ImGui::Image((void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
					
				}

				else {
					ImGui::Button(fileName.c_str(), ImVec2(128, 128));
				}
			}
			else {
				// Invalid file for the folder
				showErrorPopup = true;
				errorMessage = "Error: File '" + fileName + "' has an invalid extension (" + fileExtension + ") for folder '" + parentDir + "'.";
			}
		}
		// Calculate text width and center-align
		if (truncatedFileName.length() > 12) truncatedFileName = truncatedFileName.substr(0, 9) + "...";
		float textWidth = ImGui::CalcTextSize((truncatedFileName + "/").c_str()).x;
		float offsetX = (128 - textWidth) * 0.5f; // Center within 128px icon width
		if (offsetX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

		// Truncate file name        
		ImGui::TextWrapped("%s", truncatedFileName.c_str());
		ImGui::EndGroup();

		itemIndex++;
		if (itemIndex % itemsPerRow != 0) {
			ImGui::SameLine();
		}
		else {
			ImGui::NewLine();
		}

		ImGui::PopID();
	}

	// Show error popup if an invalid file was detected
	if (showErrorPopup) {
		ImGui::OpenPopup("Invalid File Error");
	}

	// Center the error popup
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Invalid File Error", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::TextWrapped("%s", errorMessage.c_str());
		float buttonWidth = ImGui::CalcTextSize("Close").x + ImGui::GetStyle().FramePadding.x * 2.0f;
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - buttonWidth); // Align to the right

		if (ImGui::Button("Close")) {
			showErrorPopup = false; // Reset the popup flag
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}




// Render the prefabs in the right pane as a grid
void AssetsBrowser::RenderPrefabsGrid() {
	// Calculate how many items can fit in one row
	float contentWidth = ImGui::GetContentRegionAvail().x;
	float itemWidth = 128.0f; // Width of each asset cell
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
			ImGui::Image((void*)(intptr_t)(*texture), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		}
		else {
			ImGui::Button(prefabName.c_str(), ImVec2(128, 128)); // Fallback button if no texture is found
		}

		// Drag-and-drop source for the prefab
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload("PREFAB_PAYLOAD", prefabName.c_str(), prefabName.size() + 1);
			ImGui::Text("Drag %s", prefabName.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			PrefabEditor::OpenPrefabEditor(prefabName); 
		}

		std::string truncatedPrefabName = prefabName;
		if (truncatedPrefabName.length() > 12) truncatedPrefabName = truncatedPrefabName.substr(0, 9) + "...";
		float textWidth = ImGui::CalcTextSize((truncatedPrefabName + "/").c_str()).x;
		float offsetX = (128 - textWidth) * 0.5f; // Center within 128px icon width
		if (offsetX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
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

// Handle file drag/drop into the asset browser
void AssetsBrowser::HandleFileDrop(GLFWwindow * window, int count, const char** paths) {
	UNREFERENCED_PARAMETER(window);

	for (int i = 0; i < count; ++i) {
		std::string filePath = paths[i];
		std::string extension = fs::path(filePath).extension().string();
		std::string fileName = fs::path(filePath).filename().string();

		// Validate if the file extension is allowed in the current folder
		auto allowedExtensions = folderAllowedExtensions.find(selectedFolderName);
		if (allowedExtensions != folderAllowedExtensions.end()) {
			if (std::find(allowedExtensions->second.begin(), allowedExtensions->second.end(), extension) == allowedExtensions->second.end()) {
				// Set the popup state and error message
				showErrorPopup = true;
				errorMessage = "Error: Unsupported file type '" + extension + "' for folder '" + selectedFolderName + "'.";
				continue; // Skip this file
			}
		}

		// Ensure the destination folder exists
		std::string destinationPath = selectedFolderPath + "/" + fileName;
		try {
			fs::create_directories(selectedFolderPath);
		}
		catch (const fs::filesystem_error& e) {
			std::cerr << "Failed to create directory: " << e.what() << std::endl;
			continue;
		}

		// Copy the file to the destination folder
		try {
			fs::copy_file(filePath, destinationPath, fs::copy_options::overwrite_existing);
			std::cout << "File added to: " << destinationPath << std::endl;
		}
		catch (const fs::filesystem_error& e) {
			std::cerr << "Failed to copy file: " << e.what() << std::endl;
		}
	}
}