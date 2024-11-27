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
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + contentRegion.x - 30);
	if (ImGui::Button("...")) ImGui::OpenPopup("ComponentMenu");
	if (ImGui::BeginPopup("ComponentMenu"))
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
	Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
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

		}
	}

	// Render SpriteRendererComponent if it exists
	if (auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Sprite Renderer Component"))
		{
			ImGui::Checkbox("Use Color", &spriteRenderer->useColor);
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

			// Display the current texture as a preview if it exists
			if (!spriteRenderer->texturePath.empty()) {
				auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(spriteRenderer->texturePath);
				if (texture) {
					ImGui::Text("Current Texture:");
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
			if (ImGui::DragFloat2("##Size", &size.x, 0.1f, 0.1f, 10000.0f)) hasChanged = true;

			ImGui::Text("Rotation");
			ImGui::SameLine(100);
			if (ImGui::DragFloat("##Rotation", &box->rotation, 1.0f, 0.0f, 360.0f)) hasChanged = true;

			ImGui::Text("Offset");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##Offset", &Offset.x, 0.1f, -10000.0f, 10000.0f)) hasChanged = true;

			// Update component with modified values
			box->setCenter(center);
			box->setSize(size);
			box->setOffSet(Offset);
			//box->setRotation(box->rotation);

			// Remove component button
			ComponentMenu<BoundingBox>(entityID);
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
			if (ImGui::DragFloat("##Radius", &radius, 0.1f, 0.0f, 360.0f)) hasChanged = true;

			ImGui::Text("Offset");
			ImGui::SameLine(100);
			if (ImGui::DragFloat2("##Offset", &Offset.x, 0.1f, -10000.0f, 10000.0f)) hasChanged = true;

			// Update component with modified values
			circle->setCenter(center);
			circle->setRadius(radius);
			circle->setOffSet(Offset);

			// Remove component button
			ComponentMenu<BoundingCircle>(entityID);
		}
	}

	// Render AnimatorComponent if it exists
	if (auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entityID))
	{
		if (ImGui::CollapsingHeader("Animator Component"))
		{
			if (animator->currentAnimation)
			{
				ImGui::Text("Current Animation: %s", animator->currentAnimation->name.c_str());
			}

			if (ImGui::Button("Play"))
			{
				if (animator->currentAnimation)
				{
					animator->PlayAnimation(animator->currentAnimation->name);
					hasChanged = true;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				animator->Pause();
				hasChanged = true;
			}

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
			
			// Display current sound path, if it exists
			if (!sound->soundID.empty()) {
				ImGui::Text("Current Sound: %s", sound->soundID.c_str());
			}
			else {
				ImGui::Text("Current Sound: None");
			}

			// Set up a drop target for audio files
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SOUND_PAYLOAD")) {
					const char* newSoundPath = static_cast<const char*>(payload->Data);

					// Check if the dropped file has an allowed extension
					if (IsAllowedExtension(newSoundPath, allowedSoundExtensions)) {
						std::cout << "Sound file dropped: " << newSoundPath << std::endl;

						// Update the sound component's file path
						sound->soundID = newSoundPath;
						DuckEngine::DUCKENGINE_AssetManager.LoadSound(sound->soundID, newSoundPath);

						hasChanged = true;
						std::cout << "Sound file set to: " << sound->soundID << std::endl;
					}
					else {
						std::cerr << "Error: Only OGG, MP3, and WAV audio files are allowed." << std::endl;
					}
				}
				ImGui::EndDragDropTarget();
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
			if (ImGui::DragFloat2("##Position", &text->position.x, 0.1f, -10000.0f, 10000.0f)) {
				hasChanged = true;
			}

			// Font size
			ImGui::Text("Font Size");
			ImGui::SameLine(100);
			if (ImGui::DragInt("##FontSize", &text->fontSize, 1, 1, 1000)) {
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

			// Position
			ImGui::Text("Position");
			ImGui::SameLine(100);
			Vec2 position = button->minPos;
			if (ImGui::DragFloat2("##Position", &position.x, 0.1f, -10000.0f, 10000.0f)) {
				Vec2 size = button->maxPos - button->minPos;
				button->minPos = position;
				button->maxPos = position + size;
				hasChanged = true;
			}

			// Size
			ImGui::Text("Size");
			ImGui::SameLine(100);
			Vec2 size = button->maxPos - button->minPos;
			if (ImGui::DragFloat2("##Size", &size.x, 0.1f, 0.1f, 10000.0f)) {
				button->maxPos = button->minPos + size;
				hasChanged = true;
			}

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
	// Add a separator and a dropdown to add new components
	ImGui::Separator();
	ImGui::Text("Add Component");

	// Dropdown for selecting components to add
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

	// Button to add the selected component
	if (ImGui::Button("Add Component")) {
		AddComponentToEntity(componentTypes[selectedComponentIndex], entityID);
		hasChanged = true;
	}
}

std::vector<std::string> InspectorRenderer::GetComponentTypes()
{
	return componentTypes;
}

bool InspectorRenderer::IsAllowedExtension(const std::string& filePath, const std::unordered_set<std::string>& allowedExtensions) {
	// Extract the file extension
	std::string extension = filePath.substr(filePath.find_last_of('.') + 1);

	// Convert extension to lowercase for case-insensitive comparison
	std::transform(extension.begin(), extension.end(), extension.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });

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
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(entityID);
	}
	else if (componentName == "BoundingCircle" && !DuckEngine::DUCKENGINE_ComponentManager.HasComponent<BoundingCircle>(entityID)) {
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingCircle>(entityID);
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
