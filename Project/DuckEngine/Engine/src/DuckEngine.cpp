/******************************************************************************/
/*!
\file       DuckEngine.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the DuckEngine class, which manages the initialization,
            update, rendering, and exit processes of the game engine. It handles
            the coordination of various systems such as input, graphics, and
            camera management.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "DuckEngine.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "FontManager.h"
#include "CameraManager.h"

//include systems
#include "SpriteRendererSystem.h"
#include "RigidbodySystem.h"
#include "CircleColliderSystem.h"
#include "BoxColliderSystem.h"
#include "AnimatorSystem.h"
#include "SoundSystem.h"
#include "ButtonSystem.h"
#include "TextSystem.h"

//GraphicsManager graphicsManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;
SceneManager DuckEngine::DUCKENGINE_SceneManager;
AssetManager DuckEngine::DUCKENGINE_AssetManager;
EntityFactory DuckEngine::DUCKENGINE_EntityFactory;
PrefabManager DuckEngine::DUCKENGINE_PrefabManager;
LevelManager DuckEngine::DUCKENGINE_LevelManager;

bool DuckEngine::showDebugDraw = false;

bool DuckEngine::isEditor = false;
bool isPlaying = false;
Vector2D DuckEngine::editorMouseWorldPos;
Vector2D DuckEngine::editorMouseScreenPos;
Vector2D DuckEngine::editorContentRegion;
//TextRenderingSystem textRenderingSystem;

double DuckEngine::accumulatedTime = 0.0;
int DuckEngine::currentSteps = 0;
const double DuckEngine::FIXED_TIMESTEP = 1.0 / 60.0;

/************************************************************************
@brief Initializes the DuckEngine by setting up the window, graphics, input,
       camera, and font managers. It also loads prefabs and adds systems
       such as sprite rendering and collision systems to the SystemManager.
*************************************************************************/
void DuckEngine::Initialize(bool _isEditor) 
{
    isEditor = _isEditor;
    // need to grab width and height from XML for rubrics in the future
    // Init Window, then Graphics, then Input
    Serialization::InitJson("Resources/windows_init.json");
    WindowInit window = Serialization::GetWindowInit();
    WindowManager::Initialize(1600, 900, "HEHE");
    GraphicsManager::Initialize();
    InputManager::Initialize(WindowManager::getWindow());
    CameraManager::Initialize(0.f, 0.f, 10);
    //FontManager::Initialize("../Resources/Roboto-Black.ttf", 48);
    // load prefabs
    PrefabManager::LoadPrefabsFromFile("Resources/Prefab.json");

    // add the systems
    std::shared_ptr<System> spriteRendererSystem = std::make_shared<SpriteRendererSystem>();
    DUCKENGINE_SystemManager.AddSystem(spriteRendererSystem);

    //// ADDED BY YY
    //std::shared_ptr<System> cameraSystem = std::make_shared<CameraSystem>();
    //DUCKENGINE_SystemManager.AddSystem(cameraSystem);

    // Update Collision System
    // Circle Collider System
    std::shared_ptr<System> circleColliderSystem = std::make_shared<CircleColliderSystem>();
    DUCKENGINE_SystemManager.AddSystem(circleColliderSystem);

    // Box Collider System
    std::shared_ptr<System> boxColliderSystem = std::make_shared<BoxColliderSystem>();
    DUCKENGINE_SystemManager.AddSystem(boxColliderSystem);

    std::shared_ptr<System> rigidbodySystem = std::make_shared<RigidbodySystem>();
    DUCKENGINE_SystemManager.AddSystem(rigidbodySystem);

    std::shared_ptr<System> animatorSystem = std::make_shared<AnimatorSystem>();
    DUCKENGINE_SystemManager.AddSystem(animatorSystem);

    std::shared_ptr<System> soundSystem = std::make_shared<SoundSystem>();
    DUCKENGINE_SystemManager.AddSystem(soundSystem);

    //std::shared_ptr<System> textRenderingSystem = std::make_shared<TextRenderingSystem>();
    //DUCKENGINE_SystemManager.AddSystem(textRenderingSystem);

    //std::shared_ptr<System> textRenderingSystem = std::make_shared<TextRenderingSystem>();
    //DUCKENGINE_SystemManager.AddSystem(textRenderingSystem);

    auto textSystem = std::make_shared<TextSystem>();
    DUCKENGINE_SystemManager.AddSystem(textSystem);

    auto buttonSystem = std::make_shared<ButtonSystem>();
    DUCKENGINE_SystemManager.AddSystem(buttonSystem);

    // start all systems
    DUCKENGINE_SystemManager.StartAll();

    DuckEngine::DUCKENGINE_AssetManager.LoadAll();
    GraphicsManager::Start();

}

void DuckEngine::SetPlaying(bool playing)
{
    isPlaying = playing;
}

bool DuckEngine::IsPlaying()
{
    return isPlaying;
}

/************************************************************************
@brief Updates the DuckEngine by processing inputs, updating systems,
       handling scene management, and rendering text. It also updates
       the window title with the current FPS.
*************************************************************************/
void DuckEngine::Update()
{
    // Look for inputs first   
    TimeManager::StartManagerTimer("Input System");
    TimeManager::EndManagerTimer("Input System");

    // Update viewport
    WindowManager::UpdateViewportDimensions();

    // Get the actual frame time
    TimeManager::UpdateTime(1.0);
    double frameTime = TimeManager::DT();

    // Accumulate time
    accumulatedTime += frameTime;
    currentSteps = 0;

    // Update in fixed timesteps
    while (accumulatedTime >= FIXED_TIMESTEP)
    {
        // Fixed update step
        TimeManager::StartManagerTimer("Systems Update");

        CameraManager::Update();

        DUCKENGINE_SystemManager.UpdateAll();

        if (isEditor && isPlaying)
        {
            DUCKENGINE_SceneManager.Update();
        }
        else if (!isEditor)
        {
            DUCKENGINE_SceneManager.Update();
        }

        TimeManager::EndManagerTimer("Systems Update");

        accumulatedTime -= FIXED_TIMESTEP;
        currentSteps++;

        // Safety check to prevent spiral of death
        if (currentSteps > 5)
        {
            accumulatedTime = 0.0;
            break;
        }
    }

    // Render at whatever FPS we can achieve
    TimeManager::StartManagerTimer("Font System");
    FontManager::Update();
    TimeManager::EndManagerTimer("Font System");

    DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));
}

/************************************************************************
@brief Prepares the rendering process for a new frame by clearing the screen
       and adapting the viewport to any window size changes.
*************************************************************************/
void DuckEngine::StartDraw(GLint width, GLint height)
{
    // Clear the screen
    //Adapt viewport to resized window
    GLint w{ width }, h{ height };
    static GLint old_w{}, old_h{};
    if (w != old_w || h != old_h)
    {
        old_w = w;
        old_h = h;
        glViewport(0, 0, w, h);
        std::cout << "caalled\n";
    }
}

/************************************************************************
@brief Executes the rendering of the game, including rendering the graphics,
       debug elements, and the UI if the editor mode is enabled.
*************************************************************************/
void DuckEngine::Draw() 
{ 
    //GraphicsManager::OldRender(false);
    TimeManager::StartManagerTimer("Graphics System");
    GraphicsManager::Render();
    GraphicsManager::RenderDebug();
    TimeManager::EndManagerTimer("Graphics System");

    InputManager::Update();
    DuckEngine::DUCKENGINE_SystemManager.RenderAll();
    DuckEngine::DUCKENGINE_SceneManager.Render();
    
    TimeManager::StartManagerTimer("Font System");
    FontManager::Render();
    TimeManager::EndManagerTimer("Font System");
    
}

/************************************************************************
@brief Finalizes the rendering process by swapping the window buffers to
       display the rendered frame.
*************************************************************************/
void DuckEngine::EndDraw()
{  
    // Swap buffers (assuming glfwSwapBuffers is handled elsewhere)
    glfwSwapBuffers(WindowManager::getWindow());
}

/************************************************************************
@brief Exits the DuckEngine by shutting down various systems and cleaning
       up resources.
*************************************************************************/
void DuckEngine::Exit() 
{ 
    WindowManager::Exit();
    GraphicsManager::Exit();
    //PrefabManager::Exit();
    InputManager::Exit();
    ShaderManager::Exit();

    DUCKENGINE_SceneManager.Shutdown();
    
}

/************************************************************************
@brief Checks if the game engine is still running by determining if the
       window is closed.
@return True if the window is still open, otherwise false.
*************************************************************************/
bool DuckEngine::Running() {
    if (!WindowManager::CloseWindow())
        return true;
    else
        return false;
}

/************************************************************************
@brief Sets the background color of the game using the specified RGBA values.
@param r The red component of the background color (0-255).
@param g The green component of the background color (0-255).
@param b The blue component of the background color (0-255).
@param a The alpha (transparency) component of the background color (0-255).
*************************************************************************/
void DuckEngine::SetBackgroundColor(const float r, const float g, const float b, const float a) 
{
    GraphicsManager::SetBackgroundColor(r, g, b, a);
}

/************************************************************************
@brief Retrieves the width of the game window.
@return The width of the game window.
*************************************************************************/
int DuckEngine::GetWindowWidth() 
{
    return WindowManager::GetWindowWidth();
}

/************************************************************************
@brief Retrieves the height of the game window.
@return The height of the game window.
*************************************************************************/
int DuckEngine::GetWindowHeight() 
{
    return WindowManager::GetWindowHeight();
}

/************************************************************************
@brief Retrieves the delta time between the current and previous frame.
@return The time in seconds between frames.
*************************************************************************/
float DuckEngine::DeltaTime()
{
    return static_cast<float>(FIXED_TIMESTEP);
}

/************************************************************************
@brief Sets the camera position in the game world.
@param x The x-coordinate of the camera.
@param y The y-coordinate of the camera.
*************************************************************************/
void DuckEngine::SetCameraPosition(const float x, const float y) {
    CameraManager::SetPosition(x, y);
}

/************************************************************************
@brief Sets the camera height in the game world.
@param height The new height of the camera.
*************************************************************************/
void DuckEngine::SetCameraHeight(const int height) {
    CameraManager::SetHeight(height);
}

/************************************************************************
@brief Renders text on the screen at a specified position with a given scale
       and color.
@param text The string of text to render.
@param position The position on the screen where the text will appear.
@param scale The scaling factor for the text.
@param color The color of the text.
*************************************************************************/
void DuckEngine::RenderText(const std::string& fontName, const std::string& text, const Vector2D& position, float scale, const Color& color) {
    TextRenderCommand command{
        fontName,
        text,
        position,
        scale,
        color
    };
    FontManager::AddToDrawQueue(command);
}

/************************************************************************
@brief Draws a point on the screen at a specified position, with a given
       size and color.
@param position The position of the point.
@param size The size of the point.
@param color The color of the point (default is red).
@param relativeToCamera Determines if the point should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawPoint(const Vector2D& position, float size, const Color& color, bool relativeToCamera) {
    // Create a DebugDrawCommand for a point and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::POINT, position, {}, size, 0.f, color, relativeToCamera); // position2 and rotation are unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

/************************************************************************
@brief Draws a line between two points on the screen with a specified size
       and color.
@param start The starting point of the line.
@param end The ending point of the line.
@param size The thickness of the line.
@param color The color of the line (default is red).
@param relativeToCamera Determines if the line should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color, bool relativeToCamera) {
    // Create a DebugDrawCommand for a line and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::LINE, start, end, size, 0.f, color, relativeToCamera); // rotation is unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

/************************************************************************
@brief Draws a rectangle on the screen using the given minimum and maximum
       corner positions, with optional rotation.
@param minCorner The position of the bottom-left corner of the rectangle.
@param maxCorner The position of the top-right corner of the rectangle.
@param rotation The rotation angle of the rectangle (default is 0 degrees).
@param color The color of the rectangle (default is red).
@param relativeToCamera Determines if the rectangle should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, float rotation, const Color& color, bool relativeToCamera) {
    // Calculate the center of the rectangle
    Vector2D center = (minCorner + maxCorner) * 0.5f;

    // Calculate the size (width and height) of the rectangle
    Vector2D size = maxCorner - minCorner;

    // Create a DebugDrawCommand for a rectangle with rotation and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::RECTANGLE, center, size, 0.f, rotation, color, relativeToCamera); // sizeOrRadius is 0 for rectangles
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

/************************************************************************
@brief Draws a circle at a specified position with a given radius and color.
@param position The position of the circle's center.
@param radius The radius of the circle.
@param color The color of the circle (default is red).
@param relativeToCamera Determines if the circle should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawCircle(const Vector2D& position, float radius, const Color& color, bool relativeToCamera) {
    // Create a DebugDrawCommand for a circle and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::CIRCLE, position, {}, radius, 0.f, color, relativeToCamera); // position2 and rotation are unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

/************************************************************************
@brief Sets the window title.
@param title The new title for the game window.
*************************************************************************/
void DuckEngine::SetWindowTitle(std::string title) {
    WindowManager::SetWindowTitle(title.c_str());
}

/************************************************************************
@brief Retrieves the current frames per second (FPS) of the game.
@return The current FPS.
*************************************************************************/
float DuckEngine::FPS() {
    return static_cast<float>(TimeManager::FPS());
}

/************************************************************************
@brief Enables or disables logging for the DuckEngine component manager.
@param enable True to enable logging, false to disable.
*************************************************************************/
void DuckEngine::EnableLogging(bool enable) {
    DUCKENGINE_ComponentManager.EnableLogging(enable);
}

float DuckEngine::GetViewportWidth()
{
    return static_cast<float>(WindowManager::GetViewportWidth());
}

float DuckEngine::GetViewportHeight()
{
    return static_cast<float>(WindowManager::GetViewportHeight());
} 