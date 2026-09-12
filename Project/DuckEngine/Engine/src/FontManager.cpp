/******************************************************************************/
/*!
\file       FontManager.cpp
\author     Jovan chua 2301244 (50%)
\par        email c.shengkaijovan@digipen.edu
\author     Yan Yu 2301213 (50%)
\par        email y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the FontManager class, initializing FreeType and
            OpenGL buffers for text rendering, generating character textures
            for fonts, and managing the rendering of text commands using
            shaders.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "FontManager.h"
#include <iostream>
#include "ShaderManager.h"
#include "WindowManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GraphicsManager.h"
#include "CameraManager.h"

// Map of fonts storing characters for each font name
std::map<std::string, std::map<GLchar, FontManager::Character>> FontManager::Fonts;

std::vector<TextRenderCommand> FontManager::drawQueue;

// FreeType library
FT_Library FontManager::ft = nullptr;

// OpenGL handles
unsigned int FontManager::VAO;
unsigned int FontManager::VBO;

void FontManager::LoadFont(const std::string& fontName, const std::string& fontPath, int fontSize) 
{
    if (ft == nullptr) {
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";
            ft = nullptr;
            return;
        }
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << "\n";
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    std::map<GLchar, Character> characters;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) 
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character " << c << "\n";
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width,  face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left,   face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        characters.insert({ c, character });
    }
    Fonts[fontName] = characters;
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);

    // Initialize VAO and VBO once for all fonts
    if (VAO == 0 && VBO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                // Layout location
            4,                // vec4 (x, y, u, v)
            GL_FLOAT,
            GL_FALSE,
            4 * sizeof(GLfloat),
            (void*)0
        );

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void FontManager::Update() 
{
    // Currently nothing here
}

void FontManager::Exit() 
{
    for (auto& font : Fonts) {
        for (auto& pair : font.second) {
            glDeleteTextures(1, &pair.second.TextureID);
        }
    }
    Fonts.clear();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    VAO = 0;
    VBO = 0;

    if (ft != nullptr) {
        FT_Done_FreeType(ft);
        ft = nullptr;
    }
}
