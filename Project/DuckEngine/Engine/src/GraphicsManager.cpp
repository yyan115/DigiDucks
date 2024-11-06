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



GLuint GraphicsManager::VAO = 0;
std::vector<DrawOptions> GraphicsManager::drawQueue;

GLuint GraphicsManager::pointVAO;
GLuint GraphicsManager::lineVAO;
GLuint GraphicsManager::rectVAO;
GLuint GraphicsManager::circleVAO;
int GraphicsManager::circleSegments;
Color GraphicsManager::backgroundColor;

GLuint GraphicsManager::fbo = 0;
GLuint GraphicsManager::fboTexture = 0;
GLuint GraphicsManager::depthStencil = 0;

std::vector<DebugDrawCommand> GraphicsManager::debugDrawQueue;

GLuint GraphicsManager::pointInstanceVBO = 0;
GLuint GraphicsManager::lineInstanceVBO = 0;
GLuint GraphicsManager::rectangleInstanceVBO = 0;
GLuint GraphicsManager::circleInstanceVBO = 0;

std::vector<DebugDrawCommand> GraphicsManager::pointCommands;
std::vector<DebugDrawCommand> GraphicsManager::lineCommands;
std::vector<DebugDrawCommand> GraphicsManager::rectangleCommands;
std::vector<DebugDrawCommand> GraphicsManager::circleCommands;

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

/// <summary>
/// Adds a new drawing command to the queue that will be rendered on the next call to Render().
/// </summary>
/// <param name="drawOptions">The options for rendering the object, including position, rotation, and other properties.</param>
void GraphicsManager::AddToDrawQueue(const DrawOptions& drawOptions) {
    drawQueue.emplace_back(drawOptions);
}

/// <summary>
/// Adds a new debug draw command to the debug queue that will be rendered on the next call to RenderDebug().
/// </summary>
/// <param name="drawCommand">The options for rendering the debug object, such as its position, type, and color.</param>
void GraphicsManager::AddToDebugDrawQueue(const DebugDrawCommand& drawCommand) {
    debugDrawQueue.emplace_back(drawCommand);
}

/// <summary>
/// Renders all objects in the draw queue using the default shader and configured matrices. 
/// Handles both textured and color-based rendering, setting up necessary OpenGL states.
/// </summary>
void GraphicsManager::Render() {

    BindFBO();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the clear color (e.g., black in this case)
    glClearColor(backgroundColor.r / 255.f, backgroundColor.g / 255.f, backgroundColor.b / 255.f, backgroundColor.a / 255.f);

    // Clear the color buffer (and depth buffer, if used)
    glClear(GL_COLOR_BUFFER_BIT);

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

    for (const auto& drawItem : drawQueue) {
        glm::mat3x3 modelToWorld = ModelToWorldMatrix(drawItem.scale, drawItem.rotation, drawItem.translation);

        glm::mat3x3 finalMatrix;

        if (drawItem.relativeToCamera) {
            finalMatrix = cameraToNDC * viewMatrix * modelToWorld;
        }
        else {
            glm::mat3x3 uiProjection = CameraToNDCMatrix(static_cast<float>(WindowManager::GetWindowWidth()), static_cast<float>(WindowManager::GetWindowHeight()));
            finalMatrix = uiProjection * modelToWorld;
        }

        // Send matrix to vert shader
        GLint uniformModelToNDCLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uModelToNDC");
        if (uniformModelToNDCLocation == -1) {
            std::cout << "Uniform variable for modelToNDC doesn't exist!!!\n";
            std::exit(EXIT_FAILURE);
        }

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

        if (drawItem.useTexture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *drawItem.texture);

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

        if (drawItem.useColor) {
            GLint uBlendColorsLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uBlendColors");
            GLint uBlendColorLocation = glGetUniformLocation(ShaderManager::GetShader("DefaultShader")->GetProgram(), "uBlendColor");

            glUniform1i(uBlendColorsLocation, 1);
            glUniform4f(uBlendColorLocation, drawItem.color.r / 255.f, drawItem.color.g / 255.f, drawItem.color.b / 255.f, drawItem.color.a / 255.f);
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
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);

    drawQueue.clear();

    UnbindFBO();
}

void GraphicsManager::RenderDebug() {
    if (!DuckEngine::showDebugDraw) return;

    // Clear previous grouped commands
    pointCommands.clear();
    lineCommands.clear();
    rectangleCommands.clear();
    circleCommands.clear();

    // Partition debugDrawQueue
    for (const DebugDrawCommand& command : debugDrawQueue) {
        switch (command.type) {
        case DebugDrawCommand::POINT:
            pointCommands.push_back(command);
            break;
        case DebugDrawCommand::LINE:
            lineCommands.push_back(command);
            break;
        case DebugDrawCommand::RECTANGLE:
            rectangleCommands.push_back(command);
            break;
        case DebugDrawCommand::CIRCLE:
            circleCommands.push_back(command);
            break;
        }
    }

    // Get camera matrices
    Vector2D cameraPosition = CameraManager::GetPosition();
    float ar = CameraManager::GetAR();
    float height = CameraManager::GetHeight();

    // Combine camera-to-NDC and view matrix into one
    glm::mat3x3 viewMatrix = ViewMatrix(cameraPosition);
    glm::mat3x3 cameraToNDC = CameraToNDCMatrix(ar * height, height);
    glm::mat3x3 cameraViewMatrix = cameraToNDC * viewMatrix;

    // Render each group
    RenderPoints(cameraViewMatrix);
    RenderLines(cameraViewMatrix);
    RenderRectangles(cameraViewMatrix);
    RenderCircles(cameraViewMatrix);

    // Clear the debug draw queue after rendering
    debugDrawQueue.clear();
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

    InitializeSingleMeshShaderSystem();

    InitializeDebugShaderSystem();

    return true;
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

    ShaderManager::InsertShader("DefaultShader", "../Resources/Shaders/gameVertShader.vert", "../Resources/Shaders/gameFragShader.frag");

    InitMesh(VAO);
}

/// <summary>
/// Initializes the shader system used for rendering debug elements. 
/// Sets up VAOs for rendering debug shapes such as points, lines, rectangles, and circles.
/// </summary>
void GraphicsManager::InitializeDebugShaderSystem() {
    //ShaderManager::InsertShader("DebugShader", "../Resources/Shaders/DebugVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");

    //GraphicsManager::SetupCircleVAO(100);
    //GraphicsManager::SetupLineVAO();
    //GraphicsManager::SetupPointVAO();
    //GraphicsManager::SetupRectangleVAO();

    ShaderManager::InsertShader("PointShader", "../Resources/Shaders/PointVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");
    ShaderManager::InsertShader("LineShader", "../Resources/Shaders/LineVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");
    ShaderManager::InsertShader("RectangleShader", "../Resources/Shaders/RectangleVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");
    ShaderManager::InsertShader("CircleShader", "../Resources/Shaders/CircleVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");

    // Set up VAOs for shapes
    GraphicsManager::SetupCircleVAO(100);
    GraphicsManager::SetupLineVAO();
    GraphicsManager::SetupPointVAO();
    GraphicsManager::SetupRectangleVAO();
}

void GraphicsManager::RenderPoints(const glm::mat3x3& cameraViewMatrix) {
    if (pointCommands.empty()) return;

    // Separate commands based on relativeToCamera
    std::vector<PointInstanceData> cameraRelativeData;
    std::vector<PointInstanceData> uiData;

    for (const auto& command : pointCommands) {
        PointInstanceData data;
        data.position = glm::vec2(command.position1.x, command.position1.y);
        data.size = command.sizeOrRadius;
        data.color = glm::vec4(
            command.color.r / 255.0f,
            command.color.g / 255.0f,
            command.color.b / 255.0f,
            command.color.a / 255.0f
        );

        if (command.relativeToCamera) {
            cameraRelativeData.push_back(data);
        }
        else {
            uiData.push_back(data);
        }
    }

    GLShader* shader = ShaderManager::GetShader("PointShader");
    shader->Use();

    GLint uniformModelToNDCLocation = glGetUniformLocation(shader->GetProgram(), "uModelToNDC");

    glBindVertexArray(pointVAO);

    glLineWidth(2.0f);

    // Render camera-relative points
    if (!cameraRelativeData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, pointInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, cameraRelativeData.size() * sizeof(PointInstanceData), cameraRelativeData.data(), GL_DYNAMIC_DRAW);

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));

        glDrawArraysInstanced(GL_POINTS, 0, 1, static_cast<GLsizei>(cameraRelativeData.size()));
    }

    // Render UI points
    if (!uiData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, pointInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, uiData.size() * sizeof(PointInstanceData), uiData.data(), GL_DYNAMIC_DRAW);

        glm::mat3x3 uiProjection = CameraToNDCMatrix(
            static_cast<float>(WindowManager::GetWindowWidth()),
            static_cast<float>(WindowManager::GetWindowHeight())
        );

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(uiProjection));

        glDrawArraysInstanced(GL_POINTS, 0, 1, static_cast<GLsizei>(uiData.size()));
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void GraphicsManager::RenderLines(const glm::mat3x3& cameraViewMatrix) {
    if (lineCommands.empty()) return;

    std::vector<LineInstanceData> cameraRelativeData;
    std::vector<LineInstanceData> uiData;

    for (const auto& command : lineCommands) {
        LineInstanceData data;
        data.start = glm::vec2(command.position1.x, command.position1.y);
        data.end = glm::vec2(command.position2.x, command.position2.y);
        data.thickness = command.sizeOrRadius;
        data.color = glm::vec4(
            command.color.r / 255.0f,
            command.color.g / 255.0f,
            command.color.b / 255.0f,
            command.color.a / 255.0f
        );

        if (command.relativeToCamera) {
            cameraRelativeData.push_back(data);
        }
        else {
            uiData.push_back(data);
        }
    }

    GLShader* shader = ShaderManager::GetShader("LineShader");
    shader->Use();

    GLint uniformModelToNDCLocation = glGetUniformLocation(shader->GetProgram(), "uModelToNDC");

    glBindVertexArray(lineVAO);

    glLineWidth(2.0f);

    // Render camera-relative lines
    if (!cameraRelativeData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, lineInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, cameraRelativeData.size() * sizeof(LineInstanceData), cameraRelativeData.data(), GL_DYNAMIC_DRAW);

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(cameraRelativeData.size()));
    }

    // Render UI lines
    if (!uiData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, lineInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, uiData.size() * sizeof(LineInstanceData), uiData.data(), GL_DYNAMIC_DRAW);

        glm::mat3x3 uiProjection = CameraToNDCMatrix(
            static_cast<float>(WindowManager::GetWindowWidth()),
            static_cast<float>(WindowManager::GetWindowHeight())
        );

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(uiProjection));

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(uiData.size()));
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void GraphicsManager::RenderRectangles(const glm::mat3x3& cameraViewMatrix) {
    if (rectangleCommands.empty()) return;

    std::vector<RectangleInstanceData> cameraRelativeData;
    std::vector<RectangleInstanceData> uiData;

    for (const auto& command : rectangleCommands) {
        RectangleInstanceData data;
        data.center = glm::vec2(command.position1.x, command.position1.y);
        data.size = glm::vec2(command.position2.x, command.position2.y);
        data.rotation = command.rotation;
        data.color = glm::vec4(
            command.color.r / 255.0f,
            command.color.g / 255.0f,
            command.color.b / 255.0f,
            command.color.a / 255.0f
        );

        if (command.relativeToCamera) {
            cameraRelativeData.push_back(data);
        }
        else {
            uiData.push_back(data);
        }
    }

    GLShader* shader = ShaderManager::GetShader("RectangleShader");
    shader->Use();

    GLint uniformModelToNDCLocation = glGetUniformLocation(shader->GetProgram(), "uModelToNDC");

    glBindVertexArray(rectVAO);

    // Set line width for the rectangle outline
    //glLineWidth(2.0f);

    // Render camera-relative rectangles
    if (!cameraRelativeData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, rectangleInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, cameraRelativeData.size() * sizeof(RectangleInstanceData), cameraRelativeData.data(), GL_DYNAMIC_DRAW);

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));

        glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, static_cast<GLsizei>(cameraRelativeData.size()));
    }



    // Render UI rectangles
    if (!uiData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, rectangleInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, uiData.size() * sizeof(RectangleInstanceData), uiData.data(), GL_DYNAMIC_DRAW);

        glm::mat3x3 uiProjection = CameraToNDCMatrix(
            static_cast<float>(WindowManager::GetWindowWidth()),
            static_cast<float>(WindowManager::GetWindowHeight())
        );

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(uiProjection));

        glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, static_cast<GLsizei>(uiData.size()));
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void GraphicsManager::RenderCircles(const glm::mat3x3& cameraViewMatrix) {
    if (circleCommands.empty()) return;

    std::vector<CircleInstanceData> cameraRelativeData;
    std::vector<CircleInstanceData> uiData;

    for (const auto& command : circleCommands) {
        CircleInstanceData data;
        data.position = glm::vec2(command.position1.x, command.position1.y);
        data.radius = command.sizeOrRadius;
        data.color = glm::vec4(
            command.color.r / 255.0f,
            command.color.g / 255.0f,
            command.color.b / 255.0f,
            command.color.a / 255.0f
        );

        if (command.relativeToCamera) {
            cameraRelativeData.push_back(data);
        }
        else {
            uiData.push_back(data);
        }
    }

    GLShader* shader = ShaderManager::GetShader("CircleShader");
    shader->Use();

    GLint uniformModelToNDCLocation = glGetUniformLocation(shader->GetProgram(), "uModelToNDC");

    glBindVertexArray(circleVAO);

    // Set line width for the circle outline
    glLineWidth(30.0f);

    // Render camera-relative circles
    if (!cameraRelativeData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, circleInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, cameraRelativeData.size() * sizeof(CircleInstanceData), cameraRelativeData.data(), GL_DYNAMIC_DRAW);

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(cameraViewMatrix));

        glDrawArraysInstanced(GL_LINE_LOOP, 0, circleSegments, static_cast<GLsizei>(cameraRelativeData.size()));
    }

    // Render UI circles
    if (!uiData.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, circleInstanceVBO);
        glBufferData(GL_ARRAY_BUFFER, uiData.size() * sizeof(CircleInstanceData), uiData.data(), GL_DYNAMIC_DRAW);

        glm::mat3x3 uiProjection = CameraToNDCMatrix(
            static_cast<float>(WindowManager::GetWindowWidth()),
            static_cast<float>(WindowManager::GetWindowHeight())
        );

        glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(uiProjection));

        glDrawArraysInstanced(GL_LINE_LOOP, 0, circleSegments, static_cast<GLsizei>(uiData.size()));
    }

    glBindVertexArray(0);
    glUseProgram(0);
}


void GraphicsManager::SetupPointVAO() {
    float pointVertex[] = { 0.0f, 0.0f, 0.0f };

    glGenVertexArrays(1, &pointVAO);
    glBindVertexArray(pointVAO);

    // Vertex data VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertex), pointVertex, GL_STATIC_DRAW);

    // Vertex attribute for position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Instance data VBO
    glGenBuffers(1, &pointInstanceVBO);

    // Position (location = 1)
    glBindBuffer(GL_ARRAY_BUFFER, pointInstanceVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PointInstanceData), (void*)offsetof(PointInstanceData, position));
    glVertexAttribDivisor(1, 1);

    // Size (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PointInstanceData), (void*)offsetof(PointInstanceData, size));
    glVertexAttribDivisor(2, 1);

    // Color (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(PointInstanceData), (void*)offsetof(PointInstanceData, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void GraphicsManager::SetupLineVAO() {
    // Define a unit line along the x-axis from (0, 0) to (1, 0)
    float lineVertices[] = {
        0.0f, -0.5f, 0.0f,  // Bottom-left corner
        1.0f, -0.5f, 0.0f,  // Bottom-right corner
        1.0f,  0.5f, 0.0f,  // Top-right corner
        0.0f,  0.5f, 0.0f   // Top-left corner
    };

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    glGenVertexArrays(1, &lineVAO);
    glBindVertexArray(lineVAO);

    // Vertex data VBO
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attribute for position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Instance data VBO
    glGenBuffers(1, &lineInstanceVBO);

    // Start position (location = 1)
    glBindBuffer(GL_ARRAY_BUFFER, lineInstanceVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(LineInstanceData), (void*)offsetof(LineInstanceData, start));
    glVertexAttribDivisor(1, 1);

    // End position (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(LineInstanceData), (void*)offsetof(LineInstanceData, end));
    glVertexAttribDivisor(2, 1);

    // Thickness (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(LineInstanceData), (void*)offsetof(LineInstanceData, thickness));
    glVertexAttribDivisor(3, 1);

    // Color (location = 4)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(LineInstanceData), (void*)offsetof(LineInstanceData, color));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
}

void GraphicsManager::SetupRectangleVAO() {
    // Rectangle vertices (unit square centered at origin)
    float rectVertices[] = {
        -0.5f, -0.5f, 0.0f,  // Bottom-left corner
         0.5f, -0.5f, 0.0f,  // Bottom-right corner
         0.5f,  0.5f, 0.0f,  // Top-right corner
        -0.5f,  0.5f, 0.0f   // Top-left corner
    };

    glGenVertexArrays(1, &rectVAO);
    glBindVertexArray(rectVAO);

    // Vertex data VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

    // Vertex attribute for position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Instance data VBO
    glGenBuffers(1, &rectangleInstanceVBO);

    // Center position (location = 1)
    glBindBuffer(GL_ARRAY_BUFFER, rectangleInstanceVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(RectangleInstanceData), (void*)offsetof(RectangleInstanceData, center));
    glVertexAttribDivisor(1, 1);

    // Size (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RectangleInstanceData), (void*)offsetof(RectangleInstanceData, size));
    glVertexAttribDivisor(2, 1);

    // Rotation (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(RectangleInstanceData), (void*)offsetof(RectangleInstanceData, rotation));
    glVertexAttribDivisor(3, 1);

    // Color (location = 4)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(RectangleInstanceData), (void*)offsetof(RectangleInstanceData, color));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
}

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
    glGenVertexArrays(1, &circleVAO);
    glBindVertexArray(circleVAO);

    // Vertex data VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Vertex attribute for position (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Instance data VBO
    glGenBuffers(1, &circleInstanceVBO);

    // Position (location = 1)
    glBindBuffer(GL_ARRAY_BUFFER, circleInstanceVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CircleInstanceData), (void*)offsetof(CircleInstanceData, position));
    glVertexAttribDivisor(1, 1);

    // Radius (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(CircleInstanceData), (void*)offsetof(CircleInstanceData, radius));
    glVertexAttribDivisor(2, 1);

    // Color (location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(CircleInstanceData), (void*)offsetof(CircleInstanceData, color));
    glVertexAttribDivisor(3, 1);

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