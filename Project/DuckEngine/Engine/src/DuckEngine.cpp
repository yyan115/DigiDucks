#include "DuckEngine.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glslshader.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "CameraManager.h"
#include "SoundManager.h"
#include "InputManager.h"

//include systems
#include "SpriteRendererSystem.h"
#include "CameraSystem.h"
#include "RigidbodySystem.h"
#include "CircleColliderSystem.h"

//GraphicsManager graphicsManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;
SceneManager DuckEngine::DUCKENGINE_SceneManager;



void DuckEngine::Initialize() {
    // need to grab width and height from XML for rubrics in the future
    // Init Window, then Graphics, then Input
    WindowManager::Initialize(1000, 1000, "Test");
    GraphicsManager::Initialize();
    InputManager::Initialize(WindowManager::getWindow());
    CameraManager::Initialize(0.f, 0.f, 10.f);

    // add the systems
    std::shared_ptr<System> spriteRendererSystem = std::make_shared<SpriteRendererSystem>();
    DUCKENGINE_SystemManager.AddSystem(spriteRendererSystem);

    // ADDED BY YY
    std::shared_ptr<System> cameraSystem = std::make_shared<CameraSystem>();
    DUCKENGINE_SystemManager.AddSystem(cameraSystem);

    std::shared_ptr<System> circleColliderSystem = std::make_shared<CircleColliderSystem>();
    DUCKENGINE_SystemManager.AddSystem(circleColliderSystem);

    std::shared_ptr<System> rigidbodySystem = std::make_shared<RigidbodySystem>();
    DUCKENGINE_SystemManager.AddSystem(rigidbodySystem);


    UIManager::Initialize();
    SoundManager::GetInstance().Initialize();
}

void DuckEngine::Update() 
{
    // Look for inputs first
    InputManager::Update();
    // Update dt every 1 second
    TimeManager::UpdateTime(1.0);

    DUCKENGINE_SystemManager.UpdateAll();
    DUCKENGINE_SceneManager.Update();
    //SoundManager::GetInstance().Update();
    
}

void DuckEngine::StartDraw()
{
    // Clear the screen
    //Adapt viewport to resized window
    GLint w{ WindowManager::GetWindowWidth() }, h{ WindowManager::GetWindowHeight() };
    static GLint old_w{}, old_h{};
    if (w != old_w || h != old_h)
    {
        old_w = w;
        old_h = h;
        glViewport(0, 0, w, h);
    }
}

void DuckEngine::Draw() 
{ 
    //GraphicsManager::OldRender(false);

    GraphicsManager::Render();

    GraphicsManager::RenderDebug();

    UIManager::Render();

    //GraphicsManager::DrawPoint({ 0,0 }, 50000.f);
    //GraphicsManager::DrawLine({ 0,0 }, { 10000.f ,10000.f }, 50000.f);
    //GraphicsManager::DrawRectangle({ 0,0 }, { 10000.f, 10000.f }, { 255.f, 255.f, 0.f, 255.f }, true, {});
    //GraphicsManager::DrawCircle({0,0}, 10000.f);
}

void DuckEngine::EndDraw()
{
    // Swap buffers (assuming glfwSwapBuffers is handled elsewhere)
    glfwSwapBuffers(WindowManager::getWindow());
}

void DuckEngine::Exit() 
{ 
    UIManager::Exit();
    WindowManager::Exit();
    GraphicsManager::Exit();
    DUCKENGINE_SceneManager.Shutdown();
    //SoundManager::GetInstance().Exit();
}

bool DuckEngine::Running() {
    if (!WindowManager::CloseWindow())
        return true;
    else
        return false;
}

// DO NOT PARSE NORMALIZED!!!!!!!!!!!!!!!!!!!!! MUST BE 0 TO 255!!!!
void DuckEngine::SetBackgroundColor(const float r, const float g, const float b, const float a) 
{
    GraphicsManager::SetBackgroundColor(r, g, b, a);
}

int DuckEngine::GetWindowWidth() 
{
    return WindowManager::GetWindowWidth();
}

int DuckEngine::GetWindowHeight() 
{
    return WindowManager::GetWindowHeight();
}

float DuckEngine::DeltaTime()
{
    return static_cast<float>(TimeManager::DT());
}

void DuckEngine::SetCameraPosition(const float x, const float y) {
    CameraManager::SetPosition(x, y);
}

void DuckEngine::SetCameraHeight(const int height) {
    CameraManager::SetHeight(height);
}

void DuckEngine::DrawPoint(const Vector2D& position, float size, const Color& color) {
    // Create a DebugDrawCommand for a point and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::POINT, position, {}, size, color); // position2 is unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

void DuckEngine::DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color) {
    // Create a DebugDrawCommand for a line and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::LINE, start, end, size, color);
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

void DuckEngine::DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color) {
    // Create a DebugDrawCommand for a rectangle and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::RECTANGLE, position, size, 0.f, color); // sizeOrRadius is 0 for rectangles
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

void DuckEngine::DrawCircle(const Vector2D& position, float radius, const Color& color) {
    // Create a DebugDrawCommand for a circle and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::CIRCLE, position, {}, radius, color); // position2 is unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}