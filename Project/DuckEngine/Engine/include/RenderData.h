#pragma once

#include <string>

#include "Vector2.h"
#include "Color.h"

/// <summary>
/// Represents a command to render text on the screen. It stores the text, position, scale, and color.
/// Multiple constructors allow flexibility in specifying text properties.
/// </summary>
struct TextRenderCommand {
    
    // Text to actually render
    std::string text;

    // Position of text
    Vec2 position;

    // Scale of text
    float scale;

    // Color of text
    Color color;

    /// <summary>
    /// Constructs a TextRenderCommand with specified text, position, scale, and color.
    /// Defaults to a white color.
    /// </summary>
    TextRenderCommand(const std::string& txt = "",
        float posX = 0.0f, float posY = 0.0f,
        float scl = 1.0f, Color clr = Color{ 255.f, 255.f, 255.f, 255.f })
        : text(txt), position(posX, posY), scale(scl), color(clr) {}

    /// <summary>
    /// Constructs a TextRenderCommand using RGB values with default alpha.
    /// </summary>
    TextRenderCommand(const std::string& txt,
        float posX, float posY,
        float scl, float r, float g, float b, float a)
        : text(txt), position(posX, posY), scale(scl), color(r, g, b, a) {}

    /// <summary>
    /// Constructs a TextRenderCommand using a Vec2 for position and RGB values with default alpha.
    /// </summary>
    TextRenderCommand(const std::string& txt,
        Vec2 position,
        float scl, float r, float g, float b, float a)
        : text(txt), position(position), scale(scl), color(r, g, b, a) {}

    /// <summary>
    /// Constructs a TextRenderCommand using a Vec2 for position and a Color struct for color.
    /// Defaults to a white color.
    /// </summary>
    TextRenderCommand(const std::string& txt,
        Vec2 position,
        float scl, Color clr = Color{ 255.f, 255.f, 255.f, 255.f })
        : text(txt), position(position), scale(scl), color(clr) {}

    /// <summary>
    /// Destructor for TextRenderCommand.
    /// </summary>
    ~TextRenderCommand() {}
};

/// <summary>
/// Represents options for drawing an object on the screen, including scale, rotation, translation, and color.
/// Also provides options for using textures and whether the object is relative to the camera.
/// </summary>
struct DrawOptions {

    // NECESSARY (SCALE, ROTATE, TRANSLATE)
    Vector2D scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    Vector2D translation = { 0.0f, 0.0f };

    // OPTIONAL
    bool useColor = false;
    Color color = { 255, 255, 255, 255 };

    // OPTIONAL
    bool useTexture = false;
    unsigned int* texture = 0;

    // OPTIONAL
    bool relativeToCamera = false;
};

/// <summary>
/// Represents a command to draw a debug shape on the screen, such as a point, line, rectangle, or circle.
/// Includes properties such as position, size, rotation, color, and camera relativity.
/// </summary>
struct DebugDrawCommand {
    enum Type { POINT, LINE, RECTANGLE, CIRCLE } type;

    Vector2D position1;      // For POINT, RECTANGLE (min), CIRCLE (center)
    Vector2D position2;      // For LINE (end position), RECTANGLE (max)
    float sizeOrRadius;      // For POINT (size), CIRCLE (radius)
    float rotation;          // For RECTANGLE (rotation in degrees or radians)
    Color color;             // RGBA color
    bool relativeToCamera;   // If relative to camera or not

    /// <summary>
    /// Constructs a DebugDrawCommand with specified parameters for drawing a debug shape.
    /// Initializes the appropriate fields based on the type of shape being drawn.
    /// </summary>
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
            // For RECTANGLE, position1 is the min, position2 is the max, rotation is used
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
