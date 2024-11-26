/******************************************************************************
\file       AnimationEditor.h
\author     Lucas Yee 2301212 (60%)
\par        l.yee@digipen.edu
\author     Muhammad Zikry Bin Zakaria , 2201751 (40%)
\par        muhammadzikry.b@digipen.edu

\brief      Declaration of the AnimationEditor class, which provides functionalities
			for managing the animation editor GUI, including rendering the animation
			list, timeline, and properties.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once

#include <string>
#include "AnimatorComponent.h"

/**
 * @brief AnimationEditor handles the GUI for editing animations within the engine.
 */
class AnimationEditor
{
public:
    // Indicates if the editor is currently open
    static bool isOpen;

    // The entity ID of the selected AnimatorComponent
    static int selectedEntityID;

    // The name of the currently selected animation
    static std::string currentAnimationName;

    /**
     * @brief Opens the Animation Editor for the given entity ID.
     * @param entityId The ID of the entity with the AnimatorComponent to edit.
     */
    static void Open(int entityId);

    /**
     * @brief Renders the Animation Editor GUI.
     */
    static void Render();

private:
    /**
     * @brief Renders the list of animations in the left panel.
     * @param animator Pointer to the AnimatorComponent being edited.
     */
    static void RenderAnimationList(AnimatorComponent* animator);

    /**
     * @brief Renders the animation timeline in the right panel.
     * @param animator Pointer to the AnimatorComponent being edited.
     */
    static void RenderTimeline(AnimatorComponent* animator);

    /**
     * @brief Renders the properties of the currently selected animation.
     * @param animator Pointer to the AnimatorComponent being edited.
     */
    static void RenderAnimationProperties(AnimatorComponent* animator);
};
