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

void FontManager::LoadFont(const std::string& fontName, const std::string& fontPath, int fontSize) 
{
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

void FontManager::AddToDrawQueue(TextRenderCommand& drawOptions) {
    drawQueue.emplace_back(drawOptions);
}

void FontManager::Update() 
{
    // Currently nothing here
}

void FontManager::Render() 
{
    GraphicsManager::BindFBO();

    auto shader = ShaderManager::GetShader("TextShader");
    shader->Use();

    // Enable alpha blend for text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Bind our text VAO
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);

    // For each text render command:
    for (auto& text : drawQueue)
    {
        // 1) Figure out which projection/view to use
        glm::mat4 projection(1.0f);

        if (text.isUI)
        {
            float w = (float)WindowManager::GetWindowWidth();
            float h = (float)WindowManager::GetWindowHeight();

            // Increase text scale since world scale is much smaller
            text.scale = text.scale * 40.f;

            // Orthographic with Y increasing upward from bottom to top
            projection = glm::ortho(0.0f, w, 0.0f, h, -1.0f, 1.0f);
        }
        else
        {
            // -- World text: center-based ortho, but offset by camera
            float virtualHeight = CameraManager::GetHeight();
            float ar            = CameraManager::GetAR();
            float virtualWidth  = virtualHeight * ar;

            // Standard centered ortho
            glm::mat4 orthoMat = glm::ortho(
                -virtualWidth * 0.5f,  virtualWidth * 0.5f,
                -virtualHeight * 0.5f, virtualHeight * 0.5f,
                -1.0f, 1.0f
            );

            // Then apply camera offset (i.e. a "view" transform).
            auto camPos = CameraManager::GetPosition(); 
            glm::mat4 view = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(-camPos.x, -camPos.y, 0.0f)
            );

            // Combine them
            projection = orthoMat * view;
        }

        // 2) Send that matrix to the shader
        GLint projLoc = glGetUniformLocation(shader->GetProgram(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 3) Set the text color
        GLint colorLoc = glGetUniformLocation(shader->GetProgram(), "textColor");
        glUniform4f(
            colorLoc,
            text.color.r / 255.f, 
            text.color.g / 255.f,
            text.color.b / 255.f, 
            text.color.a / 255.f
        );

        // Grab the font for the text
        const auto& fontMap = Fonts[text.fontName];
        float x = text.position.x;
        float y = text.position.y;

        // Render each character
        for (char c : text.text)
        {
            // If this character doesn't exist in the font, skip
            if (fontMap.find(c) == fontMap.end())
                continue;

            const Character& ch = fontMap.at(c);

            float xpos = x + ch.Bearing.x * text.scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * text.scale;

            float w = ch.Size.x * text.scale;
            float h = ch.Size.y * text.scale;

            // Update the VBO memory with the new quad
            GLfloat vertices[6][4] = {
                //  x      y        u     v
                { xpos,     ypos + h,  0.0f, 0.0f },
                { xpos,     ypos,      0.0f, 1.0f },
                { xpos + w, ypos,      1.0f, 1.0f },

                { xpos,     ypos + h,  0.0f, 0.0f },
                { xpos + w, ypos,      1.0f, 1.0f },
                { xpos + w, ypos + h,  1.0f, 0.0f }
            };

            // Bind the character's glyph texture
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            // Update content of VBO
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            // Draw the two triangles
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Advance x
            x += (ch.Advance >> 6) * text.scale;  // Advance is in 1/64 pixels
        }
    }

    // Cleanup
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    drawQueue.clear();
    GraphicsManager::UnbindFBO();
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

    FT_Done_FreeType(ft);
}
