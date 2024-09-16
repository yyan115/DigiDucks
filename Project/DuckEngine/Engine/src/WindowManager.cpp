#include <iostream>

#include "WindowManager.h"

GLFWwindow* WindowManager::ptrWindow = nullptr;

GLint WindowManager::width;
GLint WindowManager::height;

bool WindowManager::Initialize(GLint width, GLint height, std::string title) {
    WindowManager::width = width;
    WindowManager::height = height;
    title = title;

    // Check if glfw init success
    if (!glfwInit()) {
        std::cout << "GLFW init has failed - abort program!!!" << std::endl;
        return false;
    }

    // If GLFW function fails, callback error
    glfwSetErrorCallback(error_cb);

    // Setup GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);

    // Create window and check if success
    ptrWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!ptrWindow) {
        std::cerr << "GLFW unable to create OpenGL context - abort program\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(ptrWindow);

    // Set callback for FB size change
    glfwSetFramebufferSizeCallback(ptrWindow, fbsize_cb);

    return true;
}

GLFWwindow* WindowManager::getWindow() {
    return ptrWindow;
}

bool WindowManager::CloseWindow() {
    if (glfwWindowShouldClose(ptrWindow))
        return true;

    return false;
}

void WindowManager::Exit() {
    glfwDestroyWindow(ptrWindow);
    glfwTerminate();
}

void WindowManager::error_cb(int error, char const* description) {
#ifdef _DEBUG
    std::cerr << "GLFW error: " << description << std::endl;
#endif
}

void WindowManager::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
#ifdef _DEBUG
    std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    WindowManager::width = width;
    WindowManager::height = height;
}

GLint WindowManager::GetWindowWidth()
{
    return width;
}
GLint WindowManager::GetWindowHeight()
{
    return height;
}