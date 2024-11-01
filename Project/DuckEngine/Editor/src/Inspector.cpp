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

std::unordered_map<int, bool> InspectorRenderer::entityChanges;
static const std::unordered_set<std::string> allowedImageExtensions = { "png", "jpg", "jpeg" };

void InspectorRenderer::RenderComponents(int entityID)
{
    // Handle case where no entity is selected
    if (entityID == -1) {
        ImGui::Text("No entity selected.");
        return;
    }

    // Use reference to track changes for the current entity
    bool& hasChanged = entityChanges[entityID];

    // Render TransformComponent if it exists
    if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID))
    {
        if (ImGui::CollapsingHeader("Transform Component"))
        {
            ImGui::SliderFloat2("Position", &transform->position.x, -100.0f, 100.0f);
            if (ImGui::IsItemEdited()) hasChanged = true;

            ImGui::SliderFloat("Rotation", &transform->angle, -180.0f, 180.0f);
            if (ImGui::IsItemEdited()) hasChanged = true;

            ImGui::SliderFloat2("Scale", &transform->scale.x, 0.1f, 10.0f);
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

            ImGui::SliderInt("Layer", &spriteRenderer->layer, 0, 10);
            if (ImGui::IsItemEdited()) hasChanged = true;

            ImGui::ColorEdit4("Color", (float*)&spriteRenderer->color);
            if (ImGui::IsItemEdited()) hasChanged = true;

            // Display the current texture as a preview if it exists
            if (!spriteRenderer->texturePath.empty()) {
                auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(spriteRenderer->texturePath);
                if (texture) {
                    ImGui::Text("Current Texture:");
                    ImGui::Image((void*)(intptr_t)*texture, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

                }
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
        }
    }

    // Render RigidbodyComponent if it exists
    if (auto* rb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityID))
    {
        if (ImGui::CollapsingHeader("Rigidbody Component"))
        {
            ImGui::Checkbox("Is Static", &rb->isStatic);
            if (ImGui::IsItemEdited()) hasChanged = true;
        }
    }

    // Render BoundingBox if it exists
    if (auto* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityID))
    {
        if (ImGui::CollapsingHeader("Bounding Box"))
        {
            Vec2 center = box->getCenter();
            Vec2 size = box->getSize();

            ImGui::SliderFloat2("Center", &center.x, -10.0f, 10.0f);
            if (ImGui::IsItemEdited()) hasChanged = true;

            ImGui::SliderFloat2("Size", &size.x, 0.1f, 10.0f);
            if (ImGui::IsItemEdited()) hasChanged = true;

            // Update component with modified values
            box->setCenter(center);
            box->setSize(size);
        }
    }

    // Render BoundingCircle if it exists
    if (auto* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityID))
    {
        if (ImGui::CollapsingHeader("Bounding Circle"))
        {
            Vec2 center = circle->getCenter();
            float radius = circle->getRadius();

            ImGui::SliderFloat2("Center", &center.x, -10.0f, 10.0f);
            if (ImGui::IsItemEdited()) hasChanged = true;

            ImGui::SliderFloat("Radius", &radius, 0.1f, 10.0f);
            if (ImGui::IsItemEdited()) hasChanged = true;

            // Update component with modified values
            circle->setCenter(center);
            circle->setRadius(radius);
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
            ImGui::SliderFloat("Volume", &sound->volume, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemEdited()) hasChanged = true;
        }
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
            hasChanged = false; 
        }
    }
}

bool InspectorRenderer::IsAllowedExtension(const std::string& filePath, const std::unordered_set<std::string>& allowedExtensions) {
    // Extract the file extension
    std::string extension = filePath.substr(filePath.find_last_of('.') + 1);

    // Convert extension to lowercase for case-insensitive comparison
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    // Check if the extension is in the allowed set
    return allowedExtensions.find(extension) != allowedExtensions.end();
}
