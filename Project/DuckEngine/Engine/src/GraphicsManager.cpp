#include <iostream>

#include <random>
#include <filesystem>
#include "ImageLoader.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "CameraManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr

#include "DuckEngine.h"
#include "CameraSystem.h"

#include "ShaderManager.h"

//std::map<std::string, GLSLShader> GraphicsManager::shaders;
GLuint GraphicsManager::VAO = 0;
std::vector<DrawOptions> GraphicsManager::drawQueue;

GLuint GraphicsManager::pointVAO;
GLuint GraphicsManager::lineVAO;
GLuint GraphicsManager::rectVAO;
GLuint GraphicsManager::circleVAO;
int GraphicsManager::circleSegments;
Color GraphicsManager::backgroundColor;

GLuint TEST_TEXTURE = 0;

std::vector<DebugDrawCommand> GraphicsManager::debugDrawQueue;

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

void GraphicsManager::AddToDrawQueue(const DrawOptions& drawOptions) {
    drawQueue.emplace_back(drawOptions);
}

void GraphicsManager::AddToDebugDrawQueue(const DebugDrawCommand& drawCommand) {
    debugDrawQueue.emplace_back(drawCommand);
}

void GraphicsManager::Render() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the clear color (e.g., black in this case)
    glClearColor(backgroundColor.r / 255.f, backgroundColor.g / 255.f, backgroundColor.b / 255.f, backgroundColor.a / 255.f);

    // Clear the color buffer (and depth buffer, if used)
    glClear(GL_COLOR_BUFFER_BIT);

    ShaderManager::GetShader("DefaultShader")->Use();

    glBindVertexArray(VAO);

    Vector2D cameraPosition = CameraManager::GetPosition();
    float ar = CameraManager::GetAR();
    float height = CameraManager::GetHeight();

    glm::mat3x3 viewMatrix = ViewMatrix(cameraPosition);
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
}

void GraphicsManager::RenderDebug()
{
    // Get camera matrices
    Vector2D cameraPosition = CameraManager::GetPosition();
    float ar = CameraManager::GetAR();
    float height = CameraManager::GetHeight();

    // Combine camera-to-NDC and view matrix into one
    glm::mat3x3 viewMatrix = ViewMatrix(cameraPosition);
    glm::mat3x3 cameraToNDC = CameraToNDCMatrix(ar * height, height);
    glm::mat3x3 cameraViewMatrix = cameraToNDC * viewMatrix;

    // Iterate through the queue and process each draw command
    for (const DebugDrawCommand& command : debugDrawQueue) {
        bool useCamera = command.relativeToCamera; // Check if the command should use the camera matrix

        switch (command.type) {
        case DebugDrawCommand::POINT:
            DrawPoint(command.position1, command.sizeOrRadius, command.color, useCamera, cameraViewMatrix);
            break;
        case DebugDrawCommand::LINE:
            DrawLine(command.position1, command.position2, command.sizeOrRadius, command.color, useCamera, cameraViewMatrix);
            break;
        case DebugDrawCommand::RECTANGLE:
            DrawRectangle(command.position1, command.position2, command.rotation, command.color, useCamera, cameraViewMatrix);
            break;
        case DebugDrawCommand::CIRCLE:
            DrawCircle(command.position1, command.sizeOrRadius, command.color, useCamera, cameraViewMatrix);
            break;
        }
    }

    // Clear the debug draw queue after rendering
    debugDrawQueue.clear();
}



void GraphicsManager::SetBackgroundColor(float r, float g, float b, float a) {
    backgroundColor = { r, g, b, a };
}

bool GraphicsManager::Initialize() {

    // Init GLEW, return false if error
    if (!SetUpGLEW()) {
        return false;
    }

    TEST_TEXTURE = ImageLoader::LoadTexture("../Resources/monkey.png");
    if (TEST_TEXTURE == 0) {
        std::cerr << "Failed to load test texture" << std::endl;
        return false;
    }
    std::cout << "Test texture loaded successfully. Texture ID: " << TEST_TEXTURE << std::endl;

    InitializeSingleMeshShaderSystem();

    InitializeDebugShaderSystem();

    return true;
}

void GraphicsManager::Exit() {
    // Clean up any OpenGL resources
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }

    // Clean up shader programs
    //for (auto& shaderPair : shaders) {
    //    shaderPair.second.DeleteShaderProgram(); // Assuming `GLSLShader` has a method to delete the program
    //}
    //shaders.clear();

    // Delete VAOs for debug shapes
    if (pointVAO != 0) glDeleteVertexArrays(1, &pointVAO);
    if (lineVAO != 0) glDeleteVertexArrays(1, &lineVAO);
    if (rectVAO != 0) glDeleteVertexArrays(1, &rectVAO);
    if (circleVAO != 0) glDeleteVertexArrays(1, &circleVAO);
}

void GraphicsManager::InitializeSingleMeshShaderSystem() {

    ShaderManager::InsertShader("DefaultShader", "../Resources/Shaders/gameVertShader.vert", "../Resources/Shaders/gameFragShader.frag");

    InitMesh(VAO);
}

void GraphicsManager::InitializeDebugShaderSystem() {
    ShaderManager::InsertShader("DebugShader", "../Resources/Shaders/DebugVertShader.vert", "../Resources/Shaders/DebugFragShader.frag");

    GraphicsManager::SetupCircleVAO(100);
    GraphicsManager::SetupLineVAO();
    GraphicsManager::SetupPointVAO();
    GraphicsManager::SetupRectangleVAO();
}

//#ifdef DEBUG

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

    // Set line width if desired (optional)
    glLineWidth(1.0f);  // Adjust the line width as needed

    // Draw the rectangle outline using GL_LINE_LOOP
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}


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

    // Set line width (optional)
    glLineWidth(1.0f);  // Adjust the line width as needed

    // Draw the circle outline using GL_LINE_LOOP
    glDrawArrays(GL_LINE_LOOP, 0, circleSegments);

    glBindVertexArray(0);
    glUseProgram(0);
}

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


//#endif // DEBUG


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

    bool SetUpGLEW() {
        // Part 2: Initialize entry points to OpenGL functions and extensions
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

    // Creates 1x1 mesh to reuse for all draws
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

    glm::mat3x3 ViewMatrix(const Vector2D& position) {
        return glm::mat3x3{
                glm::vec3(1.f, 0, 0),
                glm::vec3(0, 1.f, 0),
                glm::vec3(position.x, position.y, 1.f)
        };
    }

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

    glm::mat3x3 CameraToNDCMatrix(const float width, const float height) {
        return glm::mat3x3{
                    glm::vec3(2.f / width, 0, 0),
                    glm::vec3(0, 2.f / height, 0),
                    glm::vec3(0, 0, 1.f)
        };
    }
}