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
#include "ShaderManager.h"
#include "FontManager.h"

//include systems
#include "SpriteRendererSystem.h"
#include "CameraSystem.h"
#include "RigidbodySystem.h"
#include "CircleColliderSystem.h"
#include "BoxColliderSystem.h"
#include "TextRenderingSystem.h"
#include "AnimatorSystem.h"

//GraphicsManager graphicsManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;
SceneManager DuckEngine::DUCKENGINE_SceneManager;
AssetManager DuckEngine::DUCKENGINE_AssetManager;
EntityFactory DuckEngine::DUCKENGINE_EntityFactory;
PrefabManager DuckEngine::DUCKENGINE_PrefabManager;

//TextRenderingSystem textRenderingSystem;

void DuckEngine::Initialize() {
    // need to grab width and height from XML for rubrics in the future
    // Init Window, then Graphics, then Input
    Serialization::InitJson("../Resources/windows_init.json");
    WindowInit window = Serialization::GetWindowInit();
    WindowManager::Initialize(window.width, window.height, window.title.c_str());
    GraphicsManager::Initialize();
    InputManager::Initialize(WindowManager::getWindow());
    CameraManager::Initialize(0.f, 0.f, 10);
    FontManager::Initialize("../Resources/Roboto-Black.ttf", 48);

    // load prefabs
    PrefabManager::LoadPrefabsFromFile("../Resources/Prefab.json");

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

    //std::shared_ptr<System> textRenderingSystem = std::make_shared<TextRenderingSystem>();
    //DUCKENGINE_SystemManager.AddSystem(textRenderingSystem);

    //std::shared_ptr<System> textRenderingSystem = std::make_shared<TextRenderingSystem>();
    //DUCKENGINE_SystemManager.AddSystem(textRenderingSystem);

    // start all systems
    DUCKENGINE_SystemManager.StartAll();

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

    FontManager::Update();

    DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));

    //// FIRST INSTANCE
    //TextRenderCommand titleText{
    //"TEST SCENE",       // Text
    //{ 20.f , WindowManager::GetWindowHeight() - 70.f },         // Position (X, Y)
    //1.0f,                  // Scale
    //255.f, 50.f, 100.0f, 250.0f       // Color (R, G, B)
    //};

    //FontManager::AddToDrawQueue(titleText);

    //TextRenderCommand testText {
    //"TEST TEXT",       // Text
    //{ WindowManager::GetWindowWidth() - 300.f  , 250.f },         // Position (X, Y)
    //1.0f,                  // Scale
    //0.f, 255.f, 150.f, 250.0f       // Color (R, G, B)
    //};

    //FontManager::AddToDrawQueue(testText);
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

    FontManager::Render();
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
    //PrefabManager::Exit();
    InputManager::Exit();
    ShaderManager::Exit();

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

void DuckEngine::RenderText(const std::string& text, const Vector2D& position, float scale, const Color& color) {
    TextRenderCommand command{
        text,
        position,
        scale,
        color
    };
    FontManager::AddToDrawQueue(command);
}

void DuckEngine::DrawPoint(const Vector2D& position, float size, const Color& color, bool relativeToCamera) {
    // Create a DebugDrawCommand for a point and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::POINT, position, {}, size, 0.f, color, relativeToCamera); // position2 and rotation are unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

void DuckEngine::DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color, bool relativeToCamera) {
    // Create a DebugDrawCommand for a line and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::LINE, start, end, size, 0.f, color, relativeToCamera); // rotation is unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}

//void DuckEngine::DrawRectangle(const Vector2D& position, const Vector2D& size, float rotation, const Color& color) {
//    // Create a DebugDrawCommand for a rectangle with rotation and add it to the debug draw queue
//    DebugDrawCommand drawCommand(DebugDrawCommand::RECTANGLE, position, size, 0.f, rotation, color); // sizeOrRadius is 0 for rectangles
//    GraphicsManager::AddToDebugDrawQueue(drawCommand);
//}

void DuckEngine::DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, float rotation, const Color& color, bool relativeToCamera) {
    // Calculate the center of the rectangle
    Vector2D center = (minCorner + maxCorner) * 0.5f;

    // Calculate the size (width and height) of the rectangle
    Vector2D size = maxCorner - minCorner;

    // Create a DebugDrawCommand for a rectangle with rotation and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::RECTANGLE, center, size, 0.f, rotation, color, relativeToCamera); // sizeOrRadius is 0 for rectangles
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}


void DuckEngine::DrawCircle(const Vector2D& position, float radius, const Color& color, bool relativeToCamera) {
    // Create a DebugDrawCommand for a circle and add it to the debug draw queue
    DebugDrawCommand drawCommand(DebugDrawCommand::CIRCLE, position, {}, radius, 0.f, color, relativeToCamera); // position2 and rotation are unused
    GraphicsManager::AddToDebugDrawQueue(drawCommand);
}


void DuckEngine::SetWindowTitle(std::string title) {
    WindowManager::SetWindowTitle(title.c_str());
}

float DuckEngine::FPS() {
    return static_cast<float>(TimeManager::FPS());
}