#include "DuckEngine.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glslshader.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "UIManager.h"



//include systems
#include "SpriteRendererSystem.h"
#include "CameraSystem.h"


//GraphicsManager graphicsManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;



void DuckEngine::Initialize() {
    // need to grab width and height from XML for rubrics in the future
    // Init Window, then Graphics, then Input
    WindowManager::Initialize(1000, 1000, "Test");
    GraphicsManager::Initialize();
    InputManager::Initialize();
 

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

    glClear(GL_COLOR_BUFFER_BIT);
}

void DuckEngine::Draw() 
{ 

    GraphicsManager::Render(false);

    UIManager::Render();

    GraphicsManager::DrawPoint({ 0,0 }, 50000.f);
    GraphicsManager::DrawLine({ 0,0 }, { 10000.f ,10000.f }, 50000.f);
    GraphicsManager::DrawRectangle({ 0,0 }, { 10000.f, 10000.f });
    GraphicsManager::DrawCircle({0,0}, 10000.f);
}

void DuckEngine::EndDraw()
{
    // Swap buffers (assuming glfwSwapBuffers is handled elsewhere)
    glfwSwapBuffers(WindowManager::getWindow());
}

void DuckEngine::Exit() { 
    UIManager::Shutdown();
    WindowManager::Exit();
}

    bool DuckEngine::Running() {
        if (!WindowManager::CloseWindow())
            return true;
        else
            return false;

    
}

int DuckEngine::GetWindowWidth() {
    return WindowManager::GetWindowWidth();
}

int DuckEngine::GetWindowHeight() {
    return WindowManager::GetWindowHeight();
}