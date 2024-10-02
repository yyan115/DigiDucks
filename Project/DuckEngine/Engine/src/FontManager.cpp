#include "FontManager.h"
#include <iostream>
#include "ShaderManager.h"
#include "WindowManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Map of characters for quick lookup
std::map<GLchar, FontManager::Character> FontManager::Characters;

std::vector<TextRenderCommand> FontManager::drawQueue;

// FreeType library and face
FT_Library FontManager::ft;
FT_Face FontManager::face;

// OpenGL handles
unsigned int FontManager::VAO;
unsigned int FontManager::VBO;

void FontManager::Initialize(std::string fontPath, int fontSize) {
    // Initialize FreeType library
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";
        // Handle error appropriately
    }

    // Compile and setup the shader
    ShaderManager::InsertShader("TextShader", "../Resources/Shaders/DebugVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load font face
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << "\n";
        return;
    }
    // Set font size
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Generate character textures
    Characters.clear();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character " << c << "\n";
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED, // We use GL_RED since we're dealing with grayscale
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents artifacts when scaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // Smooth edges
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));

        //if (face->glyph->bitmap.width == 0 || face->glyph->bitmap.rows == 0) {
        //    std::cerr << "WARNING: Glyph for character " << c << " has zero width or height.\n";
        //}

    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Initialize OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and configure VBO and VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // We are using dynamic draw because we'll update the content every frame
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // Vertex attributes (position and texture coordinates)
    glEnableVertexAttribArray(0);
    // Each vertex has 4 floats: x, y, u, v
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up the projection matrix
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WindowManager::GetWindowWidth()), // Replace 800 with your screen width
        0.0f, static_cast<GLfloat>(WindowManager::GetWindowHeight())); // Replace 600 with your screen height
    //glUseProgram();
    ShaderManager::GetShader("TextShader")->Use();
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::GetShader("TextShader")->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // **Set the 'text' sampler uniform to texture unit 0**
    glUniform1i(glGetUniformLocation(ShaderManager::GetShader("TextShader")->GetProgram(), "text"), 0);
}

void FontManager::Exit() {
    // Clean up textures
    for (auto& pair : Characters) {
        glDeleteTextures(1, &pair.second.TextureID);
    }
    // Delete VAO and VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Clean up FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void FontManager::AddToDrawQueue(TextRenderCommand& drawOptions) {
    drawQueue.emplace_back(drawOptions);
}

void FontManager::Update() {



    //for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextRendererComponent>()) {
    //    auto textRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextRendererComponent>(entityId);

    //    std::cout << "1 TEXT RENDERER FOUND\n";

    //    if (textRenderer) {
    //        RenderText(*textRenderer);
    //    }
    //}

    //std::cout << "UPDATE\n";
}

void FontManager::Render() {

    for (auto& text : drawQueue) {
        glDisable(GL_DEPTH_TEST);

        // Activate corresponding render state	
        //glUseProgram(shaderProgram);
        ShaderManager::GetShader("TextShader")->Use();
        glUniform4f(glGetUniformLocation(ShaderManager::GetShader("TextShader")->GetProgram(), "textColor"), text.color.r / 255.f,
                                                                                                             text.color.g / 255.f,
                                                                                                             text.color.b / 255.f,
                                                                                                             text.color.a / 255.f);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters in the text
        std::string::const_iterator c;
        for (c = text.text.begin(); c != text.text.end(); c++) {
            Character ch = Characters[*c];

            float xpos = text.position.x + ch.Bearing.x * text.scale;
            float ypos = text.position.y - (ch.Size.y - ch.Bearing.y) * text.scale;

            float w = ch.Size.x * text.scale;
            float h = ch.Size.y * text.scale;

            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f }, // Top-left
                { xpos,     ypos,       0.0f, 1.0f }, // Bottom-left
                { xpos + w, ypos,       1.0f, 1.0f }, // Bottom-right

                { xpos,     ypos + h,   0.0f, 0.0f }, // Top-left
                { xpos + w, ypos,       1.0f, 1.0f }, // Bottom-right
                { xpos + w, ypos + h,   1.0f, 0.0f }  // Top-right
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Advance cursor for next glyph
            text.position.x += (ch.Advance >> 6) * text.scale; // Bitshift by 6 to get value in pixels (1/64 pixels per unit)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    drawQueue.clear();
}