/******************************************************************************/
/*!
\file       AnimationEditor.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Declares the AnimationEditor class, which provides a graphical
            interface for managing animations of game entities. This class
            includes functionality for opening the editor, rendering animation
            lists, timelines, properties, and animation previews.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

    /**************************************************************************
    * @brief Opens the animation editor for a specific entity.
    * @param entityId The ID of the entity to edit animations for.
    **************************************************************************/
    static void Open(int entityId);

    /**************************************************************************
    * @brief Renders the Animation Editor window and its components.
    **************************************************************************/
    static void Render();

private:
    /**************************************************************************
    * @brief Renders the list of animations for the selected entity.
    * @param animator Pointer to the AnimatorComponent of the selected entity.
    **************************************************************************/
    static void RenderAnimationList(AnimatorComponent* animator);

    /**************************************************************************
    * @brief Renders the timeline of the selected animation.
    * @param animator Pointer to the AnimatorComponent of the selected entity.
    **************************************************************************/
    static void RenderTimeline(AnimatorComponent* animator);

    /**************************************************************************
    * @brief Renders the properties of the selected animation.
    * @param animator Pointer to the AnimatorComponent of the selected entity.
    **************************************************************************/
    static void RenderAnimationProperties(AnimatorComponent* animator);

    /**************************************************************************
    * @brief Renders a preview of the selected animation.
    * @param animator Pointer to the AnimatorComponent of the selected entity.
    **************************************************************************/
    static void RenderAnimationPreview(AnimatorComponent* animator);

    static void RenderSpriteSheetPreview(const std::vector<std::shared_ptr<Texture>>& frames);

    static void UpdatePrefabAnimations(AnimatorComponent* animator, const std::string& removedAnimationName);

};
