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

std::map<std::string, GLSLShader> GraphicsManager::shaders;
GLuint GraphicsManager::VAO = 0;
GLuint GraphicsManager::VBO = 0;
std::vector<DrawOptions> GraphicsManager::drawQueue;

GLuint GraphicsManager::pointVAO;
GLuint GraphicsManager::lineVAO;
GLuint GraphicsManager::rectVAO;
GLuint GraphicsManager::circleVAO;
int GraphicsManager::circleSegments;
Color GraphicsManager::backgroundColor;

GLuint TEST_TEXTURE = 0;

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
    void InitMesh(GLuint &VAO);

    glm::mat3x3 ViewMatrix(const Vector2D& position);

    glm::mat3x3 ModelToWorldMatrix(const Vector2D& scale, float rotation, const Vector2D& translate);

    glm::mat3x3 CameraToNDCMatrix(const float width, const float height);

    template <typename T>
    T* GetComponent(const int entityId);

    template <typename T>
    std::unordered_map<int, std::shared_ptr<T>>& GetComponents();
}


// a way to set bg color, then maybe i can check if bg is set then change clr
// set render mode to lines, triangles, textures, etc

// maybe add a color mode and u can blend color + texture

// maybe just render 1x1 square, that gets scaled, rotated and transformed accordingly?

// AND TEXTURE IF ANY WIP

void GraphicsManager::AddToDrawQueue(const DrawOptions& drawOptions) {
    drawQueue.emplace_back(drawOptions);
}

void GraphicsManager::Render() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the clear color (e.g., black in this case)
    glClearColor(backgroundColor.r / 255.f, backgroundColor.g / 255.f, backgroundColor.b / 255.f, backgroundColor.a / 255.f);

    // Clear the color buffer (and depth buffer, if used)
    glClear(GL_COLOR_BUFFER_BIT);

    shaders["DefaultShader"].Use();
    glBindVertexArray(VAO);

    Vector2D cameraPosition = CameraManager::GetPosition();
    float ar = CameraManager::GetAR();
    int height = CameraManager::GetHeight();

    glm::mat3x3 viewMatrix = ViewMatrix(cameraPosition);
    glm::mat3x3 cameraToNDC = CameraToNDCMatrix(ar * height, height);

    for (const auto& drawItem : drawQueue) {
        glm::mat3x3 modelToWorld = ModelToWorldMatrix(drawItem.scale, drawItem.rotation, drawItem.translation);

        glm::mat3x3 finalMatrix;

        if (drawItem.relativeToCamera) {
            finalMatrix = cameraToNDC * viewMatrix * modelToWorld;
        }
        else {
            finalMatrix = modelToWorld;
        }

        // Send matrix to vert shader
        GLint uniformModelToNDCLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uModelToNDC");
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
            GLint uTex2dLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uTex2d");
            if (uTex2dLocation != -1) {
                glUniform1i(uTex2dLocation, 0);  // Use texture unit 0
            }

            // Set other uniforms
            GLint uUseTextureLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uUseTexture");
            GLint uBlendColorsLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uBlendColors");
            GLint uBlendColorLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uBlendColor");

            glUniform1i(uUseTextureLocation, 1);  // We are using texture
            glUniform1i(uBlendColorsLocation, 0); // Not blending colors
            glUniform4f(uBlendColorLocation, 1.0f, 1.0f, 1.0f, 1.0f); // White (no blending)
        }

        if (drawItem.useColor) {
            // handle color and send to shaders...
        }

        // Render the sprite
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shaders["DefaultShader"].UnUse();

    drawQueue.clear();
}

void GraphicsManager::SetBackgroundColor(float r, float g, float b, float a) {
    backgroundColor = { r, g, b, a };
}

void GraphicsManager::OldRender(bool isUI) {

    // std::cout << "Trying\n";

    shaders["DefaultShader"].Use();
    glBindVertexArray(VAO);

    // Set the texture uniform
    GLint uTex2dLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uTex2d");
    if (uTex2dLocation != -1) {
        glUniform1i(uTex2dLocation, 0);  // Use texture unit 0
    }

    // Set other uniforms
    GLint uUseTextureLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uUseTexture");
    GLint uBlendColorsLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uBlendColors");
    GLint uBlendColorLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uBlendColor");

    // Example values - adjust as needed
    glUniform1i(uUseTextureLocation, TEST_TEXTURE != 0 ? 1 : 0);
    glUniform1i(uBlendColorsLocation, 0);  // Not blending colors
    glUniform4f(uBlendColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);  // White (no blending)

    // Bind the texture if it exists
    if (TEST_TEXTURE != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TEST_TEXTURE);
    }

    // Loop over all active cameras
    for (const auto& [cameraEntityID, camera] : GetComponents<CameraComponent>()) {

        CameraComponent* camera = GetComponent<CameraComponent>(cameraEntityID);

        // std::cout << "Camera received.\n";

        if (!camera) continue;

        // std::cout << "Camera received.\n";

        glm::mat3x3 viewMatrix = ViewMatrix(camera->position);

        for (const auto& [spriteEntityID, spriteRenderer] : GetComponents<SpriteRendererComponent>()) {

            // Check if sprite renderer exists
            if (SpriteRendererComponent* spriteRenderer = GetComponent<SpriteRendererComponent>(spriteEntityID); !spriteRenderer) continue;

            // std::cout << "Sprite available.\n";

            if (spriteRenderer->sprite) 
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, spriteRenderer->texture);
            }


            // Check if camera and sprite are on same layer
            if (spriteRenderer->layer != camera->layer) continue;

            // std::cout << "Sprite is on same layer.\n";

            // Check if transform exist and render if it does
            if (TransformComponent* transform = GetComponent<TransformComponent>(spriteEntityID))
            {
                // std::cout << "Transform exists. Rendering now\n";

                glm::mat3x3 modelToWorld = ModelToWorldMatrix(transform->scale, transform->angle, transform->position);

                glm::mat3x3 cameraToNDC = CameraToNDCMatrix(camera->windowAspectRatio * camera->cameraHeight, camera->cameraHeight);

                glm::mat3x3 finalMatrix = cameraToNDC * viewMatrix * modelToWorld;

                //std::cout << "final Matrix =\n";
                //for (int row = 0; row < 3; ++row) {
                //    std::cout << "| ";
                //    for (int col = 0; col < 3; ++col) {
                //        std::cout << finalMatrix[row][col] << " ";
                //    }
                //    std::cout << "|\n";
                //}

                //std::cout << "camMatrix =\n";
                //for (int row = 0; row < 3; ++row) {
                //    std::cout << "| ";
                //    for (int col = 0; col < 3; ++col) {
                //        std::cout << cameraToNDC[row][col] << " ";
                //    }
                //    std::cout << "|\n";
                //}

                //std::cout << "viewMatrix =\n";
                //for (int row = 0; row < 3; ++row) {
                //    std::cout << "| ";
                //    for (int col = 0; col < 3; ++col) {
                //        std::cout << viewMatrix[row][col] << " ";
                //    }
                //    std::cout << "|\n";
                //}

                //std::cout << "modelToWorld =\n";
                //for (int row = 0; row < 3; ++row) {
                //    std::cout << "| ";
                //    for (int col = 0; col < 3; ++col) {
                //        std::cout << modelToWorld[row][col] << " ";
                //    }
                //    std::cout << "|\n";
                //}

                // Send matrix to vert shader
                GLint uniformModelToNDCLocation = glGetUniformLocation(shaders["DefaultShader"].GetHandle(), "uModelToNDC");
                if (uniformModelToNDCLocation == -1) {
                    std::cout << "Uniform variable for modelToNDC doesn't exist!!!\n";
                    std::exit(EXIT_FAILURE);
                }

                glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

                // Render the sprite
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

                // std::cout << "drawn\n";
            }
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shaders["DefaultShader"].UnUse();
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

    InsertDebugShader();

    return true;
}

void GraphicsManager::Exit() {

}

void GraphicsManager::InsertShader(std::string shdr_pgm_name,
    std::string vtx_shdr,
    std::string frg_shdr) {

    std::map<std::string, GLSLShader>::iterator it =
        shaders.find(shdr_pgm_name);

    if (it != shaders.end()) return;

    std::vector<std::pair<GLenum, std::string>> shdr_files{
    std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
    std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
    };

    GLSLShader shdr_pgm;

    // Automation hook. [!WARNING!] Do not alter/remove this!
    //AUTOMATION_HOOK_SHADER(shdr_pgm, shdr_files);

    shdr_pgm.CompileLinkValidate(shdr_files);

    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs\n";
        std::cout << shdr_pgm.GetLog() << "\n";
        std::exit(EXIT_FAILURE);
    }

    // add compiled, linked, and validated shader program to
    // std::map container GLApp::shdrpgms
    shaders[shdr_pgm_name] = shdr_pgm;
}

void GraphicsManager::InitializeSingleMeshShaderSystem() {
    // Insert your shaders (this function should load the vertex and fragment shaders)
    InsertShader("DefaultShader", "../Resources/Shaders/vertShader.vert", "../Resources/Shaders/fragShader.frag");
    InitMesh(VAO);
}

//#ifdef DEBUG

void GraphicsManager::InsertDebugShader() {
    // Name for the debug shader
    std::string shdr_pgm_name = "DebugShader";

    // Check if the shader program name already exists in the shaders map
    std::map<std::string, GLSLShader>::iterator it = shaders.find(shdr_pgm_name);
    if (it != shaders.end()) return;

    // Define the vertex shader source as a const char* string
    const char* debugVertexShaderSource = R"(
    #version 450 core
    layout(location = 0) in vec3 position;
    uniform mat3 uModelToNDC;
    void main() {
        vec3 worldPosition = uModelToNDC * vec3(position.xy, 1.0);
        gl_Position = vec4(worldPosition.xy, 0.0, 1.0);
    }
    )";

    // Define the fragment shader source as a const char* string
    const char* debugFragmentShaderSource = R"(
    #version 450 core
    out vec4 FragColor;
    uniform vec4 uColor;
    void main() {
        FragColor = uColor;
    }
    )";

    // Create a vector of shader type and source code, with sources as strings
    std::vector<std::pair<GLenum, std::string>> shdr_files{
        std::make_pair(GL_VERTEX_SHADER, std::string(debugVertexShaderSource)),
        std::make_pair(GL_FRAGMENT_SHADER, std::string(debugFragmentShaderSource))
    };

    GLSLShader shdr_pgm;

    // Call the CompileLinkValidate method with compile_from_file set to false
    shdr_pgm.CompileLinkValidate(shdr_files, false);  // false indicates we are compiling from strings

    // Check if the shader was successfully linked
    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate debug shader\n";
        std::cout << shdr_pgm.GetLog() << "\n";
        std::exit(EXIT_FAILURE);
    }

    // Add the compiled, linked, and validated shader program to the shaders map
    shaders[shdr_pgm_name] = shdr_pgm;
}


void GraphicsManager::DrawPoint(const Vector2D& position, float size, const Color& color) {
    shaders["DebugShader"].Use();
    glBindVertexArray(pointVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uColor");
    glUniform4f(uniformColorLocation,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);

    // Scale down the model matrix to render the point
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(Vector2D(size, size), 0.0f, position);
    glPointSize(size);  // Increase the point size to make it more visible

    GLint uniformModelToNDCLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    // Draw the point
    glDrawArrays(GL_POINTS, 0, 1);

    glBindVertexArray(0);
    shaders["DebugShader"].UnUse();
}

void GraphicsManager::DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color) {
    shaders["DebugShader"].Use();
    glBindVertexArray(lineVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uColor");
    glUniform4f(uniformColorLocation,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);

    // Calculate midpoint and direction
    Vector2D midPoint = (start + end) * 0.5f;
    Vector2D direction = end - start;
    float length = glm::length( glm::vec2{ direction.x, direction.y });
    float angle = atan2(direction.y, direction.x);

    // Create the model matrix to scale and rotate the line
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(Vector2D(length, size), angle, midPoint);
    glLineWidth(size);  // Set line width

    GLint uniformModelToNDCLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    // Draw the line
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
    shaders["DebugShader"].UnUse();
}

void GraphicsManager::DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color) {
    shaders["DebugShader"].Use();
    glBindVertexArray(rectVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uColor");
    glUniform4f(uniformColorLocation,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);

    // Create the model matrix to scale and translate the rectangle
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(size, 0.0f, position);
    GLint uniformModelToNDCLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    // Draw the rectangle (quad)
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    shaders["DebugShader"].UnUse();
}

void GraphicsManager::DrawCircle(const Vector2D& position, float radius, const Color& color) {
    shaders["DebugShader"].Use();
    glBindVertexArray(circleVAO);

    // Set the color
    GLint uniformColorLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uColor");
    glUniform4f(uniformColorLocation,
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f);

    // Create a model matrix to scale the circle to the correct radius and position it
    glm::mat3x3 modelToWorld = ModelToWorldMatrix(Vector2D(radius, radius), 0.0f, position);
    GLint uniformModelToNDCLocation = glGetUniformLocation(shaders["DebugShader"].GetHandle(), "uModelToNDC");
    glUniformMatrix3fv(uniformModelToNDCLocation, 1, GL_FALSE, glm::value_ptr(modelToWorld));

    // Draw the circle using the triangle fan (6 * number of triangles = 3 * number of segments)
    glDrawElements(GL_TRIANGLES, 3 * circleSegments, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    shaders["DebugShader"].UnUse();
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
        0.0f, 0.0f, 0.0f,  // Start point of the line (relative position)
        1.0f, 0.0f, 0.0f   // End point of the line (relative position)
    };

    unsigned int lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    glBindVertexArray(lineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    // Set vertex attribute pointer for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void GraphicsManager::SetupRectangleVAO() {
    float rectVertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,  // bottom right
         0.5f,  0.5f, 0.0f,  // top right
        -0.5f,  0.5f, 0.0f   // top left
    };

    unsigned int rectIndices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    unsigned int rectVBO, rectEBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glGenBuffers(1, &rectEBO);

    glBindVertexArray(rectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

    // Set vertex attribute pointer for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void GraphicsManager::SetupCircleVAO(int segments) {
    std::vector<float> vertices; // Store vertices
    std::vector<unsigned int> indices; // Store indices

    circleSegments = segments;

    // First vertex is the center of the circle
    vertices.push_back(0.0f); // x
    vertices.push_back(0.0f); // y
    vertices.push_back(0.0f); // z (assuming 2D, this can be set to 0)

    // Generate vertices for the perimeter
    float angleStep = 2.0f * M_PI / circleSegments;

    for (int i = 0; i <= circleSegments; ++i) {
        float angle = i * angleStep;
        float x = 0.5f * cos(angle); // 0.5f to match the 1x1 scale
        float y = 0.5f * sin(angle);
        vertices.push_back(x); // x position
        vertices.push_back(y); // y position
        vertices.push_back(0.0f); // z position (for 2D)
    }

    // Generate indices for the triangle fan
    for (int i = 1; i <= circleSegments; ++i) {
        indices.push_back(0);  // The center vertex
        indices.push_back(i);  // Current perimeter vertex
        indices.push_back(i + 1); // Next perimeter vertex (wraps around)
    }

    // Last triangle wraps around to the first perimeter vertex
    indices.push_back(0);
    indices.push_back(circleSegments);
    indices.push_back(1);

    // Generate VAO and VBO for the circle
    unsigned int circleVBO, circleEBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glGenBuffers(1, &circleEBO);

    glBindVertexArray(circleVAO);

    // Bind and fill VBO with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Bind and fill EBO with index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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

    template <typename T>
    T* GetComponent(const int entityId) {
        return DuckEngine::DUCKENGINE_ComponentManager.GetComponent<T>(entityId);
    }

    template <typename T>
    std::unordered_map<int, std::shared_ptr<T>>& GetComponents() {
        // Use dynamic_pointer_cast to cast Component to T for each item in the unordered_map
        return reinterpret_cast<std::unordered_map<int, std::shared_ptr<T>>&>(
            DuckEngine::DUCKENGINE_ComponentManager.GetComponents<T>()
            );
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