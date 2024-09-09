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

struct TransformData {
    glm::vec3 scale;
    float rotation;
    glm::vec3 position;
};

struct Color {
    float r;
    float g;
    float b;
    float a;
};


class DUCKENGINE_API GraphicsManager {
public:

    //GraphicsManager() {}
    //~GraphicsManager() {}

    // Core functions
    static bool Initialize();               // Initialize OpenGL, GLFW, shaders, etc.
    static void Exit();                 // Cleanup resources
    static void Render();                   // Render everything for this frame
    
    // FEATURES: DRAW NORMALLY (NO COLORS, NO TEXTURES), OR DRAW WITH COLOR NO TEXT, OR DRAW WITH TEXT NO COLOR, OR DRAW WITH TEXT + COLORR
    static void AddToDrawQueue(const glm::vec3& scale, float rotation, const glm::vec3& translate);
    //static void AddToDrawQueue(const glm::vec3& scale, float rotation, const glm::vec3& translate, //TEXTURE DRAW);
    static void AddToDrawQueue(const glm::vec3& scale, float rotation, const glm::vec3& translate, Color RGBA);
    // static void AddToDrawQueue(const glm::vec3& scale, float rotation, const glm::vec3& translate, Color RGBA, //TEXTURE DRAW);

    static void SetBackgroundColor(float r, float g, float b, float a);

    // Public API for other components
    //void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotation = 0.0f);    
    //void SetViewMatrix(glm::mat4 viewMatrix); // Allow camera or screen transformations

    // Resource management
    static void LoadTexture(const std::string& filePath);    // Load texture from file

    //static void LoadShader(const std::string& vertexPath, const std::string& fragmentPath); // Shader loader

    //void error_cb(int error, char const* description);
    //void fbsize_cb(GLFWwindow* ptr_win, int width, int height);

private:
    static std::vector<TransformData> transforms;

    static void InitializeSingleMeshShaderSystem();

    // Private helper functions
    //void LoadResources();            // Load all needed assets
    //void SetupShaders();             // Initialize shaders

    static void InsertShader(std::string shdr_pgm_name,
        std::string vtx_shdr,
        std::string frg_shdr);

    // Private members
    //std::map<std::string, Texture> textures;   // Stores textures by their names
    static std::map<std::string, GLSLShader> shaders;     // Stores shaders by their names

    //
    //// GLM matrices for transformations
    //glm::mat4 projectionMatrix;
    //glm::mat4 viewMatrix;


    static GLuint VAO;
    static GLuint VBO;

    static GLuint shaderProgram;
};