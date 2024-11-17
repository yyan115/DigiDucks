/******************************************************************************/
/*!
\file       TextComponent.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 7 2024
\brief      Declares the TextComponent class, storing information for rendering
            text in the engine, including content, font, color, and scale.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <functional>

#include "Component.h"
#include "Vector2.h"
//#include "FontManager.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class TextComponent : public Component
{
public:

    std::string fontName;

    std::string text;

    Vector2D position;

    int fontSize;

    Color color;

    bool isEnabled;

    /// <summary>
    /// Constructs a TextComponent with specified font name, text content, position, 
    /// font size, color, and enabled state.
    /// </summary>
    /// <param name="fontName">The name of the font to be used for rendering the text.</param>
    /// <param name="text">The text content to render (default is an empty string).</param>
    /// <param name="position">The screen position where the text will be rendered.</param>
    /// <param name="fontSize">The font size for the text.</param>
    /// <param name="color">The color of the text (default is white).</param>
    /// <param name="enabled">Specifies whether the text component is enabled.</param>
    DUCKENGINE_API TextComponent(
        const std::string& fontName = "Kenney High Square",
        const std::string& text = "",
        const Vector2D& position = Vector2D(),
        int fontSize = 1,
        const Color& color = Color(255, 255, 255),
        //std::shared_ptr<Font> font = nullptr,
        bool enabled = true
    )
        : fontName(fontName), text(text), position(position), fontSize(fontSize), color(color), isEnabled(enabled) {}

    /// <summary>
    /// Creates and returns a clone of the TextComponent.
    /// </summary>
    /// <returns>A shared pointer to a new TextComponent copy.</returns>
    DUCKENGINE_API std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<TextComponent>(*this);
    }
};