#include "DuckEngine.h"
#include <iostream> // For std::cout
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GraphicsManager.h"
#include "glslshader.h"

//include systems
#include "SpriteRendererSystem.h"


//GraphicsManager graphicsManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;


void DuckEngine::Initialize() {
    // need to grab width and height from XML for rubrics in the future
    WindowManager::Initialize(1000, 1000, "Test");
    GraphicsManager::Initialize();


    // add the systems
    std::shared_ptr<System> spriteRendererSystem = std::make_shared<SpriteRendererSystem>();
    DUCKENGINE_SystemManager.AddSystem(spriteRendererSystem);
}

void DuckEngine::Update() 
{
    DUCKENGINE_SystemManager.UpdateAll();
}

void DuckEngine::Draw() { GraphicsManager::Render(); }

void DuckEngine::Exit() { GraphicsManager::Exit(); }

bool DuckEngine::Running() {
    if (!WindowManager::CloseWindow())
        return true;
    else
        return false;
}

//void DuckEngine::Start()
//{
//    std::cout << "DuckEngine Start function called." << std::endl;
//
//    // Initialize GLFW
//    if (!glfwInit())
//    {
//        std::cerr << "Failed to initialize GLFW!" << std::endl;
//        return;
//    }
//
//    // Set GLFW window hints (optional)
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL version 3.x
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version 3.3
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core Profile
//
//    // Create a windowed mode window and OpenGL context
//    GLFWwindow* window = glfwCreateWindow(800, 600, "DuckEngine Test", nullptr, nullptr);
//    if (!window)
//    {
//        std::cerr << "Failed to create GLFW window!" << std::endl;
//        glfwTerminate();
//        return;
//    }
//
//    // Make the window's context current
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLEW
//    GLenum err = glewInit();
//    if (err != GLEW_OK)
//    {
//        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
//        return;
//    }
//
//    // Check OpenGL and GLEW versions
//    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
//
//    // Main loop to keep the window open
//    while (!glfwWindowShouldClose(window))
//    {
//        // Clear the screen
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Swap front and back buffers
//        glfwSwapBuffers(window);
//
//        // Poll for and process events
//        glfwPollEvents();
//    }
//
//    // Clean up and terminate GLFW
//    glfwDestroyWindow(window);
//    glfwTerminate();
//}