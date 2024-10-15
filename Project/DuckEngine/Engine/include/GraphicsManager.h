/******************************************************************************/
/*!
\file       GraphicsManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Defines the GraphicsManager class responsible for managing
            graphics rendering in the game engine, including rendering
            objects, setting background colors, and handling debug rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector2.h"
#include "RenderData.h"
#include "Color.h"

#ifdef APIENTRY
#undef APIENTRY
#endif

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/// <summary>
/// The GraphicsManager class is responsible for managing all graphics rendering for game objects, including the 
/// drawing of points, lines, circles, etc, for debugging graphics. It maintains render queues and manages the
/// setup of VAOs (Vertex Array Objects) for different shapes.
/// </summary>
class GraphicsManager {
public:
    /// <summary>
    /// Initializes the GraphicsManager, setting up the required OpenGL and shaders.
    /// </summary>
    /// <returns>Returns true if initialization was successful, false otherwise.</returns>
    static bool Initialize();

    /// <summary>
    /// Cleans up the graphics system and releases any resources used by the GraphicsManager.
    /// </summary>
    static void Exit();

    /// <summary>
    /// Executes the rendering pipeline, drawing all queued objects to the screen.
    /// </summary>
    static void Render();

    /// <summary>
    /// Renders debugging visual elements that have been added to the debug draw queue.
    /// </summary>
    static void RenderDebug();

    /// <summary>
    /// Adds a drawing command to the draw queue, which will be rendered during the next call to Render().
    /// </summary>
    /// <param name="drawOptions">A set of options specifying how the object should be drawn.</param>
    static void AddToDrawQueue(const DrawOptions& drawOptions);

    /// <summary>
    /// Adds a debugging draw command to the debug draw queue, which will be rendered in the next call to RenderDebug().
    /// </summary>
    /// <param name="drawCommand">The draw command specifying how the debug element should be rendered.</param>
    static void AddToDebugDrawQueue(const DebugDrawCommand& drawCommand);

    /// <summary>
    /// Sets the background color for the rendering window.
    /// </summary>
    /// <param name="r">Red component of the background color (0-255).</param>
    /// <param name="g">Green component of the background color (0-255).</param>
    /// <param name="b">Blue component of the background color (0-255).</param>
    /// <param name="a">Alpha component of the background color (0-255).</param>
    static void SetBackgroundColor(float r, float g, float b, float a);

    // Method to initialize the FBO
    DUCKENGINE_API static bool InitializeFBO(int width, int height);

    // Method to bind the FBO for offscreen rendering
    static void BindFBO();

    // Method to unbind the FBO (return to default framebuffer)
    static void UnbindFBO();

    // Get the texture from FBO
    DUCKENGINE_API static GLuint GetFBOTexture();

private:
    /// <summary>
    /// Draws a point at the specified position with the given size and color.
    /// </summary>
    /// <param name="position">The 2D position of the point.</param>
    /// <param name="size">The size of the point.</param>
    /// <param name="color">The color of the point (default is red).</param>
    /// <param name="useCamera">Indicates whether to use the camera view matrix for rendering.</param>
    /// <param name="cameraViewMatrix">The camera view matrix to apply (default is an empty matrix).</param>
    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});

    /// <summary>
    /// Draws a line between two points with the specified size and color.
    /// </summary>
    /// <param name="start">The starting position of the line.</param>
    /// <param name="end">The ending position of the line.</param>
    /// <param name="size">The thickness of the line.</param>
    /// <param name="color">The color of the line (default is red).</param>
    /// <param name="useCamera">Indicates whether to use the camera view matrix for rendering.</param>
    /// <param name="cameraViewMatrix">The camera view matrix to apply (default is an empty matrix).</param>
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});

    /// <summary>
    /// Draws a circle at the specified position with the given radius and color.
    /// </summary>
    /// <param name="position">The center position of the circle.</param>
    /// <param name="radius">The radius of the circle.</param>
    /// <param name="color">The color of the circle (default is red).</param>
    /// <param name="useCamera">Indicates whether to use the camera view matrix for rendering.</param>
    /// <param name="cameraViewMatrix">The camera view matrix to apply (default is an empty matrix).</param>
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool useCamera = true, const glm::mat3x3& cameraViewMatrix = {});

    /// <summary>
    /// Draws a rectangle at the specified position with the given size, rotation, and color.
    /// </summary>
    /// <param name="center">The center position of the rectangle.</param>
    /// <param name="size">The size (width and height) of the rectangle.</param>
    /// <param name="rotation">The rotation of the rectangle (in degrees).</param>
    /// <param name="color">The color of the rectangle.</param>
    /// <param name="useCamera">Indicates whether to use the camera view matrix for rendering.</param>
    /// <param name="cameraViewMatrix">The camera view matrix to apply.</param>
    static void DrawRectangle(const Vector2D& center, const Vector2D& size, float rotation, const Color& color, bool useCamera, const glm::mat3x3& cameraViewMatrix);

    /// <summary>
    /// Sets up the VAO (Vertex Array Object) for drawing points.
    /// </summary>
    static void SetupPointVAO();

    /// <summary>
    /// Sets up the VAO (Vertex Array Object) for drawing lines.
    /// </summary>
    static void SetupLineVAO();

    /// <summary>
    /// Sets up the VAO (Vertex Array Object) for drawing rectangles.
    /// </summary>
    static void SetupRectangleVAO();

    /// <summary>
    /// Sets up the VAO (Vertex Array Object) for drawing circles, with the specified number of segments.
    /// </summary>
    /// <param name="segments">The number of segments used to approximate the circle.</param>
    static void SetupCircleVAO(int segments);

    /// <summary>
    /// Initializes the shader system used for single mesh rendering.
    /// </summary>
    static void InitializeSingleMeshShaderSystem();

    /// <summary>
    /// Initializes the shader system used for debugging visuals.
    /// </summary>
    static void InitializeDebugShaderSystem();

    // Variables for VAOs
    static GLuint VAO;
    static std::vector<DrawOptions> drawQueue;
    static std::vector<DebugDrawCommand> debugDrawQueue;
    static Color backgroundColor;

    // Debug-specific VAOs
    static GLuint pointVAO;
    static GLuint lineVAO;
    static GLuint rectVAO;
    static GLuint circleVAO;

    static int circleSegments;

    static GLuint fbo;
    static GLuint fboTexture;
    static GLuint depthStencil;
};