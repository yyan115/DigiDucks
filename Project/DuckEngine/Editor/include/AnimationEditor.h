#pragma once
#include <string>
#include <vector>
#include "AnimatorComponent.h"
#include "imgui.h"

class AnimationEditor
{
public:
    static bool isOpen;
    static int selectedEntityID;
    static std::string currentAnimationName;

    static void Open(int entityId);
    static void Render();

private:
    static void RenderAnimationList(AnimatorComponent* animator);
    static void RenderTimeline(AnimatorComponent* animator);
    static void RenderAnimationProperties(AnimatorComponent* animator);
};
