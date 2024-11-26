/******************************************************************************
\file       AnimationEditor.cpp
\author     Lucas Yee 2301212 (60%)
\par        l.yee@digipen.edu
\author     Muhammad Zikry Bin Zakaria , 2201751 (40%)
\par        muhammadzikry.b@digipen.edu

\brief      Definition of the AnimationEditor class, which provides functionalities
            for managing the animation editor GUI, including rendering the animation
            list, timeline, and properties.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include "AnimationEditor.h"
#include "DuckEngine.h"
#include "imgui.h"

bool AnimationEditor::isOpen = false;
int AnimationEditor::selectedEntityID = -1;
std::string AnimationEditor::currentAnimationName = "";

void AnimationEditor::Open(int entityId)
{
    selectedEntityID = entityId;
    isOpen = true;
}

void AnimationEditor::Render()
{
    if (!isOpen || selectedEntityID == -1)
        return;

    auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(selectedEntityID);
    if (!animator)
    {
        ImGui::Text("No Animator Component found on this entity.");
        if (ImGui::Button("Close")) isOpen = false;
        return;
    }

    ImGui::OpenPopup("Animation Editor");
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImVec2 windowSize = ImVec2(1200, 600);
    ImVec2 centerPos = ImVec2((viewportSize.x - windowSize.x) / 2, (viewportSize.y - windowSize.y) / 2);

    ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    if (ImGui::BeginPopupModal("Animation Editor", &isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
    {
        ImGui::Columns(2, nullptr, true);

        // Left Panel: Animation List
        ImGui::BeginChild("AnimationList", ImVec2(0, 0), true, ImGuiWindowFlags_NoCollapse);
        RenderAnimationList(animator);
        ImGui::EndChild();

        ImGui::NextColumn();

        // Right Panel: Timeline and Properties
        ImGui::BeginChild("AnimationDetails", ImVec2(0, 0), true, ImGuiWindowFlags_NoCollapse);
        if (!currentAnimationName.empty())
        {
            RenderTimeline(animator);
            RenderAnimationProperties(animator);
        }
        else
        {
            ImGui::Text("Select an animation to edit.");
        }
        ImGui::EndChild();

        ImGui::Columns(1);

        ImGui::EndPopup();
    }
}

void AnimationEditor::RenderAnimationList(AnimatorComponent* animator)
{
    ImGui::Text("Animations");
    ImGui::Separator();

    auto& animations = animator->GetAnimations();

    // List all existing animations
    for (const auto& [name, animation] : animations)
    {
        if (ImGui::Selectable(name.c_str(), currentAnimationName == name))
        {
            currentAnimationName = name; // Set the selected animation
        }
    }

    // Add animation button logic
    if (ImGui::Button("Add Animation"))
    {
        static int newAnimationIndex = 1;
        std::string newName = "NewAnimation" + std::to_string(newAnimationIndex++);

        // Ensure the name is unique
        while (animations.find(newName) != animations.end())
        {
            newName = "NewAnimation" + std::to_string(newAnimationIndex++);
        }

        Animation newAnimation(0.2f); // Default frame duration
        animator->animations[newName] = newAnimation; // Add to animations
        currentAnimationName = newName; // Set the new animation as selected
    }
}


void AnimationEditor::RenderTimeline(AnimatorComponent* animator)
{
    auto& animation = animator->animations[currentAnimationName];

    ImGui::Text("Timeline - %s", currentAnimationName.c_str());
    ImGui::Separator();

    int numFrames = animation.Frames.size();
    for (int i = 0; i < numFrames; ++i)
    {
        ImGui::PushID(i);
        if (ImGui::Button(std::to_string(i).c_str(), ImVec2(32, 32)))
        {
            animation.currentFrame = i; // Set preview frame
        }
        if (i < numFrames - 1)
            ImGui::SameLine();
        ImGui::PopID();
    }

    if (ImGui::Button("Add Frame"))
    {
        // Add a new frame (default frame or allow file selection)
        //animation.Frames.push_back(Texture()); // Placeholder texture
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove Last Frame") && !animation.Frames.empty())
    {
        animation.Frames.pop_back();
    }
}

void AnimationEditor::RenderAnimationProperties(AnimatorComponent* animator)
{
    auto& animation = animator->animations[currentAnimationName];

    ImGui::Text("Animation Properties");
    ImGui::Separator();

    if (ImGui::DragFloat("Frame Duration", &animation.frameDuration, 0.01f, 0.01f, 5.0f))
    {
        animation.frameTimer = 0.0f; // Reset frame timer if duration changes
    }

    if (ImGui::Button("Preview Animation"))
    {
        animator->SetAnimation(currentAnimationName);
        animator->Resume();
    }

    ImGui::SameLine();
    if (ImGui::Button("Pause Preview"))
    {
        animator->Pause();
    }
}

