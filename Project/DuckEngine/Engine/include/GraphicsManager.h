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

#include "Color.h"

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
    GLuint* texture = 0;

    // OPTIONAL
    bool relativeToCamera = false;
};

struct DebugDrawCommand {
    enum Type { POINT, LINE, RECTANGLE, CIRCLE } type;

    Vector2D position1, position2; // Position1 for circle/point/rect, position2 for line/rect
    float sizeOrRadius;            // Size for point/line/rectangle, radius for circle
    Color color;                   // RGBA color
    bool relativeToCamera;

    // Single constructor for all types
    DebugDrawCommand(Type t, const Vector2D& pos1, const Vector2D& pos2, float sizeOrRadius, const Color& color, bool relativeToCamera = true)
        : type(t), position1(pos1), position2(pos2), sizeOrRadius(sizeOrRadius), color(color), relativeToCamera(relativeToCamera)
    {
        switch (type) {
        case POINT:
            // For POINT, we only need position1 and size (sizeOrRadius)
            position2 = { 0.f, 0.f };  // position2 is unused
            break;

        case LINE:
            // For LINE, we need both position1 (start) and position2 (end)
            break;

        case RECTANGLE:
            // For RECTANGLE, position1 is the bottom-left corner, and position2 is the size (width, height)
            break;

        case CIRCLE:
            // For CIRCLE, position1 is the center, and sizeOrRadius is the radius
            position2 = { 0.f, 0.f };  // position2 is unused
            break;

        default:
            throw std::invalid_argument("Invalid type for DebugDrawCommand");
        }
    }
};


class DUCKENGINE_API GraphicsManager {
public:
    // Core functions
    static bool Initialize();
    static void Exit();
    static void Render();
    static void RenderDebug();

    static void AddToDrawQueue(const DrawOptions &drawOptions);

    static void SetBackgroundColor(float r, float g, float b, float a);

    static void AddToDebugDrawQueue(const DebugDrawCommand& drawCommand);

    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    //static void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});

    static void DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix);

    static void SetupPointVAO();
    static void SetupLineVAO();
    static void SetupRectangleVAO();
    static void SetupCircleVAO(int segments);

private:
    static void InitializeSingleMeshShaderSystem();

    static void InsertShader(std::string shdr_pgm_name,
        std::string vtx_shdr,
        std::string frg_shdr);

    static void InsertDebugShader();

    // Private members
    //std::map<std::string, Texture> textures;   // Stores textures by their names
    static std::map<std::string, GLSLShader> shaders;     // Stores shaders by their names

    static GLuint VAO;

    static std::vector<DrawOptions> drawQueue;

    static std::vector<DebugDrawCommand> debugDrawQueue;

    static Color backgroundColor;

//#ifdef DEBUG
    static GLuint pointVAO;
    static GLuint lineVAO;
    static GLuint rectVAO;
    static GLuint circleVAO;

    static int circleSegments;
//#endif // DEBUG

};