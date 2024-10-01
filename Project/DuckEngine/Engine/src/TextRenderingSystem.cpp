//#include "DuckEngine.h"
//#include "TextRenderingSystem.h"
//#include <iostream>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <TextRendererComponent.h>
//
//#include "GraphicsManager.h"
//#include "WindowManager.h"
//#include "ShaderManager.h"
//
//void TextRenderingSystem::Start() {
//    InitPart1();
//}
//
//void TextRenderingSystem::InitPart1() {
//    // Initialize FreeType library
//    if (FT_Init_FreeType(&ft)) {
//        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";
//        // Handle error appropriately
//    }
//
//    // Compile and setup the shader
//    ShaderManager::InsertShader("TextShader", "../Resources/Shaders/testV.vert", "../Resources/Shaders/testF.frag");//CompileShader(vertexShaderSource, fragmentShaderSource);
//
//    // Enable blending for transparency
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    std::cout << "INIT PART 1\n";
//
//    if (Init("../Resources/Roboto-Black.ttf", 48)) {
//        std::cout << "TEXT INIT FAILED\n";
//    }
//}
//
//TextRenderingSystem::~TextRenderingSystem() {
//    // Clean up textures
//    for (auto& pair : Characters) {
//        glDeleteTextures(1, &pair.second.TextureID);
//    }
//    // Delete VAO and VBO
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//
//    // Clean up FreeType
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);
//
//    std::cout << "FONT FREED\n";
//}
//
//bool TextRenderingSystem::Init(const std::string& fontPath, GLuint fontSize) {
//    // Load font face
//    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
//        std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << "\n";
//        return false;
//    }
//    // Set font size
//    FT_Set_Pixel_Sizes(face, 0, fontSize);
//
//    // Generate character textures
//    Characters.clear();
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
//
//    for (GLubyte c = 0; c < 128; c++) {
//        // Load character glyph
//        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
//            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for character " << c << "\n";
//            continue;
//        }
//        // Generate texture
//        GLuint texture;
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(
//            GL_TEXTURE_2D,
//            0,
//            GL_RED, // We use GL_RED since we're dealing with grayscale
//            face->glyph->bitmap.width,
//            face->glyph->bitmap.rows,
//            0,
//            GL_RED,
//            GL_UNSIGNED_BYTE,
//            face->glyph->bitmap.buffer
//        );
//        // Set texture options
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents artifacts when scaling
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // Smooth edges
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        // Now store character for later use
//        Character character = {
//            texture,
//            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//            static_cast<GLuint>(face->glyph->advance.x)
//        };
//        Characters.insert(std::pair<GLchar, Character>(c, character));
//
//        if (face->glyph->bitmap.width == 0 || face->glyph->bitmap.rows == 0) {
//            std::cerr << "WARNING: Glyph for character " << c << " has zero width or height.\n";
//        }
//
//    }
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    // Initialize OpenGL buffers
//    InitRenderData();
//
//    // Set up the projection matrix
//    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WindowManager::GetWindowWidth()), // Replace 800 with your screen width
//        0.0f, static_cast<GLfloat>(WindowManager::GetWindowHeight())); // Replace 600 with your screen height
//    //glUseProgram();
//    ShaderManager::GetShader("TextShader")->Use();
//    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::GetShader("TextShader")->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//
//    std::cout << "INIT PART 2\n";
//
//    // **Set the 'text' sampler uniform to texture unit 0**
//    glUniform1i(glGetUniformLocation(ShaderManager::GetShader("TextShader")->GetProgram(), "text"), 0);
//
//    return true;
//}
//
//void TextRenderingSystem::InitRenderData() {
//    // Generate VAO and VBO for text quads
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    // Bind and configure VBO and VAO
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // We are using dynamic draw because we'll update the content every frame
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//
//    // Vertex attributes (position and texture coordinates)
//    glEnableVertexAttribArray(0);
//    // Each vertex has 4 floats: x, y, u, v
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    std::cout << "RENDER DATA INIT\n";
//}
//
//void TextRenderingSystem::Update() {
//
//    for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextRendererComponent>()) {
//        auto textRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextRendererComponent>(entityId);
//
//        std::cout << "1 TEXT RENDERER FOUND\n";
//
//        if (textRenderer) {
//            RenderText(*textRenderer);
//        }
//    }
//
//    std::cout << "UPDATE\n";
//    //for (auto& entity : entities) {
//    //    if (entity.HasComponent<TextRendererComponent>()) {
//    //        auto& textComp = entity.GetComponent<TextRendererComponent>();
//    //        RenderText(textComp);
//    //    }
//    //}
//}
//
//void TextRenderingSystem::RegisterEntity(Entity entity) {
//
//    auto textRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextRendererComponent>(entity.entityID);
//
//    if (textRenderer) {
//        entities.push_back(entity);
//    }
//
//    std::cout << "REGISTERED\n";
//}
//
//void TextRenderingSystem::RenderText(const TextRendererComponent& textComp) {
//
//    glDisable(GL_DEPTH_TEST);
//
//
//    // Activate corresponding render state	
//    //glUseProgram(shaderProgram);
//    ShaderManager::GetShader("TextShader")->Use();
//    glUniform3f(glGetUniformLocation(ShaderManager::GetShader("TextShader")->GetProgram(), "textColor"), 1.f, 0.f, 1.f);
//    glActiveTexture(GL_TEXTURE0);
//    glBindVertexArray(VAO);
//
//    float x = textComp.positionX;
//    float y = textComp.positionY;
//
//    std::cout << "pos: " << textComp.positionX << ", " << textComp.positionY << "\n";
//
//    // Iterate through all characters in the text
//    std::string::const_iterator c;
//    for (c = textComp.text.begin(); c != textComp.text.end(); c++) {
//        Character ch = Characters[*c];
//
//        float xpos = x + ch.Bearing.x * textComp.scale;
//        float ypos = y - (ch.Size.y - ch.Bearing.y) * textComp.scale;
//
//        float w = ch.Size.x * textComp.scale;
//        float h = ch.Size.y * textComp.scale;
//
//        // Update VBO for each character
//        GLfloat vertices[6][4] = {
//            { xpos,     ypos + h,   0.0f, 0.0f }, // Top-left
//            { xpos,     ypos,       0.0f, 1.0f }, // Bottom-left
//            { xpos + w, ypos,       1.0f, 1.0f }, // Bottom-right
//
//            { xpos,     ypos + h,   0.0f, 0.0f }, // Top-left
//            { xpos + w, ypos,       1.0f, 1.0f }, // Bottom-right
//            { xpos + w, ypos + h,   1.0f, 0.0f }  // Top-right
//        };
//        // Render glyph texture over quad
//        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//
//        // Update content of VBO memory
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//
//        // Render quad
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        // Advance cursor for next glyph
//        x += (ch.Advance >> 6) * textComp.scale; // Bitshift by 6 to get value in pixels (1/64 pixels per unit)
//
//        std::cout << " DRAWWWWWW\n";
//    }
//    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    std::cout << "RENDERTEXT CALLED\n";
//}
//
//
//
////#include "TextRenderingSystem.h"
////#include "GraphicsManager.h"  // Assuming your engine uses a GraphicsManager for drawing textures
////#include <iostream>  // For logging errors
////
////// Initializes FreeType and performs updates for each TextRendererComponent
////void TextRendereringSystem::Update() {
////    // Ensure FreeType is initialized
////    if (!ft) {
////        if (FT_Init_FreeType(&ft)) {
////            std::cerr << "Failed to initialize FreeType Library" << std::endl;
////            return;
////        }
////    }
////
////    // Iterate through all TextRendererComponents and render text
////    for (const auto& [entityId, textRenderer] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextRendererComponent>()) {
////        TextRendererComponent* textRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextRendererComponent>(entityId);
////        if (textRenderer && textRenderer->texture == NULL) {  // Only render if no texture exists yet
////            if (!LoadFontAndRenderText(*textRenderer)) {
////                std::cerr << "Error rendering text for entity " << entityId << std::endl;
////            }
////        }
////
////        // Use GraphicsManager with texture
////        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
////        if (transform && textRenderer->texture) {
////            DrawOptions drawOptions;
////            drawOptions.translation = transform->position;
////            drawOptions.scale = transform->scale;
////            drawOptions.rotation = transform->angle;
////            drawOptions.useTexture = true;
////            drawOptions.texture = &textRenderer->texture;  // The generated texture
////            drawOptions.relativeToCamera = true;
////
////            GraphicsManager::AddToDrawQueue(drawOptions);
////        }
////    }
////}
////
////// Loads the font and renders the text using FreeType
////bool TextRendereringSystem::LoadFontAndRenderText(TextRendererComponent& textRenderer) {
////    // Load the font face
////    FT_Face face;
////    if (FT_New_Face(ft, textRenderer.fontPath.c_str(), 0, &face)) {
////        std::cerr << "Failed to load font: " << textRenderer.fontPath << std::endl;
////        return false;
////    }
////
////    // Set the font size (adjust as needed)
////    FT_Set_Pixel_Sizes(face, 0, textRenderer.fontSize);
////
////    // Render each character in the text
////    for (const char& c : textRenderer.text) {
////        if (!LoadGlyph(face, c, textRenderer)) {
////            std::cerr << "Failed to load glyph for character: " << c << std::endl;
////            continue;
////        }
////    }
////
////    FT_Done_Face(face);  // Clean up FreeType face when done
////    return true;
////}
////
////// Loads an individual glyph and generates a texture for it
////bool TextRendereringSystem::LoadGlyph(FT_Face& face, char character, TextRendererComponent& textRenderer) {
////
////    if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
////        std::cerr << "Failed to load character: " << character << std::endl;
////        return false;
////    }
////
////    // Get the bitmap for the character glyph
////    FT_Bitmap* bitmap = &face->glyph->bitmap;
////
////    // Convert the glyph bitmap to a texture
////    textRenderer.texture = CreateTextureFromBitmap(bitmap);
////
////    return true;
////}
////
////// Converts FreeType bitmap to engine's texture 
////bool CreateTextureFromBitmap(FT_Bitmap* bitmap) {
////    GLuint textureID;
////    glGenTextures(1, &textureID);
////    glBindTexture(GL_TEXTURE_2D, textureID);
////
////    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap->width, bitmap->rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap->buffer);
////
////    // Set texture parameters
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////
////    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture
////
////    return textureID;  // Return the generated texture ID
////}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////#include "TextRenderingSystem.h"
////#include "glslshader.h"
////#include <iostream>
////#include <glm/glm.hpp>
////#include <glm/gtc/matrix_transform.hpp>
////#include <glm/gtc/type_ptr.hpp>
////#include "GraphicsManager.h"
////#include "WindowManager.h"
////
////TextRendererComponent textRenderer;
////
////// Constructor
////TextRenderingSystem::TextRenderingSystem() : ft(nullptr), VAO(0), VBO(0), uniformTextColor(0), uniformProjectionMatrix(0), shaderProgram(0) {}
////
////// Destructor
////TextRenderingSystem::~TextRenderingSystem() {
////    if (ft) {
////        FT_Done_FreeType(ft);
////    }
////}
////
////void TextRenderingSystem::Start()
////{
////
////}
////
////// Initialize FreeType and set up necessary OpenGL objects
////bool TextRenderingSystem::Init() {
////    if (FT_Init_FreeType(&ft)) {
////        std::cerr << "Failed to initialize FreeType Library" << std::endl;
////        return false;
////    }
////
////    // Load shaders and setup rendering objects
////    LoadShaders();
////    SetupRenderObjects();
////    GenerateFontAtlas(textRenderer);
////
////    return true;
////}
////
////// Loads and generates a font atlas for a specific TextRendererComponent
////bool TextRenderingSystem::GenerateFontAtlas(TextRendererComponent& _textRenderer) {
////    UNREFERENCED_PARAMETER(_textRenderer);
////    // Load the font face
////    FT_Face face;
////    textRenderer.fontPath = "../Resources/Roboto-Black.ttf";
////    if (FT_New_Face(ft, textRenderer.fontPath, 0, &face)) {
////        std::cerr << "Failed to load font: " << textRenderer.fontPath << std::endl;
////        return false;
////    }
////
////    // Set the font size
////    FT_Set_Pixel_Sizes(face, 0, textRenderer.fontSize);
////
////    // Clear the character map
////    characters.clear();
////
////    //std::cout << "loading characters now\n";
////
////    // Load the first 128 ASCII characters into the atlas
////    for (unsigned char c = 0; c < 128; c++) {
////        if (!LoadCharacter(face, c)) {
////            std::cerr << "Failed to load character: " << c << std::endl;
////        }
////    }
////
////    //std::cout << "loading finished\n";
////
////    // Cleanup FreeType face
////    FT_Done_Face(face);
////
////    return true;
////}
////
////// load a character and generate a texture
////bool TextRenderingSystem::LoadCharacter(FT_Face& face, char c) {
////
////    //std::cout << "loading\n";
////
////    // Load character glyph
////    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
////        std::cout << "Failed to load Glyph for: " << c << std::endl;
////        return false;
////    }
////
////    // Generate texture for glyph
////    GLuint textureID;
////    glGenTextures(1, &textureID);
////    glBindTexture(GL_TEXTURE_2D, textureID);
////    glTexImage2D(
////        GL_TEXTURE_2D,
////        0,
////        GL_RED,
////        face->glyph->bitmap.width,
////        face->glyph->bitmap.rows,
////        0,
////        GL_RED,
////        GL_UNSIGNED_BYTE,
////        face->glyph->bitmap.buffer
////    );
////
////    // Set texture parameters
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////
////
////    // Store character in the map
////    Character character = {
////        textureID,
////        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
////        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
////        (GLuint)face->glyph->advance.x
////    };
////
////    characters.insert(std::pair<char, Character>(c, character));
////    //std::cout << "inserted: " << c << "\n";
////    return true;
////}
////
////// Render all text from TextRendererComponents
////void TextRenderingSystem::Update() {
////    //for (const auto& [entityId, textRenderer] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextRendererComponent>()) {
////    //    TextRendererComponent* textRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextRendererComponent>(entityId);
////    //    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
////
////    //    if (textRenderer && transform) {
////    //        RenderText(*textRenderer, *transform);
////    //    }
////    //}
////    Init();
////    RenderText();
////}
////
////// Renders the text from a given TextRendererComponent
////void TextRenderingSystem::RenderText() {
////    // Activate the shader program
////    glUseProgram(shaderProgram);
////
////    // Set the text color uniform
////    glUniform3f(uniformTextColor, 0.0f, 0.0f, 0.0f);  // White text for example
////
////    // Set up the projection matrix (assuming orthographic projection)
////    glm::mat4 projection = {};//glm::ortho(0.0f, (float)WindowManager::GetWindowWidth(), 0.0f, (float)WindowManager::GetWindowHeight());
////    glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
////
////    // Bind the vertex array object (VAO)
////    glBindVertexArray(VAO);
////
////    float penX = 100;
////    float penY = 100;
////
////    std::cout << characters.size();
////
////    //for (const auto& pair : characters) {
////    //    char character = pair.first;
////    //    const Character& charData = pair.second;
////
////    //    // Print character and associated texture information
////    //    //std::cout << "first: '" << character << "'" << std::endl;
////    //   // std::cout << "second: '" << character << "'" << std::endl;
////    //}
////
////    // Iterate through each character in the text
////    std::string test = "hi";
////    for (const char& c : test) {
////        if (characters.find(c) == characters.end()) {
////            std::cerr << "Character not found in atlas: " << c << std::endl;
////            continue;
////        }
////
////        Character ch = characters[c];
////
////        // Calculate position and size for the character quad
////        float xpos = penX + ch.Bearing.x * 1.0f;
////        float ypos = penY - (ch.Size.y - ch.Bearing.y) *1.0f;
////        float w = ch.Size.x * 1.0f;
////        float h = ch.Size.y * 1.0f;
////
////        // Update VBO with the vertex data for the character quad
////        GLfloat vertices[6][4] = {
////            { xpos,     ypos + h,   0.0f, 0.0f },
////            { xpos,     ypos,       0.0f, 1.0f },
////            { xpos + w, ypos,       1.0f, 1.0f },
////
////            { xpos,     ypos + h,   0.0f, 0.0f },
////            { xpos + w, ypos,       1.0f, 1.0f },
////            { xpos + w, ypos + h,   1.0f, 0.0f }
////        };
////
////        // Render the glyph texture over the quad
////        glBindTexture(GL_TEXTURE_2D, ch.textureID);
////        glBindBuffer(GL_ARRAY_BUFFER, VBO);
////        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
////        glDrawArrays(GL_TRIANGLES, 0, 6);
////
////        // Advance to the next glyph
////        penX += (ch.Advance >> 6) * 1.0f;
////    }
////
////    // Unbind the VAO
////    glBindVertexArray(0);
////    glUseProgram(0);
////}
////
////// Sets up VAO, VBO, and other OpenGL objects for rendering
////void TextRenderingSystem::SetupRenderObjects() {
////    glGenVertexArrays(1, &VAO);
////    glGenBuffers(1, &VBO);
////
////    glBindVertexArray(VAO);
////
////    glBindBuffer(GL_ARRAY_BUFFER, VBO);
////    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
////
////    // Set up vertex attributes (position, texture coordinates)
////    glEnableVertexAttribArray(0);
////    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
////
////    glBindBuffer(GL_ARRAY_BUFFER, 0);
////    glBindVertexArray(0);
////}
////
////// Loads shaders for text rendering
////void TextRenderingSystem::LoadShaders() {
////    GraphicsManager::InsertShader("FontShader", "../Resources/Shaders/fontVertShader.vert", "../Resources/Shaders/fontFragShader.frag");
////}
