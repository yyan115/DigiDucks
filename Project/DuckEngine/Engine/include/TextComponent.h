#pragma once
#include <functional>

#include "Component.h"
#include "Vector2.h"
#include "FontManager.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class TextComponent : public Component
{
public:

    std::string text;

    Vector2D position;

    int fontSize;

    Color color;

    // currently only one font is supported, need to refactor
    //std::shared_ptr<Font> font;

    bool isEnabled;

    //// Optional: Enum for alignment if desired
    //enum class Alignment { Left, Center, Right };
    //Alignment alignment = Alignment::Left;

    DUCKENGINE_API TextComponent(
        const std::string& text = "",
        const Vector2D& position = Vector2D(),
        int fontSize = 12,
        const Color& color = Color(255, 255, 255),
        //std::shared_ptr<Font> font = nullptr,
        bool enabled = true
    )
        : text(text), position(position), fontSize(fontSize), color(color), isEnabled(enabled) {}

    DUCKENGINE_API std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<TextComponent>(*this);
    }
};