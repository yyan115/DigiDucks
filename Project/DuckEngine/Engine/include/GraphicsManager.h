#pragma once

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
#include "Vector2.h"

struct Color {
    float r;
    float g;
    float b;
    float a;
};

struct DrawOptions {
    // NECESSARY
    Vector2D scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    Vector2D translation = { 0.0f, 0.0f };

    // OPTIONAL
    bool useColor = false;
    Color color = { 255, 255, 255, 255 }; // Default white color

    // OPTIONAL
    bool useTexture = false;
    GLuint* texture;

    // OPTIONAL
    bool relativeToCamera = false;
    // GRAB FROM CAMERA MANAGER
    //Vector2D cameraPosition = { 0.f, 0.f };
    //Vector2D cameraWidthHeight = { 0.f, 0.f };
};



class DUCKENGINE_API GraphicsManager {
public:

    //GraphicsManager() {}
    //~GraphicsManager() {}

    // Core functions
    static bool Initialize();               // Initialize OpenGL, GLFW, shaders, etc.
    static void Exit();                 // Cleanup resources
    static void OldRender(bool isUI = false);                   // Render everything for this frame
    static void Render();
    
    // FEATURES: DRAW NORMALLY (NO COLORS, NO TEXTURES), OR DRAW WITH COLOR NO TEXT, OR DRAW WITH TEXT NO COLOR, OR DRAW WITH TEXT + COLORR
    static void AddToDrawQueue(const DrawOptions &drawOptions);
    // static void AddToDrawQueue(const Vector2D& scale, float rotation, const Vector2D& translate);
    // static void AddToDrawQueue(const glm::vec3& scale, float rotation, const glm::vec3& translate, //TEXTURE DRAW);
    // static void AddToDrawQueue(const Vector2D& scale, float rotation, const Vector2D& translate, Color RGBA);
    // static void AddToDrawQueue(const glm::vec3& scale, float rotation, const glm::vec3& translate, Color RGBA, //TEXTURE DRAW);

    static void SetBackgroundColor(float r, float g, float b, float a);

    // Resource management
    static void LoadTexture(const std::string& filePath);    // Load texture from file

    //static void LoadShader(const std::string& vertexPath, const std::string& fragmentPath); // Shader loader

    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f });

    static void SetupPointVAO();
    static void SetupLineVAO();
    static void SetupRectangleVAO();
    static void SetupCircleVAO(int segments);

private:
    static std::vector<glm::mat3x3> transforms;

    static void InitializeSingleMeshShaderSystem();

    static void InsertShader(std::string shdr_pgm_name,
        std::string vtx_shdr,
        std::string frg_shdr);

    static void InsertDebugShader();

    // Private members
    //std::map<std::string, Texture> textures;   // Stores textures by their names
    static std::map<std::string, GLSLShader> shaders;     // Stores shaders by their names

    static GLuint VAO;
    static GLuint VBO;

    static GLuint indicesCount;

    static GLuint shaderProgram;

    static std::vector<DrawOptions> drawQueue;

    static Color backgroundColor;

//#ifdef DEBUG
    static GLuint pointVAO;
    static GLuint lineVAO;
    static GLuint rectVAO;
    static GLuint circleVAO;

    static int circleSegments;
//#endif // DEBUG

};