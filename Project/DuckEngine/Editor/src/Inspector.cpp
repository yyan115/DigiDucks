/******************************************************************************/
/*!
\file     Inspector.cpp
\author   Lucas Yee JunJie, l.yee, 2301212 (50%)
		  Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (50%)
\par      l.yee@digipen.edu, muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the InspectorRenderer class
		  which is responsible for rendering the inspector UI in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <iostream>
#include "Inspector.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "RigidbodyComponent.h"
#include "Bounding.h"
#include "AnimatorComponent.h"
#include "SoundComponent.h"
#include "DuckEngine.h"
#include "GameManager.h"
#include "SnapshotManager.h"
#include "AnimationEditor.h"


std::unordered_map<int, bool> InspectorRenderer::entityChanges;
static const std::unordered_set<std::string> allowedImageExtensions = { "png", "jpg", "jpeg" };
static const std::unordered_set<std::string> allowedSoundExtensions = { "ogg", "wav", "mp3" };

// Available component types
const std::vector<std::string> InspectorRenderer::componentTypes = {
	"TransformComponent",
	"SpriteRendererComponent",
	"RigidbodyComponent",
	"BoundingBox",
	"BoundingCircle",
	"AnimatorComponent",
	"SoundComponent",
	"TextComponent",
	"ButtonComponent",
	"GameLogicComponent"
};

bool isEditing = false;


template <typename ComponentName>
void ComponentMenu(int entityID)
{
	std::string popupId = std::string("ComponentMenu_") + typeid(ComponentName).name();

	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + contentRegion.x - 30);

	if (ImGui::Button(("...##" + popupId).c_str()))
		ImGui::OpenPopup(popupId.c_str());

	if (ImGui::BeginPopup(popupId.c_str()))
	{
		if (ImGui::MenuItem("Remove Component"))
		{
			DuckEngine::DUCKENGINE_ComponentManager.RemoveComponent<ComponentName>(entityID);
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			return;
		}
		ImGui::EndPopup();
	}
}

void InspectorRenderer::RenderLayer(int entityID)
{
	if (entityID == -1) {
		ImGui::Text("No entity selected.");
		return;
	}

	// Get the current entity and scene
	Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID).get();
	auto* activeScene = DuckEngine::DUCKENGINE_SceneManager.GetActiveScene();
	if (!entity || !activeScene) return;

	// Retrieve the list of layer names from the active scene
	const auto& layers = activeScene->GetLayers();
	std::vector<std::string> layerNames;
	for (const auto& [layerName, layer] : layers) {
		layerNames.push_back(layerName);
	}

	// Find the current layer index
	int currentLayerIndex = 0;
	for (size_t i = 0; i < layerNames.size(); ++i) {
		if (layerNames[i] == entity->layerName) {
			currentLayerIndex = static_cast<int>(i);
			break;
		}
	}

	ImGui::Text("Layer");
	ImGui::SameLine();

	ImGui::PushID(entityID);
	if (ImGui::BeginCombo("##LayerCombo", layerNames[currentLayerIndex].c_str()))
	{
		for (int i = 0; i < layerNames.size(); ++i)
		{
			bool isSelected = (currentLayerIndex == i);
			if (ImGui::Selectable(layerNames[i].c_str(), isSelected))
			{
				currentLayerIndex = i;
				entity->layerName = layerNames[i];  // Update entity layer
				LevelManager::SaveEntityChanges(entityID, GameManager::ActiveSceneName);
			}

			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
	ImGui::Separator();
}


void InspectorRenderer::RenderComponents(int entityID)
{
	if (entityID == -1) {
		return;
	}

	// Use reference to track changes for the current entity
	bool& hasChanged = entityChanges[entityID];

	// Render TransformComponent if it exists
	if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Transform Component"))
		{
			// Position
			ImGui::Text("Position");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##Position", &transform->GetPosition().x, 0.1f, -10000.0f, 10000.0f)) {
				transform->SetPosition(transform->GetPosition());
				hasChanged = true;
				if (!isEditing)
				{
					SnapshotManager::SaveUndoState();
					isEditing = true;
				}
			}

			// Rotation
			ImGui::Text("Rotation");
			ImGui::SameLine(100);
			if (ImGui::DragFloat("##Rotation", &transform->angle, 1.0f, 0.0f, 360.0f)) 
			{
				hasChanged = true;
				if (!isEditing)
				{
					SnapshotManager::SaveUndoState();
					isEditing = true;
				}
			}

			// Scale
			ImGui::Text("Scale");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##Scale", &transform->scale.x, 0.1f, 0.1f, 10000.0f))
			{
				hasChanged = true;
				if (!isEditing)
				{
					SnapshotManager::SaveUndoState();
					isEditing = true;
				}
			}

			ImGui::Checkbox("Relative To Camera", &transform->relativeToCamera);
			if (ImGui::IsItemEdited()) hasChanged = true;

		}
	}

	// Render SpriteRendererComponent if it exists
	if (auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Sprite Renderer Component"))
		{
			ImGui::Checkbox("Use Color", &spriteRenderer->useColor);
			if (ImGui::IsItemEdited()) hasChanged = true;

			ImGui::Checkbox("Visible", &spriteRenderer->isVisible);
			if (ImGui::IsItemEdited()) hasChanged = true;

			// Normalize the color values
			float color[4] = {
				spriteRenderer->color.r / 255.0f,
				spriteRenderer->color.g / 255.0f,
				spriteRenderer->color.b / 255.0f,
				spriteRenderer->color.a / 255.0f
			};

			// Display color picker
			if (ImGui::ColorEdit4("Color", color))
			{
				// Denormalize the values
				spriteRenderer->color.r = color[0] * 255.0f;
				spriteRenderer->color.g = color[1] * 255.0f;
				spriteRenderer->color.b = color[2] * 255.0f;
				spriteRenderer->color.a = color[3] * 255.0f;
				hasChanged = true;
			}

			ImGui::Text("Sorting Order");
			ImGui::SameLine(110);
			ImGui::PushItemWidth(130);
			if (ImGui::InputInt("##SortingOrder", &spriteRenderer->sortingOrder))
			{
				hasChanged = true;
			}
			ImGui::PopItemWidth();

			// Display the current texture as a preview if it exists
			if (!spriteRenderer->texturePath.empty()) {
				auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(spriteRenderer->texturePath);
				if (texture) {
					ImGui::Text("Current Texture:");
					ImGui::Text(spriteRenderer->texturePath.c_str());
					ImGui::Image((void*)(intptr_t)*texture, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
				}
			}
			else {
				ImGui::Text("Current Texture:");
				ImGui::Button("Empty", ImVec2(64, 64));
			}

			// Set up a drop target for textures
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_PAYLOAD")) {
					const char* newTexturePath = static_cast<const char*>(payload->Data);

					// Only accept files with allowed extensions
					if (IsAllowedExtension(newTexturePath, allowedImageExtensions)) {
						std::cout << "Texture dropped: " << newTexturePath << std::endl;

						// Update spriteRenderer texture path
						spriteRenderer->texturePath = newTexturePath;
						auto newTexture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(spriteRenderer->texturePath);

						// Load texture if not already loaded
						if (!newTexture) {
							DuckEngine::DUCKENGINE_AssetManager.LoadTexture(spriteRenderer->texturePath);
							newTexture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(spriteRenderer->texturePath);
						}

						if (newTexture) {
							spriteRenderer->texture = *newTexture;
							hasChanged = true;
							std::cout << "Texture replaced with: " << spriteRenderer->texturePath << std::endl;
						}
						else {
							std::cerr << "Error: Texture could not be loaded from " << spriteRenderer->texturePath << std::endl;
						}
					}
					else {
						std::cerr << "Error: Only PNG and JPG image files are allowed." << std::endl;
					}
				}
				ImGui::EndDragDropTarget();
			}

			ComponentMenu<SpriteRendererComponent>(entityID);
		}
	}

	// Render RigidbodyComponent if it exists
	if (auto* rb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Rigidbody Component"))
		{
			ImGui::Checkbox("Is Static", &rb->isStatic);
			if (ImGui::IsItemEdited()) hasChanged = true;

			// Remove component button
			ComponentMenu<RigidbodyComponent>(entityID);
		}

		
	}

	// Render BoundingBox if it exists
	if (auto* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityID))
	{
		if (ImGui::CollapsingHeader("Bounding Box"))
		{
			Vec2 center = box->getCenter();
			Vec2 size = box->getSize();
			Vec2 Offset = box->getOffSet();

			ImGui::Checkbox("Is Kinematic", &box->isKinematic);
			if (ImGui::IsItemEdited()) hasChanged = true;

			ImGui::Text("Size");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##BoundingBoxSize", &size.x, 0.1f, 0.1f, 10000.0f)) hasChanged = true;

			ImGui::Text("Rotation");
			ImGui::SameLine(100);
			if (ImGui::DragFloat("##BoundingBoxRotation", &box->rotation, 1.0f, 0.0f, 360.0f)) hasChanged = true;

			ImGui::Text("Offset");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##BoundingBoxOffset", &Offset.x, 0.1f, -10000.0f, 10000.0f)) hasChanged = true;

			// Update component with modified values
			box->setCenter(center);
			box->setSize(size);
			box->setOffSet(Offset);
			//box->setRotation(box->rotation);

			box->showDebugCollider = true;

			// Remove component button
			ComponentMenu<BoundingBox>(entityID);
		}

		else
		{
			box->showDebugCollider = false;
		}
		
	}

	// Render BoundingCircle if it exists
	if (auto* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityID))
	{
		if (ImGui::CollapsingHeader("Bounding Circle"))
		{
			Vec2 center = circle->getCenter();
			float radius = circle->getRadius();
			Vec2 Offset = circle->getOffSet();

			ImGui::Checkbox("Is Kinematic", &circle->isKinematic);
			if (ImGui::IsItemEdited()) hasChanged = true;

			ImGui::Text("Radius");
			ImGui::SameLine(100);
			if (ImGui::DragFloat("##BoundingCircleRadius", &radius, 0.1f, 0.0f, 360.0f)) hasChanged = true;

			ImGui::Text("Offset");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##BoundingCircleOffset", &Offset.x, 0.1f, -10000.0f, 10000.0f)) hasChanged = true;

			// Update component with modified values
			circle->setCenter(center);
			circle->setRadius(radius);
			circle->setOffSet(Offset);

			circle->showDebugCollider = true;

			// Remove component button
			ComponentMenu<BoundingCircle>(entityID);
		}
		else
		{
			circle->showDebugCollider = false;
		}
	}

	// Render AnimatorComponent if it exists
	if (auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Animator Component"))
		{
			ImGui::Text("Animation List");


			// List all available animations
			for (const auto& [name, animation] : animator->animations)
			{
				if (ImGui::Selectable(name.c_str(), animator->currentAnimation && animator->currentAnimation->name == name))
				{
					animator->SetAnimation(name);
					hasChanged = true;
				}
			}

			// button to open the Animation Editor
			if (ImGui::Button("Open Animation Editor"))
			{
				AnimationEditor::Open(entityID);
			}

			// Remove component button
			ComponentMenu<AnimatorComponent>(entityID);
		}

		
	}

	// Render SoundComponent if it exists
	if (auto* sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Sound Component"))
		{
			// Checkbox for looping
			ImGui::Checkbox("Loop", &sound->loop);
			if (ImGui::IsItemEdited()) hasChanged = true;

			// Checkbox for playing on start
			ImGui::Checkbox("Play on Start", &sound->playOnStart);
			if (ImGui::IsItemEdited()) hasChanged = true;
			
			// Slider for volume
			ImGui::Text("Volume");
			ImGui::SameLine(100);
			if (ImGui::DragFloat("##Volume", &sound->volume, 0.01f, 0.0f, 1.0f)) hasChanged = true;
			
			// Display sound paths
			ImGui::Text("Sound Paths:");
			for (size_t i = 0; i < sound->soundID.size(); ++i) {
				ImGui::PushID(static_cast<int>(i));

				// Show current sound path
				ImGui::Text("Sound %d:", static_cast<int>(i + 1));
				ImGui::SameLine();
				if (!sound->soundID[i].empty()) {
					ImGui::Text(sound->soundID[i].c_str());
				}
				else {
					ImGui::Text("None");
				}

				// Drag-and-drop for each sound
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SOUND_PAYLOAD")) {
						const char* newSoundPath = static_cast<const char*>(payload->Data);

						// Check if the dropped file has an allowed extension
						if (IsAllowedExtension(newSoundPath, allowedSoundExtensions)) {
							std::cout << "Sound file dropped: " << newSoundPath << std::endl;

							// Update the sound component's file path
							sound->soundID[i] = newSoundPath;
							DuckEngine::DUCKENGINE_AssetManager.LoadSound(sound->soundID[i], newSoundPath);

							hasChanged = true;
							std::cout << "Sound file set to: " << sound->soundID[i] << std::endl;
						}
						else {
							std::cerr << "Error: Only OGG, MP3, and WAV audio files are allowed." << std::endl;
						}
					}
					ImGui::EndDragDropTarget();
				}

				// Remove button for individual sounds
				ImGui::SameLine();
				if (ImGui::Button("Remove")) {
					sound->soundID.erase(sound->soundID.begin() + i);
					hasChanged = true;
					ImGui::PopID();
					break;
				}

				ImGui::PopID();
			}

			// "Add New" button to add a new sound
			if (ImGui::Button("Add New")) {
				sound->soundID.push_back(""); // Add a new empty sound slot
				hasChanged = true;
			}

			// Category dropdown
			const std::vector<std::string> categories = { "Default", "BGM", "SFX", "UI" };
			static int currentCategoryIndex = 0;

			// Update index to match the current category
			for (size_t i = 0; i < categories.size(); ++i) {
				if (categories[i] == sound->category) {
					currentCategoryIndex = static_cast<int>(i);
					break;
				}
			}

			ImGui::Text("Category");
			ImGui::SameLine(100);
			if (ImGui::BeginCombo("##Category", categories[currentCategoryIndex].c_str())) {
				for (size_t i = 0; i < categories.size(); ++i) {
					bool isSelected = (currentCategoryIndex == static_cast<int>(i));
					if (ImGui::Selectable(categories[i].c_str(), isSelected)) {
						currentCategoryIndex = static_cast<int>(i);
						sound->category = categories[i];
						hasChanged = true;
					}
					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// Remove component button
			ComponentMenu<SoundComponent>(entityID);
		}      
		
	}

	// Render TextComponent if it exists
	if (auto* text = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(entityID)) {
		if (ImGui::CollapsingHeader("Text Component")) {
			// Find all font names
			static int currentFontIndex = 0;
			const std::vector<std::string>& Fonts = DuckEngine::DUCKENGINE_AssetManager.GetFontNames();
			if (currentFontIndex >= Fonts.size() || Fonts[currentFontIndex] != text->fontName) {
				auto it = std::find(Fonts.begin(), Fonts.end(), text->fontName);
				currentFontIndex = (it != Fonts.end()) ? static_cast<int>(it - Fonts.begin()) : 0;
			}
			// Font name dropdown
			ImGui::Text("Font Name");
			ImGui::SameLine(100);
			if (ImGui::BeginCombo("##FontNameCombo", Fonts[currentFontIndex].c_str())) {
				for (int i = 0; i < Fonts.size(); ++i) {
					bool isSelected = (currentFontIndex == i);
					if (ImGui::Selectable(Fonts[i].c_str(), isSelected)) {
						currentFontIndex = i;
						text->fontName = Fonts[i]; // Update the font name
						hasChanged = true;
					}
					if (isSelected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// Text content input
			static std::vector<char> textBuffer(256);
			if (textBuffer.size() < text->text.size() + 1) textBuffer.resize(text->text.size() + 1);
			strcpy_s(textBuffer.data(), textBuffer.size(), text->text.c_str());
			ImGui::Text("Text");
			ImGui::SameLine(100);
			if (ImGui::InputText("##Text", textBuffer.data(), textBuffer.size())) {
				text->text = std::string(textBuffer.data());
				hasChanged = true;
			}

			// Position
			ImGui::Text("Position");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##TextPosition", &text->position.x, 0.1f, -10000.0f, 10000.0f)) {
				hasChanged = true;
			}

			// Font size
			ImGui::Text("Font Size");
			ImGui::SameLine(100);
			if (ImGui::DragFloat("##FontSize", &text->fontSize, 0.1f, 0.1f, 1000.f)) {
				hasChanged = true;
			}

			// Color input
			ImGui::Text("Color");
			ImGui::SameLine(100);
			float color[4] = {
				text->color.r / 255.0f,
				text->color.g / 255.0f,
				text->color.b / 255.0f,
				text->color.a / 255.0f
			};
			if (ImGui::ColorEdit4("##Color", color)) {
				text->color.r = color[0] * 255.0f;
				text->color.g = color[1] * 255.0f;
				text->color.b = color[2] * 255.0f;
				text->color.a = color[3] * 255.0f;
				hasChanged = true;
			}

			// Enabled checkbox
			if (ImGui::Checkbox("Enabled", &text->isEnabled)) {
				hasChanged = true;
			}

			// Component menu for removal
			ComponentMenu<TextComponent>(entityID);
		}
	}

	// Render ButtonComponent if it exists
	if (auto* button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(entityID)) {
		if (ImGui::CollapsingHeader("Button Component")) {
			// Enabled checkbox
			if (ImGui::Checkbox("Enabled", &button->isEnabled)) {
				hasChanged = true;
			}

			// Component menu for removal
			ComponentMenu<ButtonComponent>(entityID);
		}
	}

	if (auto* gameLogicComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<GameLogicComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Game Logic Component"))
		{
			ImGui::Text("Attached Logic Scripts:");

			// Display all logic names
			for (size_t i = 0; i < gameLogicComponent->logicNames.size(); ++i)
			{
				ImGui::Text("%s", gameLogicComponent->logicNames[i].c_str());
				ImGui::SameLine();

				// Add a remove button for each logic
				if (ImGui::Button(("Remove##" + std::to_string(i)).c_str()))
				{
					gameLogicComponent->RemoveLogic(gameLogicComponent->logicNames[i]);
					hasChanged = true;
				}
			}

			ImGui::Separator();

			// Dropdown for adding a new logic script
			static std::vector<std::string> availableLogicFiles;
			static bool logicFilesLoaded = false;
			if (!logicFilesLoaded)
			{
				std::string logicFolder = "Game/include/";
				for (const auto& entry : std::filesystem::directory_iterator(logicFolder))
				{
					if (entry.is_regular_file())
					{
						std::string fileName = entry.path().filename().string();
						if (fileName.size() >= 7 && fileName.substr(fileName.size() - 7) == "Logic.h")
						{
							std::string logicName = fileName.substr(0, fileName.size() - 2);
							availableLogicFiles.push_back(logicName);
						}
					}
				}
				logicFilesLoaded = true;
			}

			static int selectedLogicIndex = 0;
			if (ImGui::BeginCombo("##Available Logic Files", availableLogicFiles[selectedLogicIndex].c_str()))
			{
				for (int i = 0; i < availableLogicFiles.size(); ++i)
				{
					bool isSelected = (selectedLogicIndex == i);
					if (ImGui::Selectable(availableLogicFiles[i].c_str(), isSelected))
					{
						selectedLogicIndex = i;
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Add Logic"))
			{
				const std::string& selectedLogic = availableLogicFiles[selectedLogicIndex];
				if (std::find(gameLogicComponent->logicNames.begin(), gameLogicComponent->logicNames.end(), selectedLogic) == gameLogicComponent->logicNames.end())
				{
					gameLogicComponent->AddLogic(selectedLogic);
					hasChanged = true;
				}
			}

			ComponentMenu<GameLogicComponent>(entityID);
		}
	}

	if (isEditing && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		//SnapshotManager::SaveUndoState(); 
		isEditing = false;
	}
   

	// Display Save and Overwrite buttons if changes were detected
	if (hasChanged)
	{
		if (ImGui::Button("Save Entity Changes"))
		{
			LevelManager::SaveEntityChanges(entityID, GameManager::ActiveSceneName);
			hasChanged = false; 
		}
		ImGui::SameLine();
		if (ImGui::Button("Overwrite Prefab"))
		{
			LevelManager::OverwritePrefab(entityID);
			LevelManager::SaveSceneChanges(GameManager::ActiveSceneName);
			DuckEngine::DUCKENGINE_SceneManager.ReloadScene();
			hasChanged = false; 
		}
	}

	AddComponents(entityID, hasChanged);
}

void InspectorRenderer::AddComponents(int entityID, bool& hasChanged)
{
	ImGui::Separator();
	ImGui::Text("Add Component");

	static int selectedComponentIndex = 0;
	if (ImGui::BeginCombo("##AddComponent", componentTypes[selectedComponentIndex].c_str())) {
		for (int i = 0; i < componentTypes.size(); i++) {
			bool isSelected = (selectedComponentIndex == i);
			if (ImGui::Selectable(componentTypes[i].c_str(), isSelected)) {
				selectedComponentIndex = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// Check if the selected component already exists
	bool componentExists = false;
	const std::string& selectedType = componentTypes[selectedComponentIndex];

	if (selectedType == "TransformComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<TransformComponent>(entityID);
	else if (selectedType == "SpriteRendererComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SpriteRendererComponent>(entityID);
	else if (selectedType == "RigidbodyComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<RigidbodyComponent>(entityID);
	else if (selectedType == "BoundingBox")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<BoundingBox>(entityID);
	else if (selectedType == "BoundingCircle")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<BoundingCircle>(entityID);
	else if (selectedType == "AnimatorComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<AnimatorComponent>(entityID);
	else if (selectedType == "SoundComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SoundComponent>(entityID);
	else if (selectedType == "TextComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<TextComponent>(entityID);
	else if (selectedType == "ButtonComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<ButtonComponent>(entityID);
	else if (selectedType == "GameLogicComponent")
		componentExists = DuckEngine::DUCKENGINE_ComponentManager.HasComponent<GameLogicComponent>(entityID);

	// Disable the Add Component button if component exists
	if (componentExists)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::Button("Add Component");
		ImGui::PopStyleVar();

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("This component type already exists on the entity");
		}
	}

	else if (ImGui::Button("Add Component")) {
		AddComponentToEntity(componentTypes[selectedComponentIndex], entityID);
		hasChanged = true;
	}
}

std::vector<std::string> InspectorRenderer::GetComponentTypes()
{
	return componentTypes;
}

std::unordered_set<std::string> InspectorRenderer::GetAllowedImageExtensions()
{
	return allowedImageExtensions;
}

bool InspectorRenderer::IsAllowedExtension(const std::string& filePath, const std::unordered_set<std::string>& allowedExtensions) {
	// Convert to lowercase for fileExtension
	std::string extension = filePath.substr(filePath.find_last_of('.') + 1);
	for (char& c : extension) {
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	}

	// Check if the extension is in the allowed set
	return allowedExtensions.find(extension) != allowedExtensions.end();
}

void InspectorRenderer::AddComponentToEntity(const std::string& componentName, int entityID)
{
	if (componentName == "TransformComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<TransformComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entityID);
	}
	else if (componentName == "SpriteRendererComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SpriteRendererComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entityID);
	}
	else if (componentName == "RigidbodyComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<RigidbodyComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<RigidbodyComponent>(entityID);
	}
	else if (componentName == "BoundingBox" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<BoundingBox>(entityID)) {
		auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID);
		if (transform) 
		{
			Vec2 position = transform->GetPosition();
			Vec2 size = transform->scale;
			auto entityBox = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(entityID, position, size);

			Vec2 initialSize = entityBox->getInitialSize();
			Vec2 scaledSize = Vec2(initialSize.x * transform->scale.x,
				initialSize.y * transform->scale.y);
			entityBox->setSize(scaledSize);
		}
		else {
			DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(entityID);
		}
	}
	else if (componentName == "BoundingCircle" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<BoundingCircle>(entityID)) {
		auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID);
		if (transform) 
		{
			Vec2 position = transform->GetPosition();
			auto entityCircle = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingCircle>(entityID, position, 0.5f);

			float averageScale = (transform->scale.x + transform->scale.y) * 0.5f;
			entityCircle->setRadius(entityCircle->getInitialRadius() * averageScale);

		}
		else {
			DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingCircle>(entityID);
		}
	}
	else if (componentName == "AnimatorComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<AnimatorComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<AnimatorComponent>(entityID);
	}
	else if (componentName == "SoundComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<SoundComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SoundComponent>(entityID);
	}
	else if (componentName == "TextComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<TextComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TextComponent>(entityID);
	}
	else if (componentName == "ButtonComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<ButtonComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<ButtonComponent>(entityID);
	}
	else if (componentName == "GameLogicComponent" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<GameLogicComponent>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<GameLogicComponent>(entityID);
	}
}
