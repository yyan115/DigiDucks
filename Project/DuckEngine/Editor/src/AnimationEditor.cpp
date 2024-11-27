#include "AnimationEditor.h"
#include "DuckEngine.h"
#include "AssetManager.h"
#include "Inspector.h"

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

    ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1200, 600), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Animation Editor", &isOpen, ImGuiWindowFlags_NoCollapse))
    {
        float columnWidth = ImGui::GetContentRegionAvail().x;
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, columnWidth * 0.2f);

        ImGui::BeginChild("AnimationList", ImVec2(0, 0), true);
        RenderAnimationList(animator);
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("AnimationDetails", ImVec2(0, 0), true);
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

        ImGui::End();
    }
}


void AnimationEditor::RenderAnimationList(AnimatorComponent* animator)
{
    ImGui::Text("Animations");
    ImGui::Separator();

    auto& animations = animator->GetAnimations();

    for (const auto& [name, animation] : animations)
    {
        if (ImGui::Selectable(name.c_str(), currentAnimationName == name))
        {
            currentAnimationName = name;
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Remove Animation"))
            {
                animator->animations.erase(name);
                if (currentAnimationName == name)
                {
                    currentAnimationName = "";
                }
                ImGui::EndPopup();
                break;
            }
            ImGui::EndPopup();
        }
    }

    if (ImGui::Button("Add Animation"))
    {
        static int newAnimationIndex = 1;
        std::string newName = "NewAnimation" + std::to_string(newAnimationIndex++);

        while (animations.find(newName) != animations.end())
        {
            newName = "NewAnimation" + std::to_string(newAnimationIndex++);
        }

        Animation newAnimation(0.2f);
        animator->animations[newName] = newAnimation;
        currentAnimationName = newName;
    }
}

void AnimationEditor::RenderTimeline(AnimatorComponent* animator)
{
    auto& animation = animator->animations[currentAnimationName];

    ImGui::Text("Timeline - %s", currentAnimationName.c_str());
    ImGui::Separator();

    int numFrames = static_cast<int>(animation.Frames.size());
    int framesPerRow = 10;
    int framesRendered = 0;

    for (int i = 0; i < numFrames; ++i)
    {
        ImGui::PushID(i);

        auto texture = animation.Frames[i];

        if (texture && *texture != 0)
        {
            if (ImGui::ImageButton(
                ("##Frame" + std::to_string(i)).c_str(),
                (void*)(uintptr_t)*texture,
                ImVec2(64.0f, 64.0f),
                ImVec2(0.0f, 1.0f),
                ImVec2(1.0f, 0.0f),
                ImVec4(0, 0, 0, 0),
                ImVec4(1, 1, 1, 1)
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
                        if (i < animation.texturePaths.size())
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


void AnimationEditor::RenderAnimationProperties(AnimatorComponent* animator)
{
    auto& animation = animator->animations[currentAnimationName];

    ImGui::Text("Animation Properties");
    ImGui::Separator();

    if (ImGui::DragFloat("Frame Duration", &animation.frameDuration, 0.01f, 0.01f, 5.0f))
    {
        animation.frameTimer = 0.0f;
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

    if (ImGui::Button("Save Animation"))
    {
        LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
    }
}
