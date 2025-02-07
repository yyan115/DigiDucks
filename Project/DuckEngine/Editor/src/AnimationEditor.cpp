/******************************************************************************/
/*!
\file       AnimationEditor.cpp
\author     Lucas Yee 2301212 (70%)
\par        l.yee@digipen.edu
\author     Muhammad Zikry Bin Zakaria , 2201751 (30%)
\par        muhammadzikry.b@digipen.edu
\date       November 30 2024
\brief      Implements the AnimationEditor class, which provides an editor
			interface for managing animations of game entities. This includes
			rendering animation lists, timelines, properties, and previews.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "AnimationEditor.h"
#include "DuckEngine.h"
#include "AssetManager.h"
#include "Inspector.h"
#include "ImageLoader.h"

bool AnimationEditor::isOpen = false;
int AnimationEditor::selectedEntityID = -1;
std::string AnimationEditor::currentAnimationName = "";

// Preview animation variables
bool isPreviewing = false;
float previewElapsedTime = 0.0f;
int previewCurrentFrame = 0;
std::string previousAnimationName = "";

/**************************************************************************
* @brief Opens the animation editor for a specific entity.
* @param entityId The ID of the entity to edit animations for.
**************************************************************************/
void AnimationEditor::Open(int entityId)
{
	selectedEntityID = entityId;
	isOpen = true;
}

/**************************************************************************
* @brief Renders the Animation Editor window and its components.
**************************************************************************/
void AnimationEditor::Render()
{
	if (!isOpen || selectedEntityID == -1)
	{
		return;
	}

	auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(selectedEntityID);
	if (!animator)
	{
		if (ImGui::Begin("Animation Editor", &isOpen))
		{
			ImGui::Text("No Animator Component found on this entity.");
			if (ImGui::Button("Close"))
			{
				isOpen = false;
			}
		}
		ImGui::End();
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(1200, 600), ImGuiCond_FirstUseEver);

	bool windowOpen = true;
	if (ImGui::Begin("Animation Editor", &windowOpen, ImGuiWindowFlags_NoCollapse))
	{
		float columnWidth = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth * 0.2f);

		if (ImGui::BeginChild("AnimationList", ImVec2(0, 0), true))
		{
			RenderAnimationList(animator);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		if (ImGui::BeginChild("AnimationDetails", ImVec2(0, 0), true))
		{
			if (!currentAnimationName.empty())
			{
				if (previousAnimationName != currentAnimationName)
				{
					isPreviewing = false;
					previewElapsedTime = 0.0f;
					previewCurrentFrame = 0;
					previousAnimationName = currentAnimationName;
				}

				RenderTimeline(animator);
				RenderAnimationProperties(animator);
				RenderAnimationPreview(animator);
			}
			else
			{
				ImGui::Text("Select an animation to edit.");
			}
		}
		ImGui::EndChild();

		ImGui::Columns(1);
	}
	ImGui::End();

	if (!windowOpen)
	{
		isOpen = false;
	}
}

/**************************************************************************
* @brief Renders the list of animations in the editor (left column).
* @param animator The animator component of the current entity.
**************************************************************************/
void AnimationEditor::RenderAnimationList(AnimatorComponent* animator)
{
	auto& animations = animator->GetAnimations();
	std::vector<std::string> animationsToRemove;

	for (const auto& [name, animation] : animations)
	{
		bool isSelected = (currentAnimationName == name);

		if (ImGui::Selectable(name.c_str(), isSelected))
		{
			currentAnimationName = name;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove Animation"))
			{
				animationsToRemove.push_back(name);
				if (currentAnimationName == name)
				{
					currentAnimationName.clear();
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	for (const std::string& name : animationsToRemove)
	{
		animations.erase(name);
		UpdatePrefabAnimations(animator, name);
	}

	if (ImGui::Button("Add Animation"))
	{
		static int newAnimationIndex = 1;
		std::string newName = "NewAnimation" + std::to_string(newAnimationIndex++);

		while (animations.find(newName) != animations.end())
		{
			newName = "NewAnimation" + std::to_string(newAnimationIndex++);
		}

		animations[newName] = Animation(0.1f);
		currentAnimationName = newName; 
	}
}

/**************************************************************************
* @brief Renders the timeline section for the selected animation,
*        including rename, importing sprite sheet, and frame list.
* @param animator The animator component of the current entity.
**************************************************************************/
void AnimationEditor::RenderTimeline(AnimatorComponent* animator)
{
	auto& animation = animator->animations[currentAnimationName];

	static char renameBuffer[128] = "";
	static std::string originalName = "";

	if (originalName.empty() || originalName != currentAnimationName)
	{
		strncpy_s(renameBuffer, sizeof(renameBuffer), currentAnimationName.c_str(), _TRUNCATE);
		renameBuffer[sizeof(renameBuffer) - 1] = '\0';
		originalName = currentAnimationName;
	}

	ImGui::Text("Timeline - %s", currentAnimationName.c_str());
	ImGui::Separator();

	ImGui::InputText("##AnimationName", renameBuffer, sizeof(renameBuffer));

	ImGui::SameLine();
	if (ImGui::Button("Apply##RenameAnimation"))
	{
		std::string newName(renameBuffer);
		if (!newName.empty() &&
			newName != currentAnimationName &&
			animator->animations.find(newName) == animator->animations.end())
		{
			Animation tempData = animator->animations[currentAnimationName];
			animator->animations.erase(currentAnimationName);
			animator->animations[newName] = tempData;

			currentAnimationName = newName;
			originalName = newName;
			std::cout << "Animation renamed to: " << newName << std::endl;
		}
		else
		{
			std::cerr << "Error: Animation name must be unique and non-empty!" << std::endl;
		}
	}

	ImGui::Separator();

	static std::string spriteSheetPath;
	static int spriteWidth = 64;
	static int spriteHeight = 64;
	static bool showSpriteSheetPopup = false;

	if (ImGui::Button("Import Sprite Sheet"))
	{
		spriteSheetPath = LevelManager::OpenFileDialog("texture");
		if (!spriteSheetPath.empty())
		{
			showSpriteSheetPopup = true;
			ImGui::OpenPopup("Sprite Sheet Dimensions");
		}
	}

	if (ImGui::BeginPopupModal("Sprite Sheet Dimensions", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputInt("Sprite Width", &spriteWidth);
		ImGui::InputInt("Sprite Height", &spriteHeight);

		if (ImGui::Button("OK"))
		{
			if (!spriteSheetPath.empty() && spriteWidth > 0 && spriteHeight > 0)
			{
				std::vector<GLuint> frames = ImageLoader::LoadSpriteSheet(spriteSheetPath, spriteWidth, spriteHeight);
				if (!frames.empty())
				{
					// Generate the sliced sprite names
					size_t lastSlash = spriteSheetPath.find_last_of("/\\");
					std::string directory = (lastSlash == std::string::npos) ? "" : spriteSheetPath.substr(0, lastSlash + 1);
					std::string baseName = spriteSheetPath.substr(lastSlash + 1);
					baseName = baseName.substr(0, baseName.find_last_of('.'));

					for (int i = 0; i < frames.size(); ++i)
					{
						GLuint textureID = frames[i];
						auto texture = std::make_shared<Texture>(textureID);
						animation.Frames.push_back(texture);

						// Generate sliced sprite file path
						std::string relativePath = "Resources/Sprites/" + baseName + "_" + std::to_string(i + 1) + ".png";
						animation.texturePaths.push_back(relativePath);

						std::cout << "Added frame from sprite sheet: " << relativePath
							<< " with Texture ID: " << textureID << std::endl;
					}
				}
				else
				{
					std::cerr << "Failed to load sprite sheet: " << spriteSheetPath << std::endl;
				}
			}
			ImGui::CloseCurrentPopup();
			showSpriteSheetPopup = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			showSpriteSheetPopup = false;
		}

		ImGui::EndPopup();
	}

	ImGui::Separator();

	int numFrames = static_cast<int>(animation.Frames.size());
	int framesPerRow = 10;
	int framesRendered = 0;

	for (int i = 0; i < numFrames; ++i)
	{
		ImGui::PushID(i);
		auto& texture = animation.Frames[i];

		if (texture && *texture != 0)
		{
			if (ImGui::ImageButton(
				("##Frame" + std::to_string(i)).c_str(),
				(void*)(uintptr_t)*texture,
				ImVec2(64.0f, 64.0f),
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f)
			))
			{
				animation.currentFrame = i;
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Remove Frame"))
				{
					animation.Frames.erase(animation.Frames.begin() + i);
					animation.texturePaths.erase(animation.texturePaths.begin() + i);
					--i;
					--numFrames;
					ImGui::EndPopup();
					ImGui::PopID();
					continue;
				}
				ImGui::EndPopup();
			}
		}
		else
		{
			// Show an empty slot if there's no texture
			if (ImGui::Button(("Empty##Frame" + std::to_string(i)).c_str(), ImVec2(64.0f, 64.0f)))
			{
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Remove Frame"))
				{
					animation.Frames.erase(animation.Frames.begin() + i);
					animation.texturePaths.erase(animation.texturePaths.begin() + i);
					--i;
					--numFrames;
					ImGui::EndPopup();
					ImGui::PopID();
					continue;
				}
				ImGui::EndPopup();
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_PAYLOAD"))
			{
				const char* path = static_cast<const char*>(payload->Data);
				if (InspectorRenderer::IsAllowedExtension(path, InspectorRenderer::GetAllowedImageExtensions()))
				{
					DuckEngine::DUCKENGINE_AssetManager.LoadTexture(path);
					auto newTexture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(path);

					if (newTexture)
					{
						animation.Frames[i] = newTexture;
						if (i < static_cast<int>(animation.texturePaths.size()))
						{
							animation.texturePaths[i] = path;
						}
						else
						{
							animation.texturePaths.push_back(path);
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		framesRendered++;
		if (framesRendered % framesPerRow != 0)
		{
			ImGui::SameLine();
		}
	}

	if (numFrames == 0 || framesRendered % framesPerRow != 0)
	{
		ImGui::NewLine();
	}

	ImGui::PushID("AddFrameButton");
	if (ImGui::Button("Add Frame", ImVec2(85.0f, 50.0f)))
	{
		animation.Frames.push_back(nullptr);
		animation.texturePaths.push_back("");
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_PAYLOAD"))
		{
			const char* path = static_cast<const char*>(payload->Data);
			if (InspectorRenderer::IsAllowedExtension(path, InspectorRenderer::GetAllowedImageExtensions()))
			{
				DuckEngine::DUCKENGINE_AssetManager.LoadTexture(path);
				auto newTexture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(path);

				if (newTexture)
				{
					animation.Frames.push_back(newTexture);
					animation.texturePaths.push_back(path);
				}
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::PopID();
}

/**************************************************************************
* @brief Renders the properties section for the selected animation.
* @param animator The animator component of the current entity.
**************************************************************************/
void AnimationEditor::RenderAnimationProperties(AnimatorComponent* animator)
{
	auto& animation = animator->animations[currentAnimationName];

	ImGui::Text("Animation Properties");
	ImGui::Separator();

	if (ImGui::DragFloat("Frame Duration", &animation.frameDuration, 0.01f, 0.01f, 5.0f))
	{
		animation.frameTimer = 0.0f;
	}

	if (ImGui::Button(isPreviewing ? "Pause Animation" : "Play Animation"))
	{
		isPreviewing = !isPreviewing;
		if (!isPreviewing)
		{
			previewElapsedTime = 0.0f;
			previewCurrentFrame = 0;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Save Animation"))
	{
		LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
	}
}

/**************************************************************************
* @brief Renders the preview section for the selected animation.
* @param animator The animator component of the current entity.
**************************************************************************/
void AnimationEditor::RenderAnimationPreview(AnimatorComponent* animator)
{
	auto& animation = animator->animations[currentAnimationName];

	ImGui::Separator();

	ImGui::BeginChild("AnimationPreview", ImVec2(0, 200), true);

	ImGui::Text("Animation Preview");
	ImGui::Separator();

	if (isPreviewing)
	{
		previewElapsedTime += DuckEngine::DeltaTime();

		float totalDuration = animation.frameDuration * animation.Frames.size();

		if (totalDuration > 0.0f)
		{
			float animationTime = fmod(previewElapsedTime, totalDuration);
			int frameIndex = static_cast<int>(animationTime / animation.frameDuration) % animation.Frames.size();
			previewCurrentFrame = frameIndex;
		}
		else
		{
			previewCurrentFrame = 0;
		}
	}

	if (!animation.Frames.empty())
	{
		auto& texture = animation.Frames[previewCurrentFrame];

		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		ImVec2 imageSize(128.0f, 128.0f);

		float padX = (contentRegion.x - imageSize.x) * 0.5f;
		float padY = (contentRegion.y - imageSize.y) * 0.5f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padX);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padY);

		if (texture && *texture != 0)
		{
			ImGui::Image(
				(void*)(uintptr_t)*texture,
				imageSize,
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f),
				ImVec4(1, 1, 1, 1),
				ImVec4(0, 0, 0, 1)
			);
		}
		else
		{
			ImGui::Text("No texture for current frame.");
		}
	}
	else
	{
		ImGui::Text("Animation has no frames.");
	}

	ImGui::EndChild();
}

/**************************************************************************
* @brief (Optional) Renders a preview grid of frames if you wish.
*        You can call this if you want to show the user a sheet of frames
*        before adding them to the animation.
* @param frames A list of textures to display as a grid.
**************************************************************************/
void AnimationEditor::RenderSpriteSheetPreview(const std::vector<std::shared_ptr<Texture>>& frames)
{
	auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(selectedEntityID);
	if (!animator)
	{
		ImGui::Text("No Animator Component found.");
		return;
	}

	ImGui::Text("Sprite Sheet Preview");
	ImGui::Separator();

	int framesPerRow = 10;
	int framesRendered = 0;

	for (size_t i = 0; i < frames.size(); ++i)
	{
		ImGui::PushID(static_cast<int>(i));

		auto& texture = frames[i];
		if (texture)
		{
			ImTextureID texID = reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(*texture));
			if (ImGui::ImageButton(
				"##FrameButton",
				texID,
				ImVec2(64.0f, 64.0f),
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f)))
			{
				auto& animation = animator->animations[currentAnimationName];
				animation.Frames.push_back(texture);
				animation.texturePaths.push_back("Path_To_Frame");
			}
		}

		ImGui::PopID();

		framesRendered++;
		if (framesRendered % framesPerRow != 0)
		{
			ImGui::SameLine();
		}
	}

	ImGui::NewLine();
}

void AnimationEditor::UpdatePrefabAnimations(AnimatorComponent* animator, const std::string& removedAnimationName)
{
	auto prefab = PrefabManager::GetPrefabFromEntity(DuckEngine::DUCKENGINE_EntityManager.GetEntity(animator->GetEntityID()).get());
	if (!prefab)
	{
		std::cerr << "Failed to find prefab associated with entity." << std::endl;
		return;
	}

	auto& prefabAnimations = prefab->componentsData["AnimatorComponent"]["animations"];
	prefabAnimations.erase(removedAnimationName);

	PrefabManager::SyncPrefabInstances(prefab);

	PrefabManager::SavePrefab(prefab->name);

	DuckEngine::DUCKENGINE_SceneManager.ReloadScene();
}
