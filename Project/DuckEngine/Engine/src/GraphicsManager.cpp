/******************************************************************************/
/*!
\file       GraphicsManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the GraphicsManager class, which handles all aspects
            of rendering, including the main render loop, debug rendering, and
            managing the draw queue for rendering objects in the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include <iostream>

#include <random>
#include <filesystem>
#include "ImageLoader.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "CameraManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DuckEngine.h"
#include "ShaderManager.h"
#include "GizmoManager.h"

#include "ParticleSystem.h"

#include "FontManager.h"
#include "RenderData.h"

GLuint GraphicsManager::VAO = 0;

GLuint GraphicsManager::pointVAO;
GLuint GraphicsManager::lineVAO;
GLuint GraphicsManager::rectVAO;
GLuint GraphicsManager::circleVAO;
int GraphicsManager::circleSegments;
Color GraphicsManager::backgroundColor;

GLuint GraphicsManager::fbo = 0;
GLuint GraphicsManager::fboTexture = 0;
GLuint GraphicsManager::depthStencil = 0;

GLuint GraphicsManager::pointInstanceVBO = 0;
GLuint GraphicsManager::lineInstanceVBO = 0;
GLuint GraphicsManager::rectangleInstanceVBO = 0;
GLuint GraphicsManager::circleInstanceVBO = 0;

bool GraphicsManager::entityIsSelected = 0;
GizmoData GraphicsManager::gizmoData;

int GraphicsManager::currentGizmo = 1;

GLuint GraphicsManager::filledCircleVAO = 0;
int GraphicsManager::filledCircleSegments = 0;

std::vector<UnifiedRenderCommand> GraphicsManager::drawQueue;

/// <summary>
/// namespace with functions to help setup VBO and EBO
/// </summary>
namespace {
    /// <summary>
    /// Sets up a Vertex Buffer Object (VBO) with given data.
    /// </summary>
    /// <param name="vaoid">Vertex Array Object (VAO) ID</param>
    /// <param name="size">Size of the buffer</param>
    /// <param name="index">Index of the vertex attribute</param>
    /// <param name="type">Type of the vertex attribute (vec2, vec3, etc)</param>
    /// <param name="dataPointer">Pointer to the data</param>
    /// <param name="offset">Offset within the buffer</param>
    /// <param name="stride">Stride of the vertex attributes</param>
    void SetUpVBO(GLuint& vaoid, GLsizei size, GLint index, GLenum type, const void* dataPointer, GLsizei offset, GLsizei stride);

    /// <summary>
    /// Sets up an Element Buffer Object (EBO) with given indices.
    /// </summary>
    /// <param name="vaoid">Vertex Array Object (VAO) ID</param>
    /// <param name="idx_vtx">Vector of vertex indices</param>
    void SetUpEBO(GLuint& vaoid, std::vector<GLuint>& idx_vtx);

    /// <summary>
    /// Sets up GLEW for usage
    /// </summary>
    /// <returns></returns>
    bool SetUpGLEW();

    // Creates 1x1 mesh to reuse for all draws
    void InitMesh(GLuint& VAO);

    glm::mat3x3 ViewMatrix(const Vector2D& position);

    glm::mat3x3 ModelToWorldMatrix(const Vector2D& scale, float rotation, const Vector2D& translate);

    glm::mat3x3 CameraToNDCMatrix(const float width, const float height);
}

glm::mat3 OrthographicProjectionMatrix(float left, float right, float bottom, float top) {
    return glm::mat3(
        glm::vec3(2.0f / (right - left), 0.0f, 0.0f),
        glm::vec3(0.0f, 2.0f / (bottom - top), 0.0f),
        glm::vec3(-(right + left) / (right - left), -(top + bottom) / (top - bottom), 1.0f)
    );
}

void GraphicsManager::AddToDrawQueue(const UnifiedRenderCommand& cmd) {
    drawQueue.push_back(cmd);
}

// --- Unified Render Function ---
void GraphicsManager::Render() {

    // First, sort the unified render queue by layer.
    std::sort(drawQueue.begin(), drawQueue.end(), [](const UnifiedRenderCommand& a, const UnifiedRenderCommand& b) {
        return a.layer < b.layer;
        });

    // Bind the FBO and set up common state.
    BindFBO();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(backgroundColor.r / 255.f, backgroundColor.g / 255.f, backgroundColor.b / 255.f, backgroundColor.a / 255.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // For each command in the unified queue, dispatch according to its type.
    for (const auto& cmd : drawQueue) {
        switch (cmd.type) {
        case RenderCommandType::Game:
            RenderGameObject(std::get<GameRenderCommand>(cmd.command));
            break;
        case RenderCommandType::Text:
            RenderTextObject(std::get<TextRenderCommand>(cmd.command));
            break;
        case RenderCommandType::Debug:
            RenderDebugObject(std::get<DebugRenderCommand>(cmd.command));
            break;
        default:
            break;
        }
    }

    // Clear the queue after rendering.
    drawQueue.clear();

    // Optionally render particles, etc.
    ParticleSystem::RenderTemp();

    // Unbind FBO and cleanup.
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    UnbindFBO();
}

void GraphicsManager::RenderGameObject(const GameRenderCommand& cmd) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ShaderManager::GetShader("DefaultShader")->Use();

    glBindVertexArray(VAO);

    float alpha = static_cast<float>(DuckEngine::accumulatedTime / DuckEngine::FIXED_TIMESTEP);

    // Get interpolated camera position
    Vector2D interpolatedCameraPosition = CameraManager::GetPreviousPosition() +
        (CameraManager::GetPosition() - CameraManager::GetPreviousPosition()) * alpha;

    // Use interpolatedCameraPosition to set up the view matrix
    glm::mat3x3 viewMatrix = ViewMatrix(interpolatedCameraPosition);
    float ar = CameraManager::GetAR();
    float height = CameraManager::GetHeight();
    glm::mat3x3 cameraToNDC = CameraToNDCMatrix(ar * height, height);

    glm::mat3x3 modelToWorld = ModelToWorldMatrix(cmd.scale, cmd.rotation, cmd.translation);

    glm::mat3x3 finalMatrix;

    // world draw
    if (cmd.relativeToCamera) {
        finalMatrix = cameraToNDC * viewMatrix * modelToWorld;
    }
    // UI draw
    else {
        float windowWidth = static_cast<float>(WindowManager::GetWindowWidth());
        float windowHeight = static_cast<float>(WindowManager::GetWindowHeight());

        float left = 0.0f;
        float right = windowWidth;
        float bottom = windowHeight;
        float top = 0.0f;

        glm::mat3 projection = OrthographicProjectionMatrix(left, right, bottom, top);

        // Ensure drawItem positions are in screen coordinates
        glm::mat3 _modelToWorld = ModelToWorldMatrix(cmd.scale, cmd.rotation, cmd.translation);

        finalMatrix = projection * _modelToWorld;
    }


    // Send matrix to vert shader
    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uModelToNDC");
    if (uniformModelToNDCLocation == -1) {
        std::cout << "Uniform variable for modelToNDC doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    if (cmd.useTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *cmd.texture);

        // Set the texture uniform
        GLint uTex2dLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uTex2d");
        if (uTex2dLocation != -1) {
            glUniform1i(uTex2dLocation, 0);
        }

        GLint uUseTextureLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uUseTexture");

        glUniform1i(uUseTextureLocation, 1);
    }
    else {
        GLint uUseTextureLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uUseTexture");

        glUniform1i(uUseTextureLocation, 0);
    }

    if (cmd.useColor) {
        GLint uBlendColorsLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uBlendColors");
        GLint uBlendColorLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uBlendColor");

        glUniform1i(uBlendColorsLocation, 1);
        glUniform4f(uBlendColorLocation, cmd.color.r / 255.f, cmd.color.g / 255.f, cmd.color.b / 255.f, cmd.color.a / 255.f);
    }
    // Test fallback white color for when no textures or colors are provided (Else potential undefined behaviour)
    else {
        GLint uBlendColorsLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uBlendColors");
        GLint uBlendColorLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uBlendColor");

        glUniform1i(uBlendColorsLocation, 1);
        glUniform4f(uBlendColorLocation, 1.f, 1.f, 1.f, 1.f);
    }

    // Render the sprite
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}

void GraphicsManager::RenderTextObject(const TextRenderCommand& cmd) {

    auto shader = ShaderManager::GetShader("TextShader");
    shader->Use();

    // Enable alpha blending for text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Bind the text VAO and set texture unit 0 active
    glBindVertexArray(FontManager::VAO);
    glActiveTexture(GL_TEXTURE0);

    glm::mat4 projection(1.0f);

    if (cmd.isUI)
    {
        // For UI text, use an orthographic projection that maps [0,1] to the screen
        projection = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
    }
    else
    {
        // For world text, use a centered orthographic projection with camera offset
        float virtualHeight = CameraManager::GetHeight();
        float ar = CameraManager::GetAR();
        float virtualWidth = virtualHeight * ar;
        glm::mat4 orthoMat = glm::ortho(
            -virtualWidth * 0.5f, virtualWidth * 0.5f,
            -virtualHeight * 0.5f, virtualHeight * 0.5f,
            -1.0f, 1.0f
        );
        auto camPos = CameraManager::GetPosition();
        glm::mat4 view = glm::translate(glm::mat4(1.0f),
            glm::vec3(-camPos.x, -camPos.y, 0.0f));
        projection = orthoMat * view;
    }

    // Send the projection matrix to the shader
    GLint projLoc = glGetUniformLocation(shader->GetProgram(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the text color
    GLint colorLoc = glGetUniformLocation(shader->GetProgram(), "textColor");
    glUniform4f(colorLoc,
        cmd.color.r / 255.f,
        cmd.color.g / 255.f,
        cmd.color.b / 255.f,
        cmd.color.a / 255.f);

    // Get the font character map for this text command
    const auto& fontMap = FontManager::Fonts[cmd.fontName];

    // Start at the given text position
    float x = cmd.position.x;
    float y = cmd.position.y;

    if (cmd.isUI)
    {
        // Now text.scale is assumed to be normalized already.
        for (char c : cmd.text)
        {
            if (fontMap.find(c) == fontMap.end())
                continue;
            const FontManager::Character& ch = fontMap.at(c);
            // No division by winW/winH needed because we expect text.scale to be relative already.
            float xpos = x + (ch.Bearing.x * cmd.scale);
            float ypos = y - ((ch.Size.y - ch.Bearing.y) * cmd.scale);
            float w = ch.Size.x * cmd.scale;
            float h = ch.Size.y * cmd.scale;

            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,  0.0f, 0.0f },
                { xpos,     ypos,      0.0f, 1.0f },
                { xpos + w, ypos,      1.0f, 1.0f },

                { xpos,     ypos + h,  0.0f, 0.0f },
                { xpos + w, ypos,      1.0f, 1.0f },
                { xpos + w, ypos + h,  1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, FontManager::VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Advance x without additional division.
            x += ((ch.Advance >> 6) * cmd.scale);
        }
    }
    else
    {
        // For world text, positions are in world (pixel) units; use the original calculations.
        for (char c : cmd.text)
        {
            if (fontMap.find(c) == fontMap.end())
                continue;
            const FontManager::Character& ch = fontMap.at(c);

            float xpos = x + ch.Bearing.x * cmd.scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * cmd.scale;
            float w = ch.Size.x * cmd.scale;
            float h = ch.Size.y * cmd.scale;

            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,  0.0f, 0.0f },
                { xpos,     ypos,      0.0f, 1.0f },
                { xpos + w, ypos,      1.0f, 1.0f },

                { xpos,     ypos + h,  0.0f, 0.0f },
                { xpos + w, ypos,      1.0f, 1.0f },
                { xpos + w, ypos + h,  1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, FontManager::VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * cmd.scale;
        }
    }

    // Unbind VAO and texture, clear the draw queue, and unbind the FBO.
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GraphicsManager::RenderDebugObject(const DebugRenderCommand& cmd) {

    // Get camera matrices
    Vector2D cameraPosition = CameraManager::GetPosition();
    float ar = CameraManager::GetAR();
    float height = CameraManager::GetHeight();
    // Combine camera-to-NDC and view matrix into one
    glm::mat3x3 viewMatrix = ViewMatrix(cameraPosition);
    glm::mat3x3 cameraToNDC = CameraToNDCMatrix(ar * height, height);
    glm::mat3x3 cameraViewMatrix = cameraToNDC * viewMatrix;

    bool useCamera = cmd.relativeToCamera; // Check if the command should use the camera matrix

    switch (cmd.type) {
    case DebugRenderCommand::POINT:
        DrawPoint(cmd.position1, cmd.sizeOrRadius, cmd.color, useCamera, cameraViewMatrix);
        break;
    case DebugRenderCommand::LINE:
        DrawLine(cmd.position1, cmd.position2, cmd.sizeOrRadius, cmd.color, useCamera, cameraViewMatrix);
        break;
    case DebugRenderCommand::RECTANGLE:
        DrawRectangle(cmd.position1, cmd.position2, cmd.rotation, cmd.color, useCamera, cameraViewMatrix);
        break;
    case DebugRenderCommand::CIRCLE:
        DrawCircle(cmd.position1, cmd.sizeOrRadius, cmd.color, useCamera, cameraViewMatrix);
        break;
    }

    std::cout << "rendered debug for watever reason\n";
}

/// <summary>
/// Sets the background color for the rendering window.
/// </summary>
/// <param name="r">Red component of the background color (0-255).</param>
/// <param name="g">Green component of the background color (0-255).</param>
/// <param name="b">Blue component of the background color (0-255).</param>
/// <param name="a">Alpha component of the background color (0-255).</param>
void GraphicsManager::SetBackgroundColor(float r, float g, float b, float a) {
    backgroundColor = { r, g, b, a };
}

/// <summary>
/// Initializes the graphics system, setting up necessary resources and shader programs. 
/// Returns false if initialization fails.
/// </summary>
/// <returns>True if initialization is successful, false otherwise.</returns>
bool GraphicsManager::Initialize() {

    // Init GLEW, return false if error
    if (!SetUpGLEW()) {
        return false;
    }

    return true;
}

void GraphicsManager::Start() {
    InitializeSingleMeshShaderSystem();

    InitializeDebugShaderSystem();
}

/// <summary>
/// Cleans up the graphics system, releasing resources such as VAOs.
/// </summary>
void GraphicsManager::Exit() {
    // Clean up OpenGL resources
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);

    // Delete VAOs for debug shapes
    if (pointVAO != 0) glDeleteVertexArrays(1, &pointVAO);
    if (lineVAO != 0) glDeleteVertexArrays(1, &lineVAO);
    if (rectVAO != 0) glDeleteVertexArrays(1, &rectVAO);
    if (circleVAO != 0) glDeleteVertexArrays(1, &circleVAO);
}

/// <summary>
/// Initializes the shader system used for rendering single mesh objects.
/// Loads the default shaders and prepares the mesh for rendering.
/// </summary>
void GraphicsManager::InitializeSingleMeshShaderSystem() {

    //ShaderManager::InsertShader("DefaultShader", "../Resources/Shaders/gameVertShader.vert", "../Resources/Shaders/gameFragShader.frag");

    InitMesh(VAO);
}

/// <summary>
/// Initializes the shader system used for rendering debug elements. 
/// Sets up VAOs for rendering debug shapes such as points, lines, rectangles, and circles.
/// </summary>
void GraphicsManager::InitializeDebugShaderSystem() {

    ShaderManager::InsertShader("DebugShader", "Resources/Shaders/DebugVertShader.vert", "Resources/Shaders/DebugFragShader.frag");

    // Set up VAOs for shapes
    GraphicsManager::SetupCircleVAO(100);
    GraphicsManager::SetupLineVAO();
    GraphicsManager::SetupPointVAO();
    GraphicsManager::SetupRectangleVAO();
    GraphicsManager::SetupFilledCircleVAO(20); // 20 segments or so
}

void GraphicsManager::DrawGizmo() {

    if (entityIsSelected) {
        BindFBO();

        Vector2D position = gizmoData.position;
        float size = gizmoData.size;

        // Disable depth testing to render on top
        glDisable(GL_DEPTH_TEST);

        // Use the debug shader
        ShaderManager::GetShader("DebugShader")->Use();

        // Common settings
        glm::mat3x3 viewMatrix = ViewMatrix(CameraManager::GetPosition());
        float ar = CameraManager::GetAR();
        float height = CameraManager::GetHeight();
        glm::mat3x3 cameraToNDC = CameraToNDCMatrix(ar * height, height);
        glm::mat3x3 cameraViewMatrix = cameraToNDC * viewMatrix;

        if (currentGizmo == 1) {
            // Draw X axis arrow (red)
            DrawArrow(position, Vector2D(size, 0.0f), Color(255, 0, 0, 255), cameraViewMatrix);
            // Draw Y axis arrow (green)
            DrawArrow(position, Vector2D(0.0f, size), Color(0, 255, 0, 255), cameraViewMatrix);
        }
        else if (currentGizmo == 2) {
            // Draw X axis line
            Vector2D end = position + Vector2D(size, 0.0f);
            DrawLine(position, end, 0.05f, Color(255, 0, 0, 255), true, cameraViewMatrix);
            // Draw Y axis line
            end = position + Vector2D(0.0f, size);
            DrawLine(position, end, 0.05f, Color(0, 255, 0, 255), true, cameraViewMatrix);

            float scaleHandleSize = 0.2f * size;

            // Draw scaling handles (yellow squares)
            DrawSquare(position + Vector2D(size, 0.0f), scaleHandleSize, Color(255, 255, 0, 255), cameraViewMatrix); // x-axis scale handle
            DrawSquare(position + Vector2D(0.0f, size), scaleHandleSize, Color(255, 255, 0, 255), cameraViewMatrix); // y-axis scale handle
        }
        else if (currentGizmo == 3) {
            // Draw rotation gizmo (blue circle)
            DrawCircle(position, size * 1.2f, Color(0, 0, 255, 255), true, cameraViewMatrix);
        }

        // Re-enable depth testing
        glEnable(GL_DEPTH_TEST);

        UnbindFBO();
    }
}

void GraphicsManager::DrawSquare(const Vector2D& center, float size, const Color& color, const glm::mat3x3& cameraViewMatrix) {
    ShaderManager::GetShader("DebugShader")->Use();

    // Set up vertex data for a square centered at (0,0) with size 1
    float halfSize = 0.5f;
    float squareVertices[] = {
        -halfSize, -halfSize, 0.0f,
         halfSize, -halfSize, 0.0f,
         halfSize,  halfSize, 0.0f,
        -halfSize,  halfSize, 0.0f
    };

    GLuint squareVAO, squareVBO;
    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);

    glBindVertexArray(squareVAO);

    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    // Position attribute (layout location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uColor");
    glUniform4f(uniformColorLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    // Compute the transformation matrix
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(Vector2D(size, size), 0.0f, center);

    glm::mat3x3 finalMatrix = cameraViewMatrix * modelToWorld;

    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // Draw the square
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &squareVBO);
    glDeleteVertexArrays(1, &squareVAO);

    glUseProgram(0);
}

Vector2D GraphicsManager::ScreenToWorld(const Vector2D& screenPosition) {
    // Convert screen space to NDC space [-1, 1]
    float ndcX = (2.0f * screenPosition.x) / WindowManager::GetWindowWidth() - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPosition.y) / WindowManager::GetWindowHeight();

    glm::vec3 ndcPos(ndcX, ndcY, 1.0f);

    // Build the camera matrices
    Vector2D cameraPosition = CameraManager::GetPosition();
    float ar = CameraManager::GetAR();
    float height = CameraManager::GetHeight();

    glm::mat3x3 viewMatrix = glm::mat3x3{
        glm::vec3(1.0f, 0, 0),
        glm::vec3(0, 1.0f, 0),
        glm::vec3(-cameraPosition.x, -cameraPosition.y, 1.0f)
    };

    glm::mat3x3 cameraToNDC = glm::mat3x3{
        glm::vec3(2.0f / (ar * height), 0, 0),
        glm::vec3(0, 2.0f / height, 0),
        glm::vec3(0, 0, 1.0f)
    };

    glm::mat3x3 worldToNDC = cameraToNDC * viewMatrix;
    glm::mat3x3 ndcToWorld = glm::inverse(worldToNDC);

    glm::vec3 worldPos = ndcToWorld * ndcPos;

    return Vector2D(worldPos.x, worldPos.y);
}

void GraphicsManager::DrawArrow(const Vector2D& start, const Vector2D& direction, const Color& color, const glm::mat3x3& cameraViewMatrix) {
    // Calculate the end point of the arrow
    Vector2D end = start + direction;

    // Draw the shaft of the arrow
    DrawLine(start, end, 0.05f, color, true, cameraViewMatrix);

    // Calculate the magnitude of the direction vector
    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude == 0) return; // Avoid division by zero

    // Calculate the unit direction vector
    Vector2D unitDir = { direction.x / magnitude, direction.y / magnitude };

    // Calculate an orthogonal vector for the arrowhead
    Vector2D orthoDir = { -unitDir.y, unitDir.x };

    // Arrowhead size is proportional to the direction magnitude
    float arrowheadSize = 0.2f * magnitude;

    // Calculate arrowhead points
    Vector2D arrowPoint1 = end;
    Vector2D arrowPoint2 = end - unitDir * arrowheadSize + orthoDir * arrowheadSize * 0.5f;
    Vector2D arrowPoint3 = end - unitDir * arrowheadSize - orthoDir * arrowheadSize * 0.5f;

    // Draw the arrowhead (triangle)
    DrawFilledTriangle(arrowPoint1, arrowPoint2, arrowPoint3, color, true, cameraViewMatrix);
}

void GraphicsManager::DrawFilledTriangle(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix) {
    ShaderManager::GetShader("DebugShader")->Use();

    // Set up vertex data
    GLfloat vertices[] = {
        p1.x, p1.y, 0.0f,
        p2.x, p2.y, 0.0f,
        p3.x, p3.y, 0.0f
    };

    GLuint triangleVAO, triangleVBO;

    // Generate buffers
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    // Bind and set data
    glBindVertexArray(triangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (layout location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uColor");
    glUniform4f(uniformColorLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    // Compute the transformation matrix
    glm::mat3x3 modelToWorld = glm::mat3x3(1.0f); // Identity matrix

    glm::mat3x3 finalMatrix;
    if (useCamera) {
        finalMatrix = cameraViewMatrix * modelToWorld;
    }
    else {
        glm::mat3x3 uiProjection = CameraToNDCMatrix(static_cast<float>(WindowManager::GetWindowWidth()), static_cast<float>(WindowManager::GetWindowHeight()));
        finalMatrix = uiProjection * modelToWorld;
    }

    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Cleanup
    glBindVertexArray(0);
    glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &triangleVAO);

    glUseProgram(0);
}



/// <summary>
/// Draws a point at the specified position with the given size and color. 
/// Optionally applies camera transformations if useCamera is true.
/// </summary>
/// <param name="position">The 2D position of the point.</param>
/// <param name="size">The size of the point.</param>
/// <param name="color">The color of the point (default is red).</param>
/// <param name="useCamera">Indicates whether to apply camera transformations.</param>
/// <param name="cameraViewMatrix">The camera view matrix to apply if useCamera is true.</param>
void GraphicsManager::DrawPoint(const Vector2D& position, float size, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix) {
    ShaderManager::GetShader("DebugShader")->Use();
    glBindVertexArray(pointVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uColor");
    glUniform4f(uniformColorLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    // Model matrix for the point
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(Vector2D(size, size), 0.0f, position);
    glPointSize(size);

    // Use camera-view matrix if useCamera is true, otherwise use the model matrix directly
    glm::mat3x3 finalMatrix;

    if (useCamera) {
        // Apply camera transformation
        finalMatrix = cameraViewMatrix * modelToWorld;
    }
    else {
        // Apply UI projection matrix
        glm::mat3x3 uiProjection = CameraToNDCMatrix(static_cast<float>(WindowManager::GetWindowWidth()), static_cast<float>(WindowManager::GetWindowHeight()));
        finalMatrix = uiProjection * modelToWorld;
    };

    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // Draw the point
    glDrawArrays(GL_POINTS, 0, 1);

    glBindVertexArray(0);
    glUseProgram(0);
}

/// <summary>
/// Draws a line between two points with the specified thickness and color. 
/// Optionally applies camera transformations if useCamera is true.
/// </summary>
/// <param name="start">The starting position of the line.</param>
/// <param name="end">The ending position of the line.</param>
/// <param name="size">The thickness of the line.</param>
/// <param name="color">The color of the line (default is red).</param>
/// <param name="useCamera">Indicates whether to apply camera transformations.</param>
/// <param name="cameraViewMatrix">The camera view matrix to apply if useCamera is true.</param>
void GraphicsManager::DrawLine(const Vector2D& start, const Vector2D& end, float size,
    const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix) {
    ShaderManager::GetShader("DebugShader")->Use();

    glBindVertexArray(lineVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uColor");
    glUniform4f(uniformColorLocation,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);
    // Calculate the direction vector and length
    Vector2D direction = end - start;
    float length = glm::length(glm::vec2{ direction.x, direction.y });
    // Normalize the direction
    Vector2D unitDirection = direction / length;
    // Compute angle of rotation
    float angle = atan2(unitDirection.y, unitDirection.x);
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    // Construct scaling matrix
    glm::mat3x3 scaleMatrix = glm::mat3x3(1.0f);
    scaleMatrix[0][0] = length; // Scale X to match the length of the line
    scaleMatrix[1][1] = size;   // Scale Y to adjust line thickness
    // Construct rotation matrix
    glm::mat3x3 rotationMatrix = glm::mat3x3(1.0f);
    rotationMatrix[0][0] = cosTheta;
    rotationMatrix[0][1] = sinTheta;
    rotationMatrix[1][0] = -sinTheta;
    rotationMatrix[1][1] = cosTheta;
    // Construct translation matrix
    glm::mat3x3 translationMatrix = glm::mat3x3(1.0f);
    translationMatrix[2][0] = start.x; // Translate to start position
    translationMatrix[2][1] = start.y;
    // Combine the transformations: modelToWorld = Translation * Rotation * Scale
    glm::mat3x3 modelToWorld = translationMatrix * rotationMatrix * scaleMatrix;
    // Apply camera transformation if useCamera is true
    glm::mat3x3 finalMatrix;
    if (useCamera) {
        // Apply camera transformation
        finalMatrix = cameraViewMatrix * modelToWorld;
    }
    else {
        // Apply UI projection matrix
        glm::mat3x3 uiProjection = CameraToNDCMatrix(static_cast<float>(WindowManager::GetWindowWidth()), static_cast<float>(WindowManager::GetWindowHeight()));
        finalMatrix = uiProjection * modelToWorld;
    }

    // Pass the final matrix to the shader
    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // Draw the line rectangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}


/// <summary>
/// Draws a rectangle at the specified center position with the given size, rotation, and color. 
/// Optionally applies camera transformations if useCamera is true.
/// </summary>
/// <param name="center">The center position of the rectangle.</param>
/// <param name="size">The width and height of the rectangle.</param>
/// <param name="rotation">The rotation angle of the rectangle (in degrees).</param>
/// <param name="color">The color of the rectangle.</param>
/// <param name="useCamera">Indicates whether to apply camera transformations.</param>
/// <param name="cameraViewMatrix">The camera view matrix to apply if useCamera is true.</param>
void GraphicsManager::DrawRectangle(const Vector2D& center, const Vector2D& size, float rotation, const Color& color,
    bool useCamera, const glm::mat3x3& cameraViewMatrix) {
    ShaderManager::GetShader("DebugShader")->Use();

    glBindVertexArray(rectVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uColor");
    glUniform4f(uniformColorLocation,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);

    // Create the model-to-world matrix using the center position and rotation
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(size, rotation, center);

    // Apply camera transformation if useCamera is true
    glm::mat3x3 finalMatrix;

    if (useCamera) {
        // Apply camera transformation
        finalMatrix = cameraViewMatrix * modelToWorld;
    }
    else {
        // Apply UI projection matrix
        glm::mat3x3 uiProjection = CameraToNDCMatrix(static_cast<float>(WindowManager::GetWindowWidth()), static_cast<float>(WindowManager::GetWindowHeight()));
        finalMatrix = uiProjection * modelToWorld;
    }

    // Pass the final transformation matrix to the shader
    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // Set line width
    glLineWidth(2.0f);

    // Draw the rectangle outline using GL_LINE_LOOP
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}

/// <summary>
/// Draws a circle at the specified position with the given radius and color. 
/// Optionally applies camera transformations if useCamera is true.
/// </summary>
/// <param name="position">The center position of the circle.</param>
/// <param name="radius">The radius of the circle.</param>
/// <param name="color">The color of the circle (default is red).</param>
/// <param name="useCamera">Indicates whether to apply camera transformations.</param>
/// <param name="cameraViewMatrix">The camera view matrix to apply if useCamera is true.</param>
void GraphicsManager::DrawCircle(const Vector2D& position, float radius, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix) {
    ShaderManager::GetShader("DebugShader")->Use();

    glBindVertexArray(circleVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uColor");
    glUniform4f(uniformColorLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    // Model matrix for the circle
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(Vector2D(radius, radius), 0.0f, position);

    // Use camera-view matrix if useCamera is true
    glm::mat3x3 finalMatrix;

    if (useCamera) {
        // Apply camera transformation
        finalMatrix = cameraViewMatrix * modelToWorld;
    }
    else {
        // Apply UI projection matrix
        glm::mat3x3 uiProjection = CameraToNDCMatrix(static_cast<float>(WindowManager::GetWindowWidth()), static_cast<float>(WindowManager::GetWindowHeight()));
        finalMatrix = uiProjection * modelToWorld;
    }

    GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DebugShader")->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // Set width of line
    glLineWidth(2.0f);

    // Draw the circle outline using GL_LINE_LOOP
    glDrawArrays(GL_LINE_LOOP, 0, circleSegments);

    glBindVertexArray(0);
    glUseProgram(0);
}

/// <summary>
/// Sets up the Vertex Array Object (VAO) for rendering points.
/// </summary>
void GraphicsManager::SetupPointVAO() {
    float pointVertex[] = {
        0.0f, 0.0f, 0.0f  // Single point at the origin
    };

    unsigned int pointVBO;
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);
    glBindVertexArray(pointVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertex), pointVertex, GL_STATIC_DRAW);

    // Set vertex attribute pointer for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

/// <summary>
/// Sets up the Vertex Array Object (VAO) for rendering lines.
/// </summary>
void GraphicsManager::SetupLineVAO() {

    float lineVertices[] = {
        // positions (x, y, z)
        0.0f, -0.5f, 0.0f,  // Bottom-left corner
        1.0f, -0.5f, 0.0f,  // Bottom-right corner
        1.0f,  0.5f, 0.0f,  // Top-right corner
        0.0f,  0.5f, 0.0f   // Top-left corner
    };

    unsigned int lineIndices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
    };

    unsigned int lineVBO, lineEBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &lineEBO);
    glBindVertexArray(lineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);

    // Set vertex attribute pointer for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

/// <summary>
/// Sets up the Vertex Array Object (VAO) for rendering rectangles.
/// </summary>
void GraphicsManager::SetupRectangleVAO() {

    float rectVertices[] = {
        // positions in counter-clockwise order
        -0.5f, -0.5f, 0.0f,  // bottom left (vertex 0)
         0.5f, -0.5f, 0.0f,  // bottom right (vertex 1)
         0.5f,  0.5f, 0.0f,  // top right (vertex 2)
        -0.5f,  0.5f, 0.0f   // top left (vertex 3)
    };

    unsigned int rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

    // Set vertex attribute pointer for position (layout location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

/// <summary>
/// Sets up the Vertex Array Object (VAO) for rendering circles with the specified number of segments.
/// </summary>
/// <param name="segments">The number of segments to approximate the circle shape.</param>
void GraphicsManager::SetupCircleVAO(int segments) {
    std::vector<float> vertices; // Store vertices

    circleSegments = segments;

    // Generate vertices for the perimeter
    float angleStep = static_cast<float>(2.0f * M_PI / circleSegments);

    for (int i = 0; i < circleSegments; ++i) {
        float angle = i * angleStep;
        float x = cos(angle);
        float y = sin(angle);
        vertices.push_back(x); // x position
        vertices.push_back(y); // y position
        vertices.push_back(0.0f); // z position (for 2D)
    }

    // Generate VAO and VBO for the circle
    unsigned int circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);

    // Bind and fill VBO with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Set up vertex attributes (position in this case)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}






/// <summary>
/// namespace with helper functions
/// </summary>
namespace {

    /// <summary>
    /// Sets up a Vertex Buffer Object (VBO) with given data.
    /// </summary>
    /// <param name="vaoid">Vertex Array Object (VAO) ID</param>
    /// <param name="size">Size of the buffer</param>
    /// <param name="index">Index of the vertex attribute</param>
    /// <param name="type">Type of the vertex attribute (vec2, vec3, etc)</param>
    /// <param name="dataPointer">Pointer to the data</param>
    /// <param name="offset">Offset within the buffer</param>
    /// <param name="stride">Stride of the vertex attributes</param>
    void SetUpVBO(GLuint& vaoid, GLsizei size, GLint index, GLenum type, const void* dataPointer, GLsizei offset, GLsizei stride) {

        // setup vbo

        GLuint vbo_hdl;

        glCreateBuffers(1, &vbo_hdl);
        glNamedBufferStorage(vbo_hdl, size, NULL, GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferSubData(vbo_hdl, 0, size, dataPointer);

        // bind vao

        glEnableVertexArrayAttrib(vaoid, index);
        glVertexArrayVertexBuffer(vaoid, index, vbo_hdl, offset, stride);
        glVertexArrayAttribFormat(vaoid, index, type, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vaoid, index, index);
    }

    /// <summary>
    /// Sets up an Element Buffer Object (EBO) with given indices.
    /// </summary>
    /// <param name="vaoid">Vertex Array Object (VAO) ID</param>
    /// <param name="idx_vtx">Vector of vertex indices</param>
    void SetUpEBO(GLuint& vaoid, std::vector<GLuint>& idx_vtx) {

        // setup ebo
        GLuint ebo_hdl;

        glCreateBuffers(1, &ebo_hdl);
        glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(), idx_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
        glVertexArrayElementBuffer(vaoid, ebo_hdl);
        glBindVertexArray(0);
    }

    /// <summary>
    /// Initializes GLEW and checks for OpenGL 4.5 support.
    /// </summary>
    /// <returns>True if GLEW is successfully initialized and OpenGL 4.5 is supported, false otherwise.</returns>
    bool SetUpGLEW() {
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            std::cerr << "Unable to initialize GLEW - error: "
                << glewGetErrorString(err) << " abort program" << std::endl;
            return false;
        }
        if (GLEW_VERSION_4_5) {
            std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
            std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
        }
        else {
            std::cerr << "Warning: The driver may lack full compatibility with OpenGL 4.5, potentially limiting access to advanced features." << std::endl;
        }

        return true;
    }

    /// <summary>
    /// Creates and initializes a reusable 1x1 mesh for rendering.
    /// </summary>
    /// <param name="VAO">The Vertex Array Object (VAO) to associate with this mesh.</param>
    void InitMesh(GLuint& VAO) {
        std::vector<glm::vec2> pos_vtx{
            glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f),
            glm::vec2(-0.5f, 0.5f), glm::vec2(-0.5f, -0.5f)
        };

        std::vector<glm::vec3> clr_vtx{
            glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)
        };

        std::vector<glm::vec2> tex_coords{
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f)
        };

        std::vector<GLuint> idx_vtx{
            0, 1, 2,
            0, 2, 3
        };

        glCreateVertexArrays(1, &VAO);

        // Setup position (layout location = 0)
        SetUpVBO(VAO, static_cast<GLsizei>(sizeof(glm::vec2) * pos_vtx.size()), 0, 2, pos_vtx.data(), 0, sizeof(glm::vec2));

        // Setup color (layout location = 1)
        SetUpVBO(VAO, static_cast<GLsizei>(sizeof(glm::vec3) * clr_vtx.size()), 1, 3, clr_vtx.data(), 0, sizeof(glm::vec3));

        // Setup texture coordinates (layout location = 2)
        SetUpVBO(VAO, static_cast<GLsizei>(sizeof(glm::vec2) * tex_coords.size()), 2, 2, tex_coords.data(), 0, sizeof(glm::vec2));

        SetUpEBO(VAO, idx_vtx);

        // Error checking
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error in InitMesh: " << error << std::endl;
        }
    }

    /// <summary>
    /// Generates a 3x3 view matrix from the specified camera position.
    /// </summary>
    /// <param name="position">The position of the camera.</param>
    /// <returns>A 3x3 view matrix.</returns>
    glm::mat3x3 ViewMatrix(const Vector2D& position) {
        return glm::mat3x3{
                glm::vec3(1.f, 0, 0),
                glm::vec3(0, 1.f, 0),
                glm::vec3(-position.x, -position.y, 1.f)
        };
    }

    /// <summary>
    /// Generates a 3x3 model-to-world matrix from the given scale, rotation, and translation parameters.
    /// </summary>
    /// <param name="scale">The scale of the object.</param>
    /// <param name="rotation">The rotation of the object (in degrees).</param>
    /// <param name="translate">The translation (position) of the object.</param>
    /// <returns>A 3x3 model-to-world matrix.</returns>
    glm::mat3x3 ModelToWorldMatrix(const Vector2D& scale, float rotation, const Vector2D& translate) {

        glm::mat3x3 scaleMatrix{
        glm::vec3(scale.x, 0, 0),
        glm::vec3(0, scale.y, 0),
        glm::vec3(0, 0, 1.f),
        };

        float radians = glm::radians(rotation);  // Convert degrees to radians

        glm::mat3x3 rotationMatrix{
        glm::vec3(glm::cos(radians), glm::sin(radians), 0),
        glm::vec3(-glm::sin(radians), glm::cos(radians), 0),
        glm::vec3(0, 0, 1.f)
        };

        glm::mat3x3 translationMatrix{
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(translate.x, translate.y, 1.f),
        };

        // Combine the matrices (S * R * T) and add to list
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    /// <summary>
    /// Generates a 3x3 camera-to-NDC (Normalized Device Coordinates) matrix using the given width and height.
    /// </summary>
    /// <param name="width">The width of the viewport or screen.</param>
    /// <param name="height">The height of the viewport or screen.</param>
    /// <returns>A 3x3 camera-to-NDC matrix.</returns>
    glm::mat3x3 CameraToNDCMatrix(const float width, const float height) {
        return glm::mat3x3{
                    glm::vec3(2.f / width, 0, 0),
                    glm::vec3(0, 2.f / height, 0),
                    glm::vec3(0, 0, 1.f)
        };
    }
}

bool IsOpenGLContextActive()
{
    const GLubyte* version = glGetString(GL_VERSION);
    return version != nullptr;
}

bool GraphicsManager::InitializeFBO(int width, int height)
{

    if (!IsOpenGLContextActive())
    {
        return false;
    }

    if (fbo != 0)
    {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &fboTexture);
        glDeleteRenderbuffers(1, &depthStencil);
    }

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    glGenRenderbuffers(1, &depthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencil);

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "FBO initialization failed! Status: " << fboStatus << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void GraphicsManager::SetupFilledCircleVAO(int segments)
{
    filledCircleSegments = segments;

    // We'll build a simple array for a TRIANGLE FAN:
    // The first vertex is center (0,0), 
    // then each subsequent vertex is on the circle perimeter.
    std::vector<GLfloat> verts;
    verts.reserve((segments + 2) * 3);

    // Center point
    verts.push_back(0.0f);
    verts.push_back(0.0f);
    verts.push_back(0.0f);

    float angleStep = 2.0f * 3.14159f / segments;
    for (int i = 0; i <= segments; ++i)
    {
        float angle = i * angleStep;
        float x = cosf(angle);
        float y = sinf(angle);

        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(0.0f);
    }

    GLuint vbo;
    glGenVertexArrays(1, &filledCircleVAO);
    glGenBuffers(1, &vbo);

    glBindVertexArray(filledCircleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    // layout (location=0) is position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void GraphicsManager::DrawFilledCircle(const Vector2D& position, float radius, const Color& color, bool relativeToCamera)
{
    // We'll reuse the "DebugShader" or something that accepts a mat3
    auto shader = ShaderManager::GetShader("DebugShader");
    shader->Use();

    glBindVertexArray(filledCircleVAO);

    // set color uniform
    GLint colorLoc = glGetUniformLocation(shader->GetProgram(), "uColor");
    glUniform4f(colorLoc,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);

    // build modelToWorld scaling by (radius, radius), translation by position
    // similar to your existing model->world steps
    glm::mat3 modelToWorld(1.0f);
    modelToWorld[0][0] = radius;
    modelToWorld[1][1] = radius;
    modelToWorld[2][0] = position.x;
    modelToWorld[2][1] = position.y;

    // Then combine with camera or UI projection
    glm::mat3 finalMatrix(1.0f);

    if (relativeToCamera)
    {
        // same approach as your debug geometry
        Vector2D camPos = CameraManager::GetPosition();
        float ar = CameraManager::GetAR();
        float h = CameraManager::GetHeight();

        glm::mat3 view(1.0f);
        view[2][0] = -camPos.x;
        view[2][1] = -camPos.y;

        glm::mat3 cameraToNDC(1.0f);
        cameraToNDC[0][0] = 2.f / (ar * h);
        cameraToNDC[1][1] = 2.f / h;

        glm::mat3 cameraView = cameraToNDC * view;
        finalMatrix = cameraView * modelToWorld;
    }
    else
    {
        // UI approach: orthographic from (0..windowWidth, 0..windowHeight).
        float w = (float)WindowManager::GetWindowWidth();
        float h = (float)WindowManager::GetWindowHeight();

        // convert to ND
        glm::mat3 ortho(1.0f);
        ortho[0][0] = 2.f / w;
        ortho[1][1] = 2.f / h;
        ortho[2][0] = -1.f;
        ortho[2][1] = -1.f;

        finalMatrix = ortho * modelToWorld;
    }

    GLint matLoc = glGetUniformLocation(shader->GetProgram(), "uModelToNDC");
    glUniformMatrix3fv(matLoc, 1, GL_FALSE, (GLfloat*)&finalMatrix[0][0]);

    // draw with triangle fan:
    //  (we have segments+2 vertices, center + each perimeter vertex + repeat)
    glDrawArrays(GL_TRIANGLE_FAN, 0, filledCircleSegments + 2);

    glBindVertexArray(0);
    glUseProgram(0);
}

void GraphicsManager::BindFBO() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void GraphicsManager::UnbindFBO() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GraphicsManager::GetFBOTexture() 
{
    return fboTexture;
}