#include "DuckEngine.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glslshader.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
//#include "InputManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include "CameraManager.h"

//include systems
#include "SpriteRendererSystem.h"
#include "CameraSystem.h"

//GraphicsManager graphicsManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;

bool DuckEngine::WKeyDown = false;
bool DuckEngine::AKeyDown = false;
bool DuckEngine::SKeyDown = false;
bool DuckEngine::DKeyDown = false;

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

    UIManager::Initialize();
}

void DuckEngine::Update() 
{
    // Look for inputs first
    InputManager::Update();
    // Update dt every 1 second
    TimeManager::UpdateTime(1.0);

    DUCKENGINE_SystemManager.UpdateAll();
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

// Define key constants for letters (A-Z)
const int DuckEngine::KEY_A = GLFW_KEY_A;
const int DuckEngine::KEY_B = GLFW_KEY_B;
const int DuckEngine::KEY_C = GLFW_KEY_C;
const int DuckEngine::KEY_D = GLFW_KEY_D;
const int DuckEngine::KEY_E = GLFW_KEY_E;
const int DuckEngine::KEY_F = GLFW_KEY_F;
const int DuckEngine::KEY_G = GLFW_KEY_G;
const int DuckEngine::KEY_H = GLFW_KEY_H;
const int DuckEngine::KEY_I = GLFW_KEY_I;
const int DuckEngine::KEY_J = GLFW_KEY_J;
const int DuckEngine::KEY_K = GLFW_KEY_K;
const int DuckEngine::KEY_L = GLFW_KEY_L;
const int DuckEngine::KEY_M = GLFW_KEY_M;
const int DuckEngine::KEY_N = GLFW_KEY_N;
const int DuckEngine::KEY_O = GLFW_KEY_O;
const int DuckEngine::KEY_P = GLFW_KEY_P;
const int DuckEngine::KEY_Q = GLFW_KEY_Q;
const int DuckEngine::KEY_R = GLFW_KEY_R;
const int DuckEngine::KEY_S = GLFW_KEY_S;
const int DuckEngine::KEY_T = GLFW_KEY_T;
const int DuckEngine::KEY_U = GLFW_KEY_U;
const int DuckEngine::KEY_V = GLFW_KEY_V;
const int DuckEngine::KEY_W = GLFW_KEY_W;
const int DuckEngine::KEY_X = GLFW_KEY_X;
const int DuckEngine::KEY_Y = GLFW_KEY_Y;
const int DuckEngine::KEY_Z = GLFW_KEY_Z;

// Define key constants for numbers (0-9)
const int DuckEngine::KEY_0 = GLFW_KEY_0;
const int DuckEngine::KEY_1 = GLFW_KEY_1;
const int DuckEngine::KEY_2 = GLFW_KEY_2;
const int DuckEngine::KEY_3 = GLFW_KEY_3;
const int DuckEngine::KEY_4 = GLFW_KEY_4;
const int DuckEngine::KEY_5 = GLFW_KEY_5;
const int DuckEngine::KEY_6 = GLFW_KEY_6;
const int DuckEngine::KEY_7 = GLFW_KEY_7;
const int DuckEngine::KEY_8 = GLFW_KEY_8;
const int DuckEngine::KEY_9 = GLFW_KEY_9;

// Define key constants for common keys
const int DuckEngine::KEY_SPACE = GLFW_KEY_SPACE;
const int DuckEngine::KEY_ENTER = GLFW_KEY_ENTER;
const int DuckEngine::KEY_ESCAPE = GLFW_KEY_ESCAPE;
const int DuckEngine::KEY_LEFT = GLFW_KEY_LEFT;
const int DuckEngine::KEY_RIGHT = GLFW_KEY_RIGHT;
const int DuckEngine::KEY_UP = GLFW_KEY_UP;
const int DuckEngine::KEY_DOWN = GLFW_KEY_DOWN;
const int DuckEngine::KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const int DuckEngine::KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
const int DuckEngine::KEY_BACKSPACE = GLFW_KEY_BACKSPACE;