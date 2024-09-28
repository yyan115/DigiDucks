//#pragma once
//
//#include "System.h"
//#include "DuckEngine.h"
//#include "TextRendererComponent.h"  // Include the text renderer component
//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//// Export/Import macro
//#ifdef DUCKENGINE_EXPORTS
//#define DUCKENGINE_API __declspec(dllexport)
//#else
//#define DUCKENGINE_API __declspec(dllimport)
//#endif
//
//// TextRendererSystem class
//class DUCKENGINE_API TextRendereringSystem : public System
//{
//public:
//    // Overrides the base system update function
//    void Update() override;
//
//    // Loads the font and renders the text
//    bool LoadFontAndRenderText(TextRendererComponent& textRenderer);
//
//    // Loads a single glyph and generates a texture
//    bool LoadGlyph(FT_Face& face, char character, TextRendererComponent& textRenderer);
//    // FreeType library instance
//    FT_Library ft;
//
//    bool CreateTextureFromBitmap(FT_Bitmap* bitmap);
//};

#pragma once

#include "TextRendererComponent.h"
#include "GraphicsManager.h"  
#include "ft2build.h"
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "System.h"
#include "DuckEngine.h"


#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

// Structure to hold the data of each character
struct Character {
    GLuint textureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};

class DUCKENGINE_API TextRenderingSystem : public System {
public:
    TextRenderingSystem();
    ~TextRenderingSystem();

    // Initializes the system and FreeType
    bool Init();

    void Start() override;

    // Updates the system (renders all TextRendererComponents)
    void Update() override;

    // Loads and generates a font atlas from the specified font
    bool GenerateFontAtlas(TextRendererComponent& textRenderer);

    // Renders the text for a given entity and component
    void RenderText();

private:
    FT_Library ft;  // FreeType library
    GLuint VAO, VBO; // Vertex Array and Buffer Objects for rendering
    std::map<char, Character> characters;  // Map of loaded characters

    // Helper to load individual characters into the atlas
    bool LoadCharacter(FT_Face& face, char c);

    // Shader uniform locations
    GLuint shaderProgram;
    GLint uniformTextColor, uniformProjectionMatrix;

    // Utility functions to setup rendering objects and shaders
    void SetupRenderObjects();
    void LoadShaders();
};
