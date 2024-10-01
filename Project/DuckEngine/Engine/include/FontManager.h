#pragma once

// Export/Import macro for DLL
//#ifdef DUCKENGINE_EXPORTS
//#define DUCKENGINE_API __declspec(dllexport)
//#else
//#define DUCKENGINE_API __declspec(dllimport)
//#endif

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

class FontManager {
public:
    // Core functions
    static bool Initialize(std::string fontPath, int fontSize);
    static void Exit();
    static void Update();
    static void Render();

    static void AddToDrawQueue(TextRenderCommand& drawOptions);

private:
    // Structure to hold information about each character glyph
    struct Character {
        unsigned int TextureID;     // ID handle of the glyph texture
        glm::ivec2 Size;            // Size of glyph
        glm::ivec2 Bearing;         // Offset from baseline to left/top of glyph
        unsigned int Advance;       // Offset to advance to next glyph
    };

    // Map of characters for quick lookup
    static std::map<GLchar, Character> Characters;

    static std::vector<TextRenderCommand> drawQueue;

    // FreeType library and face
    static FT_Library ft;
    static FT_Face face;

    // OpenGL handles
    static unsigned int VAO;
    static unsigned int VBO;
};