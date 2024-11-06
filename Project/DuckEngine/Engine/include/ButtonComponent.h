/******************************************************************************/
/*!
\file       ButtonComponent.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 7 2024
\brief      Declares the ButtonComponent class, providing properties and state
            management for UI buttons in the engine, such as hover and click.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <functional>

#include "Component.h"
#include "Vector2.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

// ONLY SUPPORT RECTANGLE BUTTONS AS OF NOW
class ButtonComponent : public Component
{
public:

    Vector2D minPos, maxPos;

    // To be decided if needed/will be implemented
    //std::shared_ptr<Texture> texture;

    // To be decided if needed/will be implemented
    //Color color;
    //Color hoverColor;
    //Color textColor;

    bool isHovered = false;
    bool isPressed = false;

    bool isEnabled = true;

    // Callback functions
    std::function<void()> onClick;
    std::function<void()> onHover;

    /// <summary>
    /// Constructs a ButtonComponent with specified minimum and maximum positions 
    /// and an enabled state.
    /// </summary>
    /// <param name="min">The minimum position (bottom-left corner) of the button.</param>
    /// <param name="max">The maximum position (top-right corner) of the button.</param>
    /// <param name="enabled">Determines if the button is enabled or disabled.</param>
    DUCKENGINE_API ButtonComponent(Vector2D min = Vector2D(), Vector2D max = Vector2D(), bool enabled = true) : minPos(min), maxPos(max), isEnabled(enabled) {}

    /// <summary>
    /// Creates and returns a clone of the ButtonComponent.
    /// </summary>
    /// <returns>A shared pointer to a new ButtonComponent copy.</returns>
    DUCKENGINE_API std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<ButtonComponent>(*this);
    }
};