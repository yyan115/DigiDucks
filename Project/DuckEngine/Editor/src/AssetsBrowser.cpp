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
#include <algorithm>

static char searchQuery[128] = ""; // Buffer to store the search query
std::string queryLower = ""; // Lowercase version of the search query

namespace fs = std::filesystem;
std::string AssetsBrowser::selectedFolderPath = "Resources/Scenes";
std::string AssetsBrowser::selectedFolderName = "Scenes";

static bool showErrorPopup = false;
static std::string errorMessage = "";

static std::unordered_map<std::string, std::filesystem::file_time_type> fileModificationTimes;


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
		std::string pathToOpen = selectedFolderPath;
		std::replace(pathToOpen.begin(), pathToOpen.end(), '/', '\\');

		// Ensure the path exists
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
	if (selectedFolderName.compare("Prefabs") == 0) RenderPrefabsGrid();
	else RenderAssetGrid(selectedFolderPath);
	ImGui::EndChild();

	PrefabEditor::Render();
}

// Renders top-level directories in the Resources folder
void AssetsBrowser::RenderDirectoryTree() {
	const std::string rootPath = "Resources";
	const std::string prefabsPath = rootPath + "/Prefabs";

	// Ensure Prefabs directory exists
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

std::string static NormalizePath(const std::string& path) {
	std::string normalizedPath = path;
	std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
	return normalizedPath;
}

void AssetsBrowser::RenderAssetGrid(const std::string& path) {
	if (!fs::exists(path)) return;

	// Get second directory from path
	std::string mainFolder = "";
	fs::path relativePath = fs::relative(path, "Resources");
	auto SecondFolderit = relativePath.begin();
	if (SecondFolderit != relativePath.end()) ++SecondFolderit;
	if (SecondFolderit != relativePath.end()) mainFolder = SecondFolderit->string();


	// Back button if not in the main folder
	if (!mainFolder.empty() && path != "Resources/" + mainFolder) {
		if (ImGui::Button("< Back")) {
			selectedFolderPath = fs::path(path).parent_path().string();
			selectedFolderName = fs::path(selectedFolderPath).filename().string();
			return;
		}
		ImGui::Separator();
	}

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

	// Render directories first
	Texture Foldertexture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("duck_folder_icon");
	for (const auto& entry : directories) {
		std::string fileName = entry.path().filename().string();
		std::string truncatedFolderName = fileName;
		std::string normalizedPath = NormalizePath(entry.path().string());

		ImGui::PushID(normalizedPath.c_str());
		ImGui::BeginGroup();

		// Load the folder icon		
		if (Foldertexture) {
			ImGui::Image((void*)(intptr_t)Foldertexture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
				selectedFolderPath = normalizedPath;
				selectedFolderName = fileName;
			}
		}

		// Calculate text width and centeralign
		if (truncatedFolderName.length() > 12) truncatedFolderName = truncatedFolderName.substr(0, 9) + "...";
		float textWidth = ImGui::CalcTextSize((truncatedFolderName + "/").c_str()).x;
		float offsetX = (128 - textWidth) * 0.5f;
		if (offsetX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

		// Display the folder name below the icon
		ImGui::TextWrapped("%s", ("/" + truncatedFolderName).c_str());
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
	bool isAssetContextOpen = false;
	// Render files
	for (const auto& entry : files) {
		std::string fileName = entry.path().filename().string();
		std::string truncatedFileName = fileName;
		std::string fileExtension = entry.path().extension().string();
		std::string normalizedPath = NormalizePath(entry.path().string());

		// Convert to lowercase for fileExtension
		for (char& c : fileExtension) {
			c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		}

		// Filter assets based on the search query
		std::string fileNameLower = fileName;
		std::transform(fileNameLower.begin(), fileNameLower.end(), fileNameLower.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		// Skip files that don't match the query
		if (!queryLower.empty() && fileNameLower.find(queryLower) == std::string::npos) {
			continue; 
		}

		// Find the parent directory
		std::string absolutePath = fs::absolute(entry.path()).string();
		std::string parentDir = "";
		for (const auto& [dir, extensions] : folderAllowedExtensions) {
			std::string absoluteParent = fs::absolute("Resources/" + dir).string();
			if (absolutePath.find(absoluteParent) == 0) {
				parentDir = dir;
				break;
			}
		}

		// Find the icon for the file extension
		std::string iconName = "";
		auto Iconit = fileIcons.find(fileExtension);
		if (Iconit != fileIcons.end()) {
			iconName = Iconit->second;
		}

		ImGui::PushID(normalizedPath.c_str());
		ImGui::BeginGroup();
		// Validate file extension based on parent directory
		if (!parentDir.empty()) {
			const auto& allowedExtensions = folderAllowedExtensions[parentDir];
			if (std::find(allowedExtensions.begin(), allowedExtensions.end(), fileExtension) != allowedExtensions.end()) {
				// Render based on parent directory type
				if (parentDir == "Sprites") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(normalizedPath);

					// Check if the texture needs reloading
					auto currentWriteTime = std::filesystem::last_write_time(normalizedPath);
					auto it = fileModificationTimes.find(normalizedPath);
					bool needsReload = (it == fileModificationTimes.end() || it->second != currentWriteTime);
					// Update the modification time
					if (needsReload) {	
						fileModificationTimes[normalizedPath] = currentWriteTime;
						DuckEngine::DUCKENGINE_AssetManager.UnloadTexture(normalizedPath);
						DuckEngine::DUCKENGINE_AssetManager.LoadTexture(normalizedPath);
					}

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

					// Right-click context menu for deleting individual assets
					if (ImGui::BeginPopupContextItem(("##ContextMenu_" + normalizedPath).c_str())) {
						isAssetContextOpen = true;  // Mark that an asset menu is open

						if (ImGui::MenuItem("Delete")) {
							AssetManager::RemoveAsset(normalizedPath);
						}

						ImGui::EndPopup();
					}

					// Open context menu on right-click
					//if (ImGui::BeginPopupContextItem(("Replace##" + normalizedPath).c_str())) {
					//	// Replace the asset with the new file
					//	if (ImGui::MenuItem("Replace texture")) {
					//		std::string newFilePath = LevelManager::OpenFileDialog("texture");
					//		if (!newFilePath.empty()) {
					//			ReplaceAsset(normalizedPath, newFilePath);
					//		}
					//	}
					//	ImGui::EndPopup();
					//}
				}
				else if (parentDir == "Sounds") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName(iconName);

					// Check if the sound needs reloading
					auto currentWriteTime = std::filesystem::last_write_time(normalizedPath);
					auto it = fileModificationTimes.find(normalizedPath);
					bool needsReload = (it == fileModificationTimes.end() || it->second != currentWriteTime);
					// Update the modification time
					if (needsReload) {						
						fileModificationTimes[normalizedPath] = currentWriteTime;
						DuckEngine::DUCKENGINE_AssetManager.ReloadSound(normalizedPath, normalizedPath);
					}

					if (texture) ImGui::Image((void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("SOUND_PAYLOAD", normalizedPath.c_str(), entry.path().string().size() + 1);
						ImGui::Text("Drag %s", fileName.c_str());
						ImGui::EndDragDropSource();
					}

					// Right-click context menu for deleting individual assets
					if (ImGui::BeginPopupContextItem(("##ContextMenu_" + normalizedPath).c_str())) {
						isAssetContextOpen = true;  // Mark that an asset menu is open

						if (ImGui::MenuItem("Delete")) {
							AssetManager::RemoveAsset(normalizedPath);
						}

						ImGui::EndPopup();
					}
				}
				else if (parentDir == "Scenes") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName(iconName);
					if (texture) {
						ImGui::Image((void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
						// Handle doubleclick for loading the scene
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							// Get the file name without extension
							std::string sceneName = entry.path().stem().string(); 
							if (sceneName != DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName())
							{
								LevelManager::LoadLevelEditor(normalizedPath);
								std::cout << "Loading scene: " << sceneName << std::endl;
							}
						}
					}					
				}
				else if (parentDir == "Fonts") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName(iconName);
					if (texture) ImGui::Image((void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));

					// Right-click context menu for deleting individual assets
					if (ImGui::BeginPopupContextItem(("##ContextMenu_" + normalizedPath).c_str())) {
						isAssetContextOpen = true;  // Mark that an asset menu is open

						if (ImGui::MenuItem("Delete")) {
							AssetManager::RemoveAsset(normalizedPath);
						}

						ImGui::EndPopup();
					}
				}
				else if (parentDir == "Shaders") {
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName(iconName);
					if (texture) ImGui::Image((void*)(intptr_t)texture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));

					// Right-click context menu for deleting individual assets
					if (ImGui::BeginPopupContextItem(("##ContextMenu_" + normalizedPath).c_str())) {
						isAssetContextOpen = true;  // Mark that an asset menu is open

						if (ImGui::MenuItem("Delete")) {
							AssetManager::RemoveAsset(normalizedPath);
						}

						ImGui::EndPopup();
					}
				}

				else {
					ImGui::Button(fileName.c_str(), ImVec2(128, 128));
				}

				if(!isAssetContextOpen && ImGui::BeginPopupContextWindow("FolderContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
					if (ImGui::MenuItem("Import Asset")) {
						std::string filePath = LevelManager::OpenFileDialog("All Files (*.*)\0*.*\0");
						if (!filePath.empty()) {
							AssetManager::AddAsset(filePath, selectedFolderPath);
						}
					}
					ImGui::EndPopup();
				}
			}
			else {
				// Invalid file for the folder
				showErrorPopup = true;
				errorMessage = "Error: File '" + fileName + "' has an invalid extension (" + fileExtension + ") for folder '" + parentDir + "'. Please delete the file.";
			}
		}
		// Calculate text width and centeralign
		if (truncatedFileName.length() > 12) truncatedFileName = truncatedFileName.substr(0, 9) + "...";
		float textWidth = ImGui::CalcTextSize((truncatedFileName + "/").c_str()).x;
		float offsetX = (128 - textWidth) * 0.5f;
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
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - buttonWidth);

		if (ImGui::Button("Close")) {
			showErrorPopup = false;
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
	bool isAssetContextOpen = false;

	auto prefabsCopy = PrefabManager::GetAllPrefabs();

	for (const auto& [prefabName, prefab] : prefabsCopy) {
		if (!prefab) {
			std::cerr << "[ERROR] Prefab '" << prefabName << "' is NULL in memory!" << std::endl;
			continue;
		}

		ImGui::PushID(itemIndex);
		ImGui::BeginGroup();

		if (!prefab->texturePath.empty()) {
			if (auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(prefab->texturePath)) {
				ImGui::Image((void*)(intptr_t)(*texture), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			}
			else {
				ImGui::Button(prefabName.c_str(), ImVec2(128, 128));
			}
		}
		else
		{
			ImGui::Button(prefabName.c_str(), ImVec2(128, 128));
		}

		// Drag/drop source for the prefab
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload("PREFAB_PAYLOAD", prefabName.c_str(), prefabName.size() + 1);
			ImGui::Text("Drag %s", prefabName.c_str());
			ImGui::EndDragDropSource();
		}

		// **Check if prefab still exists before showing context menu**
		if (PrefabManager::GetPrefab(prefabName)) {
			if (ImGui::BeginPopupContextItem(("##ContextMenu_" + prefabName).c_str())) {
				isAssetContextOpen = true;

				if (ImGui::MenuItem("Delete")) {
					std::string prefabPath = "Resources/Prefabs/" + prefabName + ".json";
					std::cout << "[DEBUG] Requesting deletion of prefab: " << prefabName << std::endl;

					if (AssetManager::RemoveAsset(prefabPath)) {
						std::cout << "[INFO] Prefab deleted in assetbrowser: " << prefabName << std::endl;
					}
					else {
						std::cerr << "[ERROR] Failed to remove prefab: " << prefabName << std::endl;
					}
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			if (PrefabManager::GetPrefab(prefabName)) {
				PrefabEditor::OpenPrefabEditor(prefabName);
			}
		}

		std::string truncatedPrefabName = prefabName;
		if (truncatedPrefabName.length() > 12) truncatedPrefabName = truncatedPrefabName.substr(0, 9) + "...";
		float textWidth = ImGui::CalcTextSize((truncatedPrefabName + "/").c_str()).x;
		float offsetX = (128 - textWidth) * 0.5f;
		if (offsetX > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
		ImGui::TextWrapped("%s", truncatedPrefabName.c_str());
		ImGui::EndGroup();

		if ((itemIndex + 1) % itemsPerRow != 0) {
			ImGui::SameLine();
		}

		ImGui::PopID();
		itemIndex++;
	}

	// Flag to track whether the popup was triggered
	static bool openCreatePrefabPopup = false;

	// Rightclick context menu for creating a new prefab
	if (!isAssetContextOpen && ImGui::BeginPopupContextWindow("PrefabContextMenu", ImGuiPopupFlags_MouseButtonRight)) {
		if (ImGui::MenuItem("Create Prefab")) {
			openCreatePrefabPopup = true;
		}
		ImGui::EndPopup();
	}

	// Check if the popup should be opened
	if (openCreatePrefabPopup) {
		ImGui::OpenPopup("Create New Prefab");
		openCreatePrefabPopup = false;
	}

	// Center the popup
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	// Handle the popup for creating a new prefab
	if (ImGui::BeginPopupModal("Create New Prefab", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		static char newPrefabName[64] = "";

		// Display input text
		ImGui::PushItemWidth(-1);
		ImGui::InputTextWithHint("##PrefabInput", "Enter prefab name...", newPrefabName, IM_ARRAYSIZE(newPrefabName));
		ImGui::PopItemWidth();
		ImGui::Spacing();

		// Create button
		if (ImGui::Button("Create", ImVec2(100, 0))) {
			if (std::strlen(newPrefabName) > 0) {
				std::string prefabName(newPrefabName);

				// Create and register the new prefab
				std::shared_ptr<Prefab> newPrefab = std::make_shared<Prefab>(prefabName);				
				nlohmann::json defaultTransformComponent = {
					{"type", "TransformComponent"},
					{"properties", {
						{"position", {{"x", 0.0f}, {"y", 0.0f}}},
						{"scale", {{"x", 1.0f}, {"y", 1.0f}}},
						{"rotation", 0.0f}
					}}
				};
				newPrefab->componentsData.push_back(defaultTransformComponent);

				PrefabManager::AddPrefab(prefabName, newPrefab);
				PrefabManager::SavePrefab(prefabName);

				// Open the prefab in the PrefabEditor
				PrefabEditor::OpenPrefabEditor(prefabName);
				std::fill(std::begin(newPrefabName), std::end(newPrefabName), '\0');
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(100, 0))) ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
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
		int entityID = entity.get()->entityID;
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
void AssetsBrowser::HandleFileDrop(GLFWwindow* window, int count, const char** paths) {
	UNREFERENCED_PARAMETER(window);

	for (int i = 0; i < count; ++i) {
		std::string filePath = paths[i];
		std::string extension = fs::path(filePath).extension().string();
		std::string fileName = fs::path(filePath).filename().string();
		std::string absolutePath = fs::absolute(selectedFolderPath).string();
		// Convert to lowercase for fileExtension
		for (char& c : extension) {
			c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		}

		// Determine the parent directory dynamically
		std::string parentDir = "";
		for (const auto& [dir, extensions] : folderAllowedExtensions) {
			std::string absoluteParent = fs::absolute("Resources/" + dir).string();
			if (absolutePath.find(absoluteParent) == 0) {
				parentDir = dir;
				break;
			}
		}

		if (!parentDir.empty()) {
			// Validate file extension
			const auto& allowedExtensions = folderAllowedExtensions.at(parentDir);
			if (std::find(allowedExtensions.begin(), allowedExtensions.end(), extension) == allowedExtensions.end()) {
				// Set the popup state and error message
				showErrorPopup = true;
				errorMessage = "Error: Unsupported file type '" + extension + "' for folder '" + parentDir + "'.";
				continue; // Skip this file
			}
		}
		else {
			// Unknown folder type
			showErrorPopup = true;
			errorMessage = "Error: Unknown folder type for path '" + selectedFolderPath + "'.";
			continue; // Skip this file
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