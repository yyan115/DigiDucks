/******************************************************************************/
/*!
\file       ButtonSystem.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 28 2024
\brief      Implements the ButtonSystem class, providing functionality for
            managing button interactions such as detecting clicks and handling
            events in both editor and game modes. Includes utilities for
            determining whether a point is within button bounds.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GraphicsManager.h"
#include "ButtonSystem.h"
#include "ButtonComponent.h"
#include "DuckEngine_Input.h"
#include "imgui.h"
#include "CameraManager.h"
#include "WindowManager.h"

// Utility function to check if a point is within button bounds
bool IsPointInside(const Vector2D& point, const Vector2D& position, const Vector2D& scale)
{
    Vector2D min = position - scale * 0.5f; // Calculate the minimum boundary
    Vector2D max = position + scale * 0.5f; // Calculate the maximum boundary

    return point.x >= min.x && point.x <= max.x &&
        point.y >= min.y && point.y <= max.y;
}

void ButtonSystem::Start()
{
}

void ButtonSystem::Update()
{
}

// Temporary render function until FixedUpdate and Update are implemented
void ButtonSystem::Render()
{
    // static int frameCounter = 0;

    std::vector<ButtonComponent> ClickedButtons;

    // Iterate over all ButtonComponents
    for (const auto& [entityId, component] :
        DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
    {
        ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
        if (!button) continue;

        // Get the TransformComponent so we can read position/scale
        TransformComponent* sprite =
            DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
        if (!sprite) continue;

        // If the button is disabled, skip
        if (!button->isEnabled)
        {
            continue;
        }

        SpriteRendererComponent* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityId);

        if (spriteRenderer)
        {
            if (!spriteRenderer->isVisible) continue;
        }

		if (DuckEngine::DUCKENGINE_EntityManager.ShouldSkipDueToInvisibleAncestor(entityId)) 
        {
			continue;
		}

        // Retrieve position and scale from TransformComponent
        Vector2D position = sprite->GetPosition();
        Vector2D scale = sprite->scale;

        // Determine the mouse position in the correct coordinate space
        Vector2D finalMousePos;

        if (sprite->relativeToCamera)
        {
            // -- WORLD BUTTON --
            if (DuckEngine::isEditor)
            {
                // If in editor, we might want to require "playing" mode?
                // (Your code checks if not playing => return.)
                // We'll replicate that logic:
                if (!DuckEngine::IsPlaying())
                    return;

                // Use your editor's world position
                finalMousePos = DuckEngine::editorMouseWorldPos;
            }
            else
            {
                // Normal game mode: convert screen to world
                Vector2D mousePosScreen = {
                    static_cast<float>(DuckEngine_Input::GetMouseX()),
                    static_cast<float>(DuckEngine_Input::GetMouseY())
                };
                finalMousePos = GraphicsManager::ScreenToWorld(mousePosScreen);
            }
        }
        else
        {
            float mouseX = 0;
            float mouseY = 0;

            if (DuckEngine::isEditor) {
                // Get the mouse position in screen coordinates
                mouseX = DuckEngine::editorMouseScreenPos.x;
                mouseY = DuckEngine::editorMouseScreenPos.y;
            }
            else {
                mouseX = static_cast<float>(DuckEngine_Input::GetMouseX());
                mouseY = static_cast<float>(DuckEngine_Input::GetMouseY());
            }

            // Normalize the mouse position to the button coordinate system
            // Assuming screen width and height are known or accessible
            float screenWidth = static_cast<float>(WindowManager::GetViewportWidth());
            float screenHeight = static_cast<float>(WindowManager::GetViewportHeight());

            // Normalize mouse coordinates to [0, 1] for X and [0, -1] for Y
            // Mouse X normalization (left = 0, right = 1)
            float normalizedMouseX = mouseX / screenWidth; // Range: [0, 1]

            // Mouse Y normalization (top = 0, bottom = -1)
            float normalizedMouseY = -(mouseY / screenHeight); // Range: [0, -1]

            // Store the normalized mouse position
            finalMousePos = { normalizedMouseX, normalizedMouseY };

            //frameCounter++;

            //if (frameCounter >= 100) {

            //    Vector2D min = position - scale * 0.5f;
            //    Vector2D max = position + scale * 0.5f;

            //    std::cout << "UI Button Debug:\n";
            //    std::cout << "  Mouse Pos (normalized): (" << finalMousePos.x << ", " << finalMousePos.y << ")\n";
            //    std::cout << "  Button Pos (normalized): (" << position.x << ", " << position.y << ")\n";
            //    std::cout << "  Button Min: (" << min.x << ", " << min.y << ")\n";
            //    std::cout << "  Button Max: (" << max.x << ", " << max.y << ")\n";
            //    std::cout << "----------------------------------\n";
            //    std::cout << "editor mouse screen pos: " << DuckEngine::editorMouseScreenPos.x << ", " << DuckEngine::editorMouseScreenPos.y << "\n";

            //    frameCounter = 0; // Reset counter after printing
            //}
        }

        // Check hover
        bool isCurrentlyHovered = IsPointInside(finalMousePos, position, scale);
        if (isCurrentlyHovered && !button->isHovered)
        {
            if (button->onHover) {
                button->onHover();
            }
            button->isHovered = true;
        }
        else if (!isCurrentlyHovered && button->isHovered)
        {
            if (button->onFinishHover) {
                button->onFinishHover();
            }
            button->isHovered = false;
        }

        // Check click
        if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
        {
            if (IsPointInside(finalMousePos, position, scale))
            {
                if (button->onClick)
                {
                    // We'll push it into the array so we can handle topmost
                    ClickedButtons.push_back(*button);
                }
            }
        }
    }

    // If multiple buttons were clicked, pick the topmost
    if (!ClickedButtons.empty())
    {
        int buttonIndexToClick = 0;
        int currentIndex = 0;
        int topSortingOrder = -999999;

        for (auto& btn : ClickedButtons)
        {
            auto* spriteRenderer =
                DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(
                    btn.GetEntityID());
            if (spriteRenderer && spriteRenderer->sortingOrder > topSortingOrder)
            {
                topSortingOrder = spriteRenderer->sortingOrder;
                buttonIndexToClick = currentIndex;
            }
            currentIndex++;
        }

        // Call onClick() for the top button
        ClickedButtons[buttonIndexToClick].onClick();
    }
}
