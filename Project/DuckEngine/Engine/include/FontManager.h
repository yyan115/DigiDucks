/******************************************************************************/
/*!
\file       FontManager.h
\author     Jovan chua
\par        email c.shengkaijovan@digipen.edu
\date       October 3 2024
\brief      Declares the FontManager class, responsible for managing font
            rendering using FreeType and OpenGL. This class handles the loading
            of fonts, generating character textures, and rendering text to the
            screen.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Component.h"
#include <string>
#include <map>
#include <vector>
#include "Vector2.h"
#include "RenderData.h"
#include "Color.h"
#include <ft2build.h>
#include FT_FREETYPE_H

/// <summary>
/// Manages font rendering and text drawing in the engine using FreeType and OpenGL.
/// Responsible for loading fonts, handling text rendering, and managing draw queues for text.
/// </summary>
class FontManager {
public:
    /// <summary>
    /// Initializes the FontManager by loading a font from the specified file and setting the font size.
    /// Also initializes the FreeType library and sets up OpenGL buffers for text rendering.
    /// </summary>
    /// <param name="fontPath">The path to the font file (e.g., .ttf).</param>
    /// <param name="fontSize">The size of the font in pixels.</param>
    static void Initialize(std::string fontPath, int fontSize);

    /// <summary>
    /// Cleans up and exits the FontManager by releasing the FreeType library, deleting OpenGL buffers,
    /// and clearing any loaded font data.
    /// </summary>
    static void Exit();

    /// <summary>
    /// Updates any state in the FontManager, if needed. This function is a placeholder for potential future updates.
    /// </summary>
    static void Update();

    /// <summary>
    /// Renders all text in the draw queue. This function is called each frame to draw text on the screen.
    /// </summary>
    static void Render();

    /// <summary>
    /// Adds a text render command to the draw queue for rendering in the next frame.
    /// </summary>
    /// <param name="drawOptions">The TextRenderCommand object containing the text, position, scale, and color.</param>
    static void AddToDrawQueue(TextRenderCommand& drawOptions);

private:
    /// <summary>
    /// Represents information about a single character glyph, including its texture ID, size, bearing, and advance offset.
    /// </summary>
    struct Character {
        unsigned int TextureID;  ///< The OpenGL texture ID for the glyph texture.
        glm::ivec2 Size;         ///< The size of the glyph in pixels.
        glm::ivec2 Bearing;      ///< The offset from baseline to left/top of glyph.
        unsigned int Advance;    ///< The horizontal offset to advance to the next glyph.
    };

    /// <summary>
    /// A map storing character glyphs for quick lookup based on character codes.
    /// </summary>
    static std::map<GLchar, Character> Characters;

    /// <summary>
    /// A queue that stores text render commands to be processed and rendered each frame.
    /// </summary>
    static std::vector<TextRenderCommand> drawQueue;

    /// <summary>
    /// The FreeType library instance.
    /// </summary>
    static FT_Library ft;

    /// <summary>
    /// The FreeType font face instance for the loaded font.
    /// </summary>
    static FT_Face face;

    /// <summary>
    /// The OpenGL Vertex Array Object (VAO) used for rendering text.
    /// </summary>
    static unsigned int VAO;

    /// <summary>
    /// The OpenGL Vertex Buffer Object (VBO) used for rendering text.
    /// </summary>
    static unsigned int VBO;
};
