#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector2.h"
#include "RenderData.h"
#include "Color.h"

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

private:

    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    //static void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});

    static void DrawRectangle(const Vector2D& center, const Vector2D& size, float rotation, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix);

    static void SetupPointVAO();
    static void SetupLineVAO();
    static void SetupRectangleVAO();
    static void SetupCircleVAO(int segments);

    static void InitializeSingleMeshShaderSystem();

    static void InitializeDebugShaderSystem();

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