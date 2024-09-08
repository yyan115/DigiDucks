#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glslshader.h"


class DUCKENGINE_API GraphicsManager {
public:

    //GraphicsManager() {}
    //~GraphicsManager() {}

    // Core functions
    bool Initialize(GLint width, GLint height, std::string title);               // Initialize OpenGL, GLFW, shaders, etc.
    void Shutdown();                 // Cleanup resources
    void Render();                   // Render everything for this frame

    bool CloseWindow();

    // Public API for other components
    //void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotation = 0.0f);    
    //void SetViewMatrix(glm::mat4 viewMatrix); // Allow camera or screen transformations

    // Resource management
    void LoadTexture(const std::string& filePath);    // Load texture from file
    void LoadShader(const std::string& vertexPath, const std::string& fragmentPath); // Shader loader

    //void error_cb(int error, char const* description);
    //void fbsize_cb(GLFWwindow* ptr_win, int width, int height);

private:

    // Private helper functions
    void LoadResources();            // Load all needed assets
    void SetupShaders();             // Initialize shaders

    void InsertShader(std::string shdr_pgm_name,
        std::string vtx_shdr,
        std::string frg_shdr);

    GLint width, height;
    //static GLdouble fps;
    //static GLdouble delta_time; // time taken to complete most recent game loop
    std::string title;
    GLFWwindow* ptr_window;

    // Private members
    //std::map<std::string, Texture> textures;   // Stores textures by their names
    std::map<std::string, GLSLShader> shaders;     // Stores shaders by their names

    //
    //// GLM matrices for transformations
    //glm::mat4 projectionMatrix;
    //glm::mat4 viewMatrix;


    GLuint VAO, VBO, shaderProgram;  // Buffers and shader program for rendering
    GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource);
};