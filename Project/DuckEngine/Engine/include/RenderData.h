#pragma once

#include <string>

#include "Vector2.h"
#include "Color.h"

struct TextRenderCommand {

    std::string text;
    Vec2 position;
    float scale;
    Color color;

    // Constructor with default values using Color struct
    TextRenderCommand(const std::string& txt = "",
        float posX = 0.0f, float posY = 0.0f,
        float scl = 1.0f, Color clr = Color{ 255.f, 255.f, 255.f, 255.f })
        : text(txt), position(posX, posY), scale(scl), color(clr) {}

    // Overloaded constructor with 3 floats for RGB, default alpha
    TextRenderCommand(const std::string& txt,
        float posX, float posY,
        float scl, float r, float g, float b, float a)
        : text(txt), position(posX, posY), scale(scl), color(r, g, b, a) {}

    // Overloaded constructor with 3 floats for RGB, default alpha + using vec2
    TextRenderCommand(const std::string& txt,
        Vec2 position,
        float scl, float r, float g, float b, float a)
        : text(txt), position(position), scale(scl), color(r, g, b, a) {}

    // Overloaded constructor using Color struct + using vec2
    TextRenderCommand(const std::string& txt,
        Vec2 position,
        float scl, Color clr = Color{ 255.f, 255.f, 255.f, 255.f })
        : text(txt), position(position), scale(scl), color(clr) {}

    // Destructor
    ~TextRenderCommand() {}
};

struct DrawOptions {
    // NECESSARY
    Vector2D scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    Vector2D translation = { 0.0f, 0.0f };

    // OPTIONAL
    bool useColor = false;
    Color color = { 255, 255, 255, 255 }; // Default white color

    // OPTIONAL
    bool useTexture = false;
    unsigned int* texture = 0;

    // OPTIONAL
    bool relativeToCamera = false;
    // GRAB FROM CAMERA MANAGER
    //Vector2D cameraPosition = { 0.f, 0.f };
    //Vector2D cameraWidthHeight = { 0.f, 0.f };
};

struct DebugDrawCommand {
    enum Type { POINT, LINE, RECTANGLE, CIRCLE } type;

    Vector2D position1;      // For POINT, RECTANGLE (center), CIRCLE (center)
    Vector2D position2;      // For LINE (end position), RECTANGLE (size)
    float sizeOrRadius;      // For POINT (size), CIRCLE (radius)
    float rotation;          // For RECTANGLE (rotation in degrees or radians)
    Color color;             // RGBA color
    bool relativeToCamera;

    // Constructor
    DebugDrawCommand(Type t, const Vector2D& pos1, const Vector2D& pos2, float sizeOrRadius, float rotation, const Color& color, bool relativeToCamera = true)
        : type(t), position1(pos1), position2(pos2), sizeOrRadius(sizeOrRadius), rotation(rotation), color(color), relativeToCamera(relativeToCamera)
    {
        switch (type) {
        case POINT:
            // For POINT, position1 is the position, sizeOrRadius is the size
            position2 = { 0.f, 0.f };  // Unused
            rotation = 0.f;            // No rotation for POINT
            break;

        case LINE:
            // For LINE, position1 is the start, position2 is the end
            sizeOrRadius = 0.f;        // Unused
            rotation = 0.f;            // No rotation for LINE
            break;

        case RECTANGLE:
            // For RECTANGLE, position1 is the center, position2 is the size (width, height), rotation is used
            sizeOrRadius = 0.f;        // Unused
            break;

        case CIRCLE:
            // For CIRCLE, position1 is the center, sizeOrRadius is the radius
            position2 = { 0.f, 0.f };  // Unused
            rotation = 0.f;            // No rotation for CIRCLE
            break;
        }
    }
};
