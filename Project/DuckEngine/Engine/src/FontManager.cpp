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
FT_Library FontManager::ft;

// OpenGL handles
unsigned int FontManager::VAO;
unsigned int FontManager::VBO;

/// <summary>
/// Loads a font and stores it with the specified name, generating textures for each character.
/// </summary>
/// <param name="fontName">A unique name for the font.</param>
/// <param name="fontPath">The path to the font file.</param>
/// <param name="fontSize">The size of the font.</param>
void FontManager::LoadFont(const std::string& fontName, const std::string& fontPath, int fontSize) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << "\n";
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    std::map<GLchar, Character> characters;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character " << c << "\n";
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
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
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void FontManager::AddToDrawQueue(TextRenderCommand& drawOptions) {
    drawQueue.emplace_back(drawOptions);
}

void FontManager::Update() {

}

/// <summary>
/// Renders all text commands in the draw queue, using the specified font for each command.
/// </summary>
void FontManager::Render() {
    GraphicsManager::BindFBO();

    // Use the same projection matrix as the game objects
    float virtualHeight = CameraManager::GetHeight(); // Fixed virtual height used in your game
    float ar = CameraManager::GetAR(); // Aspect ratio
    float virtualWidth = virtualHeight * ar;

    // Create the projection matrix
    glm::mat4 projection = glm::ortho(
        0.0f, virtualWidth,
        0.0f, virtualHeight
    );

    // Get the shader used by game objects
    auto shader = ShaderManager::GetShader("TextShader");
    shader->Use();

    // Set the projection matrix uniform
    glUniformMatrix4fv(
        glGetUniformLocation(shader->GetProgram(), "projection"),
        1, GL_FALSE, glm::value_ptr(projection)
    );

    // Prepare for rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Conversion factor from pixels to world units
    float pixelsPerUnit = WindowManager::GetWindowHeight() / virtualHeight;

    for (auto& text : drawQueue) {
        // Set the text color
        glUniform4f(
            glGetUniformLocation(shader->GetProgram(), "textColor"),
            text.color.r / 255.f, text.color.g / 255.f,
            text.color.b / 255.f, text.color.a / 255.f
        );

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);

        const auto& font = Fonts[text.fontName];
        float x = text.position.x / pixelsPerUnit;
        float y = text.position.y / pixelsPerUnit;

        for (const char& c : text.text) {
            Character ch = font.at(c);

            float xpos = x + (ch.Bearing.x * text.scale) / pixelsPerUnit;
            float ypos = y - ((ch.Size.y - ch.Bearing.y) * text.scale) / pixelsPerUnit;
            float w = (ch.Size.x * text.scale) / pixelsPerUnit;
            float h = (ch.Size.y * text.scale) / pixelsPerUnit;

            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h, 0.0f, 0.0f },
                { xpos,     ypos,     0.0f, 1.0f },
                { xpos + w, ypos,     1.0f, 1.0f },

                { xpos,     ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos,     1.0f, 1.0f },
                { xpos + w, ypos + h, 1.0f, 0.0f }
            };

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Advance cursors for next glyph
            x += (ch.Advance >> 6) * text.scale / pixelsPerUnit;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    drawQueue.clear();
    GraphicsManager::UnbindFBO();
}


/// <summary>
/// Cleans up the loaded fonts and FreeType resources.
/// </summary>
void FontManager::Exit() {
    for (auto& font : Fonts) {
        for (auto& pair : font.second) {
            glDeleteTextures(1, &pair.second.TextureID);
        }
    }
    Fonts.clear();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    FT_Done_FreeType(ft);
}
