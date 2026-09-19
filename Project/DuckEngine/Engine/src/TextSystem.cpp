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
#include "WindowManager.h"
#include "GraphicsManager.h"

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

        int orderInt = DuckEngine::DUCKENGINE_SceneManager.GetActiveScene()->GetOrderFromEntityID(entityId);

        auto entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityId);
        auto* activeScene = DuckEngine::DUCKENGINE_SceneManager.GetActiveScene();
        Layer* currentLayer = activeScene->GetLayer(entity.get()->layerName);

        if (!currentLayer->IsVisible())
            continue;

		if (DuckEngine::DUCKENGINE_EntityManager.ShouldSkipDueToInvisibleAncestor(entityId))
		{
			continue;
		}

        // skip if no sprite renderer
        //auto sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityId);
        //if (!sprite)
        //    continue;

        //// skip if sprite disabled
        //if (!sprite->isVisible)
        //    continue;

        float scale;
        float scaleY;
        Vector2D renderPosition;

        if (!transform->relativeToCamera) {
            // Use a constant reference height (e.g., 1080) to compute normalized scale.
            float referenceHeight = 1080.f;
            // AtlasScale cancels the glyph atlas resolution, so raising it
            // sharpens the text without resizing it. See FontManager.h.
            scale = text->fontSize / referenceHeight * FontManager::AtlasScale;
            renderPosition = transform->GetPosition();
            //DuckEngine::RenderText(text->fontName, text->text, transform->GetPosition(), normalizedScale, text->color, transform->relativeToCamera);

            if (text->fontSizeY != -1.f){
                scaleY = text->fontSizeY / referenceHeight * FontManager::AtlasScale;
            }
            else {
                scaleY = scale;
            }

            // Interface text is placed in a space that runs 0 to 1 across the
            // viewport's width and 0 to 1 down its height, so one unit across
            // is wider than one unit down by the viewport's aspect ratio. A
            // glyph scaled the same both ways therefore came out stretched
            // sideways by exactly that ratio, 1.78 on a 16:9 screen, which is
            // why every line of interface text looked wide. Narrowing the
            // horizontal scale by the aspect ratio draws each glyph in the
            // shape its font gives it, whatever shape the screen is.
            const float viewportWidth = DuckEngine::GetViewportWidth();
            const float viewportHeight = DuckEngine::GetViewportHeight();
            if (viewportWidth > 0.f && viewportHeight > 0.f) {
                scale *= viewportHeight / viewportWidth;
            }

            // A line of interface text that asks to be centred is centred
            // across on its x. Its y stays the baseline, since that is what a
            // line of interface text is laid out against. Without this a
            // centred line had to be placed by hand from its left edge, and
            // moved again whenever its wording or size changed.
            if (text->centered) {
                renderPosition.x -= CalculateTextWidth(text->fontName, text->text, scale) * 0.5f;
            }
        }
        else {
            Vector2D position = transform->GetPosition();
            scale = static_cast<float>(text->fontSize) * 0.03f * FontManager::AtlasScale;
            scaleY = scale;

            // Calculate text size
            Vector2D textSize = GetTextSize(text->fontName, text->text, scale);

            // Adjust position to center the text
            renderPosition = position - (textSize * 0.5f);

            // Debugging outputs
            // std::cout << "Text Size: " << textSize.x << ", " << textSize.y << std::endl;
            // std::cout << "Centered Position: " << centeredPosition.x << ", " << centeredPosition.y << std::endl;

            //DuckEngine::RenderText(text->fontName, text->text, centeredPosition, scale, text->color, transform->relativeToCamera);
        }

        //float yScale = -1.f;

        //if (text->text.find("Score more") != std::string::npos) {
        //    yScale = scale * 1.5f;

        //}

        // The world-space branch above computes a centred origin and the draw
        // command used to be handed the raw transform instead, so the centring
        // was calculated every frame and thrown away: world text was drawn from
        // its left edge. Every level-select number in MainMenu.json carries a
        // different hand-tuned offset because of it, and the result screen's
        // score line moved sideways whenever the score gained a digit.
        //
        // Honouring it for everything would shift all of that hand-tuned text
        // by half its width, so it is opt-in. A string that asks to be centred
        // gets the centred origin; everything that shipped keeps the position
        // it was laid out against.
        const Vector2D drawAt = text->centered ? renderPosition : transform->GetPosition();
        TextRenderCommand TextDrawCommand (text->fontName, text->text, drawAt, scale, text->color, transform->relativeToCamera, scaleY);

        GraphicsManager::AddToDrawQueue({ orderInt, text->sortingOrder, RenderCommandType::Text, TextDrawCommand });
    }
}


