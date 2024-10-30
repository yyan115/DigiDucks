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

    // probably dont need/shouldnt have texture
    //std::shared_ptr<Texture> texture;

    // not sure if color is needed either
    //Color color;
    //Color hoverColor;
    //Color textColor;

    bool isHovered = false;
    bool isPressed = false;

    bool isEnabled = true;

    // Callback functions
    std::function<void()> onClick;
    std::function<void()> onHover;

    DUCKENGINE_API ButtonComponent(Vector2D min = Vector2D(), Vector2D max = Vector2D(), bool enabled = true) : minPos(min), maxPos(max), isEnabled(enabled) {}

    DUCKENGINE_API std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<ButtonComponent>(*this);
    }
};