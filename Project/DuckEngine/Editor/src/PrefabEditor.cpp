/******************************************************************************/
/*!
\file       PrefabEditor.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Implements the PrefabEditor class, which provides a graphical
			interface for editing prefab objects in the game engine. This
			includes functionality for rendering prefab properties,
			components, and previewing prefabs.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "PrefabEditor.h"
#include "DuckEngine.h"
#include "GameManager.h"
#include <imgui_internal.h>
#include "ImageLoader.h"
#include "Inspector.h"
#include "AnimationEditor.h"
#include "PrefabManager.h"

bool PrefabEditor::isOpen = false;
std::string PrefabEditor::currentPrefabName = "";
std::shared_ptr<Prefab> PrefabEditor::currentPrefab = nullptr;
std::vector<int> componentsToRemove;

int selectedComponentIndex = 0;

/**************************************************************************
* @brief Opens the Prefab Editor for the specified prefab.
* @param prefabName The name of the prefab to edit.
**************************************************************************/
void PrefabEditor::OpenPrefabEditor(const std::string& prefabName)
{
	currentPrefabName = prefabName;
	currentPrefab = PrefabManager::GetPrefab(prefabName);
	isOpen = true;
}

/**************************************************************************
* @brief Renders the Prefab Editor window and its components.
**************************************************************************/
void PrefabEditor::Render()
{
	if (!isOpen)
		return;

	ImGui::SetNextWindowSize(ImVec2(1000, 500), ImGuiCond_Appearing);

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(
		ImVec2(viewport->Pos.x + viewport->Size.x * 0.5f, viewport->Pos.y + viewport->Size.y * 0.5f),
		ImGuiCond_Appearing,
		ImVec2(0.5f, 0.5f)
	);

	bool windowOpen = true;
	if (ImGui::Begin(("Prefab Editor - " + currentPrefabName).c_str(), &windowOpen, ImGuiWindowFlags_NoCollapse))
	{
		if (currentPrefab)
		{
			ImGui::Columns(2, nullptr, true);

			if (ImGui::BeginChild("PrefabPreview", ImVec2(0, 0), true, ImGuiWindowFlags_NoCollapse))
			{
				RenderPrefabPreview();
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			if (ImGui::BeginChild("PrefabProperties", ImVec2(0, 0), true, ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Text("Editing Prefab: %s", currentPrefabName.c_str());
				RenderPrefabProperties();

				if (ImGui::Button("Save Prefab"))
				{
					PrefabManager::SavePrefab(currentPrefabName);
					LevelManager::SaveSceneChanges(GameManager::ActiveSceneName);
					DuckEngine::DUCKENGINE_SceneManager.ReloadScene();
					currentPrefab = PrefabManager::GetPrefab(currentPrefabName);
				}

				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					isOpen = false;
				}
			}
			ImGui::EndChild();

			ImGui::Columns(1);
		}
		else
		{
			ImGui::Text("Prefab not found!");
			if (ImGui::Button("Close"))
			{
				isOpen = false;
			}
		}
	}
	ImGui::End();

	if (!windowOpen)
		isOpen = false;
}

/**************************************************************************
* @brief Renders the properties of the selected prefab, including components.
**************************************************************************/
void PrefabEditor::RenderPrefabProperties()
{
	if (!currentPrefab || !currentPrefab->componentsData.is_array())
	{
		ImGui::Text("Invalid prefab data or components are missing!");
		return;
	}

	componentsToRemove.clear();

	int index = 0;
	for (auto& componentJson : currentPrefab->componentsData)
	{
		if (!componentJson.contains("type") || !componentJson.contains("properties"))
		{
			ImGui::Text("Invalid component format!");
			index++;
			continue;
		}

		std::string type = componentJson["type"];
		auto& properties = componentJson["properties"];

		if (type == "TransformComponent")
		{
			if (ImGui::CollapsingHeader("Transform Component"))
			{
				float pos[2] = {
					properties["position"].value("x", 0.0f),
					properties["position"].value("y", 0.0f)
				};

				float scl[2] = {
					properties["scale"].value("x", 1.0f),
					properties["scale"].value("y", 1.0f)
				};

				float rotation = properties.value("rotation", 0.0f);

				ImGui::Text("Position");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##Position", pos, 0.1f))
				{
					properties["position"]["x"] = pos[0];
					properties["position"]["y"] = pos[1];
				}

				ImGui::Text("Scale");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##Scale", scl, 0.1f))
				{
					properties["scale"]["x"] = scl[0];
					properties["scale"]["y"] = scl[1];
				}

				ImGui::Text("Rotation");
				ImGui::SameLine(100);
				if (ImGui::DragFloat("##Rotation", &rotation, 1.0f, -360.0f, 360.0f))
				{
					properties["rotation"] = rotation;
				}

				bool relativeToCamera = properties.value("relativeToCamera", false);
				if (ImGui::Checkbox("Relative To Camera", &relativeToCamera))
				{
					properties["relativeToCamera"] = relativeToCamera;
				}

				ComponentMenu(index);
			}
			index++;
		}


		else if (type == "SpriteRendererComponent")
		{
			if (ImGui::CollapsingHeader("Sprite Renderer Component"))
			{
				bool useColor = properties.value("useColor", false);
				ImVec4 color = {
					properties["color"].value("r", 255.0f) / 255.0f,
					properties["color"].value("g", 255.0f) / 255.0f,
					properties["color"].value("b", 255.0f) / 255.0f,
					properties["color"].value("a", 255.0f) / 255.0f
				};

				int sortingOrder = properties.value("sortingOrder", 0);
				std::string texturePath = properties.value("texture", "");

				if (ImGui::Checkbox("Use Color", &useColor))
				{
					properties["useColor"] = useColor;
				}


				bool isVisible = properties.value("isVisible", true);
				if (ImGui::Checkbox("Visible", &isVisible))
				{
					properties["isVisible"] = isVisible;
				}

				if (ImGui::ColorEdit4("Color", &color.x))
				{
					properties["color"]["r"] = static_cast<int>(color.x * 255);
					properties["color"]["g"] = static_cast<int>(color.y * 255);
					properties["color"]["b"] = static_cast<int>(color.z * 255);
					properties["color"]["a"] = static_cast<int>(color.w * 255);
				}

				ImGui::Text("Sorting Order");
				ImGui::SameLine(120);
				if (ImGui::InputInt("##SortingOrder", &sortingOrder))
				{
					properties["sortingOrder"] = sortingOrder;
				}

				if (!texturePath.empty())
				{
					auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(texturePath);
					if (texture)
					{
						ImGui::Text("Current Texture:");
						ImGui::Text(texturePath.c_str());
						ImGui::Image((void*)(intptr_t)*texture, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
					}
				}
				else
				{
					ImGui::Text("Current Texture:");
					ImGui::Button("Empty", ImVec2(64, 64));
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_PAYLOAD"))
					{
						const char* newTexturePath = static_cast<const char*>(payload->Data);
						if (texturePath != newTexturePath)
						{
							properties["texture"] = newTexturePath;
							texturePath = newTexturePath;
						}
					}
					ImGui::EndDragDropTarget();
				}

				ComponentMenu(index);
			}

			index++;
		}
		else if (type == "RigidbodyComponent")
		{
			if (ImGui::CollapsingHeader("Rigidbody Component"))
			{
				bool isStatic = properties.value("isStatic", false);

				if (ImGui::Checkbox("Is Static", &isStatic))
				{
					properties["isStatic"] = isStatic;
				}

				ComponentMenu(index);
			}

			index++;
		}
		else if (type == "BoundingBox")
		{
			if (ImGui::CollapsingHeader("Bounding Box"))
			{
				float size[2] = {
					properties["size"].value("x", 1.0f),
					properties["size"].value("y", 1.0f)
				};

				float offset[2] = {
					properties["offset"].value("x", 0.0f),
					properties["offset"].value("y", 0.0f)
				};

				float rotation = properties.value("rotation", 0.0f);
				bool isKinematic = properties.value("isKinematic", false);

				ImGui::Text("Size");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##Size", size, 0.1f))
				{
					properties["size"]["x"] = size[0];
					properties["size"]["y"] = size[1];
				}

				ImGui::Text("Offset");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##Offset", offset, 0.1f))
				{
					properties["offset"]["x"] = offset[0];
					properties["offset"]["y"] = offset[1];
				}

				ImGui::Text("Rotation");
				ImGui::SameLine(100);
				if (ImGui::DragFloat("##Rotation", &rotation, 1.0f, -360.0f, 360.0f))
				{
					properties["rotation"] = rotation;
				}

				if (ImGui::Checkbox("Is Kinematic", &isKinematic))
				{
					properties["isKinematic"] = isKinematic;
				}

				ComponentMenu(index);
			}
			index++;
		}
		else if (type == "BoundingCircle")
		{
			if (ImGui::CollapsingHeader("Bounding Circle"))
			{
				float offset[2] = {
					properties["offset"].value("x", 0.0f),
					properties["offset"].value("y", 0.0f)
				};

				float radius = properties.value("radius", 1.0f);
				bool isKinematic = properties.value("isKinematic", false);

				ImGui::Text("Offset");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##Offset", offset, 0.1f))
				{
					properties["offset"]["x"] = offset[0];
					properties["offset"]["y"] = offset[1];
				}

				ImGui::Text("Radius");
				ImGui::SameLine(100);
				if (ImGui::DragFloat("##Radius", &radius, 0.1f))
				{
					properties["radius"] = radius;
				}

				if (ImGui::Checkbox("Is Kinematic", &isKinematic))
				{
					properties["isKinematic"] = isKinematic;
				}

				ComponentMenu(index);
			}
			index++;
		}

		if (type == "AnimatorComponent")
		{
			if (ImGui::CollapsingHeader("Animator Component"))
			{
				std::string currentAnimation = properties.value("currentAnimation", "");
				if (ImGui::InputText("Current Animation", &currentAnimation[0], currentAnimation.size() + 1))
				{
					properties["currentAnimation"] = currentAnimation;
				}

				if (ImGui::Button("Open Animation Editor"))
				{
					int prefabEntityID = PrefabManager::GenerateTemporaryEntityFromPrefab(currentPrefabName);
					if (prefabEntityID != -1)
					{
						AnimationEditor::Open(prefabEntityID);
					}
					else
					{
						std::cerr << "Failed to generate a temporary entity for animation editing." << std::endl;
					}
				}

				ComponentMenu(index);
			}
			index++;
		}

		else if (type == "SoundComponent")
		{
			if (ImGui::CollapsingHeader("Sound Component"))
			{
				std::string soundID = properties.value("soundID", "");
				float volume = properties.value("volume", 1.0f);
				bool loop = properties.value("loop", false);
				bool playOnStart = properties.value("playOnStart", false);

				ImGui::Text("Sound ID");
				ImGui::SameLine(100);
				if (ImGui::InputText("##SoundID", soundID.data(), soundID.size()))
				{
					properties["soundID"] = soundID;
				}

				ImGui::Text("Volume");
				ImGui::SameLine(100);
				if (ImGui::DragFloat("##Volume", &volume, 0.01f, 0.0f, 1.0f))
				{
					properties["volume"] = volume;
				}

				if (ImGui::Checkbox("Loop", &loop))
				{
					properties["loop"] = loop;
				}

				if (ImGui::Checkbox("Play On Start", &playOnStart))
				{
					properties["playOnStart"] = playOnStart;
				}

				ComponentMenu(index);
			}
			index++;
		}
		else if (type == "TextComponent")
		{
			if (ImGui::CollapsingHeader("Text Component"))
			{
				std::string text = properties.value("text", "");
				std::string fontName = properties.value("fontName", "Default");
				int fontSize = properties.value("fontSize", 16);
				bool isEnabled = properties.value("isEnabled", true);

				float position[2] = {
					properties["position"].value("x", 0.0f),
					properties["position"].value("y", 0.0f)
				};

				ImVec4 color = {
					properties["color"].value("r", 255.0f) / 255.0f,
					properties["color"].value("g", 255.0f) / 255.0f,
					properties["color"].value("b", 255.0f) / 255.0f,
					properties["color"].value("a", 255.0f) / 255.0f
				};

				ImGui::Text("Text");
				ImGui::SameLine(100);
				if (ImGui::InputText("##Text", text.data(), text.size()))
				{
					properties["text"] = text;
				}

				ImGui::Text("Font Name");
				ImGui::SameLine(100);
				if (ImGui::InputText("##FontName", fontName.data(), fontName.size()))
				{
					properties["fontName"] = fontName;
				}

				ImGui::Text("Font Size");
				ImGui::SameLine(100);
				if (ImGui::DragInt("##FontSize", &fontSize, 1, 1, 100))
				{
					properties["fontSize"] = fontSize;
				}

				ImGui::Text("Position");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##Position", position, 0.1f))
				{
					properties["position"]["x"] = position[0];
					properties["position"]["y"] = position[1];
				}

				ImGui::Text("Color");
				ImGui::SameLine(100);
				if (ImGui::ColorEdit4("##Color", &color.x))
				{
					properties["color"]["r"] = static_cast<int>(color.x * 255);
					properties["color"]["g"] = static_cast<int>(color.y * 255);
					properties["color"]["b"] = static_cast<int>(color.z * 255);
					properties["color"]["a"] = static_cast<int>(color.w * 255);
				}

				if (ImGui::Checkbox("Enabled", &isEnabled))
				{
					properties["isEnabled"] = isEnabled;
				}

				ComponentMenu(index);
			}
			index++;
		}
		else if (type == "ButtonComponent")
		{
			if (ImGui::CollapsingHeader("Button Component"))
			{
				float minPos[2] = {
					properties["minPos"].value("x", 0.0f),
					properties["minPos"].value("y", 0.0f)
				};

				float maxPos[2] = {
					properties["maxPos"].value("x", 100.0f),
					properties["maxPos"].value("y", 50.0f)
				};

				bool isEnabled = properties.value("isEnabled", true);

				ImGui::Text("Min Position");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##MinPos", minPos, 0.1f))
				{
					properties["minPos"]["x"] = minPos[0];
					properties["minPos"]["y"] = minPos[1];
				}

				ImGui::Text("Max Position");
				ImGui::SameLine(100);
				if (ImGui::DragFloat2("##MaxPos", maxPos, 0.1f))
				{
					properties["maxPos"]["x"] = maxPos[0];
					properties["maxPos"]["y"] = maxPos[1];
				}

				if (ImGui::Checkbox("Enabled", &isEnabled))
				{
					properties["isEnabled"] = isEnabled;
				}

				ComponentMenu(index);
			}
			index++;
		}

		else if (type == "GameLogicComponent")
		{
			if (ImGui::CollapsingHeader("Game Logic Component"))
			{
				auto& logicNames = properties["logicNames"];

				if (!logicNames.is_array())
				{
					logicNames = nlohmann::json::array();
				}

				for (size_t i = 0; i < logicNames.size(); ++i)
				{
					std::string logicName = logicNames[i].get<std::string>();
					ImGui::Text("%s", logicName.c_str());
					ImGui::SameLine();

					if (ImGui::Button(("Remove##" + std::to_string(i)).c_str()))
					{
						logicNames.erase(logicNames.begin() + i);
					}
				}

				ImGui::Separator();

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
				if (ImGui::BeginCombo("###Available Logic Files", availableLogicFiles[selectedLogicIndex].c_str()))
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
					if (std::find(logicNames.begin(), logicNames.end(), selectedLogic) == logicNames.end())
					{
						logicNames.push_back(selectedLogic);
					}
				}

				ComponentMenu(index);
			}

			index++;
		}
	}

	if (!componentsToRemove.empty())
	{
		std::sort(componentsToRemove.rbegin(), componentsToRemove.rend());
		for (int removeIndex : componentsToRemove)
		{
			currentPrefab->componentsData.erase(currentPrefab->componentsData.begin() + removeIndex);
		}
		componentsToRemove.clear();
	}

	ImGui::Separator();
	AddComponent();
}

/**************************************************************************
* @brief Adds a new component to the selected prefab.
**************************************************************************/
void PrefabEditor::AddComponent()
{
	const std::vector<std::string> componentTypes = InspectorRenderer::GetComponentTypes();

	ImGui::Text("Add Component");
	if (ImGui::BeginCombo("##AddComponent", componentTypes[selectedComponentIndex].c_str()))
	{
		for (int i = 0; i < componentTypes.size(); ++i)
		{
			bool isSelected = (selectedComponentIndex == i);
			if (ImGui::Selectable(componentTypes[i].c_str(), isSelected))
			{
				selectedComponentIndex = i;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	const std::string& selectedType = componentTypes[selectedComponentIndex];
	bool componentExists = false;

	for (const auto& comp : currentPrefab->componentsData)
	{
		if (comp["type"] == selectedType)
		{
			componentExists = true;
			break;
		}
	}

	if (componentExists)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::Button("Add Component");
		ImGui::PopStyleVar();

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("This component type already exists on the prefab");
		}
		return;
	}


	if (ImGui::Button("Add Component"))
	{
		const std::string& selectedComponentType = componentTypes[selectedComponentIndex];
		nlohmann::json newComponent = { { "type", selectedComponentType }, { "properties", {} } };

		std::cout << "Selected Type: " << selectedComponentType << std::endl;

		if (selectedComponentType == "TransformComponent")
		{
			newComponent["properties"] = {
				{ "position", { { "x", 0.0f }, { "y", 0.0f } } },
				{ "scale", { { "x", 1.0f }, { "y", 1.0f } } },
				{ "rotation", 0.0f }
			};
		}
		else if (selectedComponentType == "SpriteRendererComponent")
		{
			newComponent["properties"] = {
				{ "useColor", false },
				{ "color", { { "r", 255 }, { "g", 255 }, { "b", 255 }, { "a", 255 } } },
				{ "sortingOrder", 0 },
				{ "texture", "" },
				{ "isVisible", true }
			};
		}
		else if (selectedComponentType == "RigidbodyComponent")
		{
			newComponent["properties"] = {
				{ "isStatic", false }
			};
		}
		else if (selectedComponentType == "BoundingBox")
		{
			Vec2 scale(0.5f, 0.5f);
			Vec2 position(0.0f, 0.0f);

			for (const auto& comp : currentPrefab->componentsData)
			{
				if (comp["type"] == "TransformComponent")
				{
					float transformScaleX = comp["properties"]["scale"].value("x", 1.0f);
					float transformScaleY = comp["properties"]["scale"].value("y", 1.0f);

					scale = Vec2(transformScaleX * 0.5f, transformScaleY * 0.5f);

					position = Vec2(
						comp["properties"]["position"].value("x", 0.0f),
						comp["properties"]["position"].value("y", 0.0f)
					);
					break;
				}
			}

			newComponent["properties"] = {
				{ "offset", { { "x", 0.0f }, { "y", 0.0f } } },
				{ "size", {
					{ "x", scale.x },
					{ "y", scale.y }
				}},
				{ "rotation", 0.0f },
				{ "isKinematic", false }
			};
		}

		else if (selectedComponentType == "AnimatorComponent")
		{
			newComponent["properties"] = {
				{ "currentAnimation", "" },
				{ "animations", nlohmann::json::array() }
			};
		}
		else if (selectedComponentType == "SoundComponent")
		{
			newComponent["properties"] = {
				{ "soundID", "" },
				{ "volume", 1.0f },
				{ "loop", false },
				{ "playOnStart", false }
			};
		}
		else if (selectedComponentType == "TextComponent")
		{
			newComponent["properties"] = {
				{ "text", "" },
				{ "fontName", "Default" },
				{ "fontSize", 16 },
				{ "position", { { "x", 0.0f }, { "y", 0.0f } } },
				{ "color", { { "r", 255 }, { "g", 255 }, { "b", 255 }, { "a", 255 } } },
				{ "isEnabled", true }
			};
		}
		else if (selectedComponentType == "ButtonComponent")
		{
			newComponent["properties"] = {
				{ "minPos", { { "x", 0.0f }, { "y", 0.0f } } },
				{ "maxPos", { { "x", 100.0f }, { "y", 50.0f } } },
				{ "isEnabled", true }
			};
		}
		else if (selectedComponentType == "GameLogicComponent")
		{
			newComponent["properties"] = {
				{ "logicNames", nlohmann::json::array() }
			};
		}

		currentPrefab->componentsData.push_back(newComponent);
	}
}

/**************************************************************************
* @brief Renders the preview of the selected prefab.
**************************************************************************/
void PrefabEditor::RenderPrefabPreview()
{
	if (!currentPrefab)
		return;

	std::string texturePath;
	ImVec2 scale = { 1.0f, 1.0f };
	float rotation = 0.0f;
	ImVec2 position = { 0.0f, 0.0f };

	for (const auto& component : currentPrefab->componentsData)
	{
		if (component["type"] == "SpriteRendererComponent")
		{
			texturePath = component["properties"].value("texture", "");
		}
		else if (component["type"] == "TransformComponent")
		{
			if (component["properties"].contains("scale"))
			{
				const auto& scaleJson = component["properties"]["scale"];
				scale = ImVec2(
					scaleJson.value("x", 1.0f),
					scaleJson.value("y", 1.0f)
				);
			}

			rotation = component["properties"].value("rotation", 0.0f);

			if (component["properties"].contains("position"))
			{
				const auto& positionJson = component["properties"]["position"];
				position = ImVec2(
					positionJson.value("x", 0.0f),
					positionJson.value("y", 0.0f)
				);
			}
		}
	}

	if (!texturePath.empty())
	{
		auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(texturePath);
		if (texture)
		{
			//float scaleDownFromOriginalSize = 5.0f;
			ImVec2 availableSpace = ImGui::GetContentRegionAvail();
			ImVec2 windowPos = ImGui::GetCursorScreenPos();
			//ImVec2 textureSize = ImVec2(ImageLoader::GetTextureWidth(texturePath) / scaleDownFromOriginalSize, ImageLoader::GetTextureHeight(texturePath) / scaleDownFromOriginalSize);
			ImVec2 textureSize = { 32.0f, 25.0f };
			
			//std::cout << "Width: " << textureSize.x << ", Height: " << textureSize.y << std::endl;
			ImVec2 scaledTextureSize = ImVec2(
				textureSize.x * scale.x,
				textureSize.y * scale.y
			);

			ImVec2 centerPos = ImVec2(
				(availableSpace.x * 0.5f) + windowPos.x,
				(availableSpace.y * 0.5f) + windowPos.y
			);

			ImVec2 adjustedPos = ImVec2(
				centerPos.x + position.x,
				centerPos.y - position.y
			);

			ImVec2 textureOffset = ImVec2(
				scaledTextureSize.x * 0.5f,
				scaledTextureSize.y * 0.5f
			);

			float rotationRadians = rotation * (IM_PI / 180.0f);

			ImVec2 corners[4];
			float sinTheta = sin(-rotationRadians);
			float cosTheta = cos(-rotationRadians);

			corners[0] = ImVec2(-textureOffset.x, -textureOffset.y);
			corners[1] = ImVec2(textureOffset.x, -textureOffset.y);
			corners[2] = ImVec2(textureOffset.x, textureOffset.y);
			corners[3] = ImVec2(-textureOffset.x, textureOffset.y);

			for (int i = 0; i < 4; i++)
			{
				float x = corners[i].x * cosTheta - corners[i].y * sinTheta;
				float y = corners[i].x * sinTheta + corners[i].y * cosTheta;
				corners[i] = ImVec2(x + adjustedPos.x, y + adjustedPos.y);
			}

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddImageQuad(
				(void*)(intptr_t)*texture,
				corners[0], corners[1], corners[2], corners[3],
				ImVec2(0, 1), ImVec2(1, 1), ImVec2(1, 0), ImVec2(0, 0)
			);

		}
		else
		{
			ImGui::Text("Failed to load texture.");
		}
	}
	else
	{
		ImGui::Text("No texture available.");
	}
}

/**************************************************************************
* @brief Displays a context menu for a specific component.
* @param componentIndex The index of the component in the prefab's data.
**************************************************************************/
void PrefabEditor::ComponentMenu(int componentIndex)
{
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + contentRegion.x - 30);

	if (ImGui::Button(("...##" + std::to_string(componentIndex)).c_str()))
	{
		ImGui::OpenPopup(("ComponentMenu##" + std::to_string(componentIndex)).c_str());
	}

	if (ImGui::BeginPopup(("ComponentMenu##" + std::to_string(componentIndex)).c_str()))
	{
		if (ImGui::MenuItem("Remove Component"))
		{
			// Mark the component for removal
			componentsToRemove.push_back(componentIndex);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
