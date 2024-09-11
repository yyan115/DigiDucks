#include <iostream>

#include <random>
#include <filesystem>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GraphicsManager.h"
#include "WindowManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

std::map<std::string, GLSLShader> GraphicsManager::shaders;
GLuint GraphicsManager::VAO = 0;
GLuint GraphicsManager::VBO = 0;
std::vector<glm::mat3x3> GraphicsManager::transforms;

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
}


// a way to set bg color, then maybe i can check if bg is set then change clr
// set render mode to lines, triangles, textures, etc

// maybe add a color mode and u can blend color + texture

// maybe just render 1x1 square, that gets scaled, rotated and transformed accordingly?

// AND TEXTURE IF ANY WIP
void GraphicsManager::AddToDrawQueue(const Vector2D& scale, float rotation, const Vector2D& translate) {

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
    transforms.push_back(translationMatrix * rotationMatrix * scaleMatrix);
}

void GraphicsManager::Render(bool isUI) {

    // Use the shader program and bind the VAO to render the rectangle
    //glUseProgram(shaderProgram);

    shaders["DefaultShader"].Use();

    glBindVertexArray(VAO);

    // Draw the rectangle (6 vertices = 2 triangles)
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    for (const auto& transform : transforms) {

    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    // Unbind the VAO
    glBindVertexArray(0);

    shaders["DefaultShader"].UnUse();
}

bool GraphicsManager::Initialize() {

    // Init GLEW, return false if error
    if (!SetUpGLEW()) {
        return false;
    }

    GraphicsManager::InitializeSingleMeshShaderSystem();

    return true;
}

void GraphicsManager::Exit() {

}

void GraphicsManager::SetBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
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
    InsertShader("DefaultShader", "../../Engine/src/vertShader.vert", "../../Engine/src/fragShader.frag");

    InitMesh(VAO);
}

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
    void InitMesh(GLuint &VAO) {
        std::vector<glm::vec2> pos_vtx{
            glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f),
            glm::vec2(-0.5, 0.5f), glm::vec2(-0.5f, -0.5f)
        };

        std::vector<GLuint> idx_vtx{
            0, 1, 2,
            0, 2, 3
        };

        std::vector<glm::vec3> clr_vtx;

        std::random_device randomDevice;
        std::default_random_engine randomEngine(randomDevice());
        std::uniform_real_distribution<GLfloat> urdfloats(0.0f, 1.0f);

        for (int i = 0; i < pos_vtx.size(); i++) {
            clr_vtx.push_back({ urdfloats(randomEngine), urdfloats(randomEngine) , urdfloats(randomEngine) });
        }

        glCreateVertexArrays(1, &VAO);

        // setup position
        SetUpVBO(VAO, static_cast<GLsizei>(sizeof(glm::vec2) * pos_vtx.size()), 0, 2, pos_vtx.data(), 0, sizeof(glm::vec2));

        // setup color
        SetUpVBO(VAO, static_cast<GLsizei>(sizeof(glm::vec3) * clr_vtx.size()), 1, 3, clr_vtx.data(), 0, sizeof(glm::vec3));

        SetUpEBO(VAO, idx_vtx);
    }
}