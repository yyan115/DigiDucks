/******************************************************************************/
/*!
\file       TextSystem.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       November 7 2024
\brief      Implements the TextSystem class, responsible for rendering text
			components and managing text properties in the engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "TextSystem.h"
#include "TextComponent.h"
#include "DuckEngine_Input.h"
#include "FontManager.h"

void TextSystem::Start()
{

}

void TextSystem::Update()
{

}

float CalculateTextWidth(const std::string& fontName, const std::string& text, float scale)
{
    float totalWidth = 0.0f;

    auto fontIt = FontManager::Fonts.find(fontName);
    if (fontIt == FontManager::Fonts.end())
    {
        std::cerr << "Font not found: " << fontName << std::endl;
        return 0.0f;
    }

    const auto& font = fontIt->second;

    for (const char& c : text)
    {
        auto charIt = font.find(c);
        if (charIt == font.end())
        {
            std::cerr << "Character not found in font: " << fontName << ", character: " << c << std::endl;
            continue;
        }

        const auto& character = charIt->second;
        float advance = (character.Advance >> 6) * scale; // Correct scaling
        totalWidth += advance;
    }

    return totalWidth;
}

float CalculateTextHeight(const std::string& fontName, const std::string& text, float scale)
{
    float maxAscent = 0.0f;
    float maxDescent = 0.0f;

    auto fontIt = FontManager::Fonts.find(fontName);
    if (fontIt == FontManager::Fonts.end())
    {
        std::cerr << "Font not found: " << fontName << std::endl;
        return 0.0f;
    }

    const auto& font = fontIt->second;

    for (const char& c : text)
    {
        auto charIt = font.find(c);
        if (charIt == font.end())
        {
            std::cerr << "Character not found in font: " << fontName << ", character: " << c << std::endl;
            continue;
        }

        const auto& character = charIt->second;

        float ascent = character.Bearing.y * scale;
        float descent = (character.Size.y - character.Bearing.y) * scale;

        if (ascent > maxAscent)
            maxAscent = ascent;
        if (descent > maxDescent)
            maxDescent = descent;
    }

    return maxAscent + maxDescent;
}


Vector2D GetTextSize(const std::string& fontName, const std::string& text, float scale)
{
    float width = CalculateTextWidth(fontName, text, scale);
    float height = CalculateTextHeight(fontName, text, scale);
    return Vector2D(width, height);
}

void TextSystem::Render()
{
    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextComponent>())
    {
        TextComponent* text = static_cast<TextComponent*>(component.get());

        if (!text->isEnabled)
            continue;

        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
        if (!transform)
            continue;

        // skip if no sprite renderer
        //auto sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityId);
        //if (!sprite)
        //    continue;

        //// skip if sprite disabled
        //if (!sprite->isVisible)
        //    continue;

        Vector2D position = transform->GetPosition();
        float scale = static_cast<float>(text->fontSize) * 0.03f;

        // Calculate text size
        Vector2D textSize = GetTextSize(text->fontName, text->text, scale);

        // Adjust position to center the text
        Vector2D centeredPosition = position - (textSize * 0.5f);

        // Debugging outputs
        // std::cout << "Text Size: " << textSize.x << ", " << textSize.y << std::endl;
        // std::cout << "Centered Position: " << centeredPosition.x << ", " << centeredPosition.y << std::endl;

        DuckEngine::RenderText(text->fontName, text->text, centeredPosition, scale, text->color, transform->relativeToCamera);
    }
}


