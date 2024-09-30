#pragma once

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
    // GRAB FROM CAMERA MANAGER
    //Vector2D cameraPosition = { 0.f, 0.f };
    //Vector2D cameraWidthHeight = { 0.f, 0.f };
};

struct DebugDrawCommand {
    enum Type { POINT, LINE, RECTANGLE, CIRCLE } type;

    Vector2D position1;      // For POINT, RECTANGLE (center), CIRCLE (center)
    Vector2D position2;      // For LINE (end position), RECTANGLE (size)
    float sizeOrRadius;      // For POINT (size), CIRCLE (radius)
    float rotation;          // For RECTANGLE (rotation in degrees or radians)
    Color color;             // RGBA color
    bool relativeToCamera;

    // Constructor
    DebugDrawCommand(Type t, const Vector2D& pos1, const Vector2D& pos2, float sizeOrRadius, float rotation, const Color& color, bool relativeToCamera = true)
        : type(t), position1(pos1), position2(pos2), sizeOrRadius(sizeOrRadius), rotation(rotation), color(color), relativeToCamera(relativeToCamera)
    {
        switch (type) {
        case POINT:
            // For POINT, position1 is the position, sizeOrRadius is the size
            position2 = { 0.f, 0.f };  // Unused
            rotation = 0.f;            // No rotation for POINT
            break;

        case LINE:
            // For LINE, position1 is the start, position2 is the end
            sizeOrRadius = 0.f;        // Unused
            rotation = 0.f;            // No rotation for LINE
            break;

        case RECTANGLE:
            // For RECTANGLE, position1 is the center, position2 is the size (width, height), rotation is used
            sizeOrRadius = 0.f;        // Unused
            break;

        case CIRCLE:
            // For CIRCLE, position1 is the center, sizeOrRadius is the radius
            position2 = { 0.f, 0.f };  // Unused
            rotation = 0.f;            // No rotation for CIRCLE
            break;
        }
    }
};


class GraphicsManager {
public:
    // Core functions
    static bool Initialize();
    static void Exit();
    static void Render();
    static void RenderDebug();

    static void AddToDrawQueue(const DrawOptions& drawOptions);

    static void SetBackgroundColor(float r, float g, float b, float a);

    static void AddToDebugDrawQueue(const DebugDrawCommand& drawCommand);

    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    //static void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});

    static void DrawRectangle(const Vector2D& center, const Vector2D& size, float rotation, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix);

    static void SetupPointVAO();
    static void SetupLineVAO();
    static void SetupRectangleVAO();
    static void SetupCircleVAO(int segments);

    static void InsertShader(std::string shdr_pgm_name,
        std::string vtx_shdr,
        std::string frg_shdr);

private:
    static void InitializeSingleMeshShaderSystem();

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