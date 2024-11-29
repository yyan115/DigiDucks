/******************************************************************************/
/*!
\file       WindowManager.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the WindowManager class, handling the creation of the
            game window, responding to window events such as resizing, and
            managing the window's lifecycle.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <iostream>

#include "WindowManager.h"
#include "DuckEngine.h"

#define UNREFERENCED_PARAMETER(P) (P)

GLFWwindow* WindowManager::ptrWindow = nullptr;
GLint WindowManager::width;
GLint WindowManager::height;
GLint WindowManager::viewportWidth;
GLint WindowManager::viewportHeight;
const char* WindowManager::title;

bool WindowManager::isFullscreen = false;
GLint WindowManager::windowedWidth = 1600;   // Default windowed size
GLint WindowManager::windowedHeight = 900;  // Default windowed size
GLint WindowManager::windowedPosX = 0;      // Default window position
GLint WindowManager::windowedPosY = 0;      // Default window position

/// <summary>
/// Initializes the window manager by creating a GLFW window with the specified dimensions and title.
/// Also sets up OpenGL context and GLFW callbacks.
/// </summary>
/// <param name="_width">The width of the window in pixels.</param>
/// <param name="_height">The height of the window in pixels.</param>
/// <param name="_title">The title of the window.</param>
/// <returns>Returns true if the window is successfully created, false otherwise.</returns>
bool WindowManager::Initialize(GLint _width, GLint _height, const char* _title) {
    WindowManager::width = _width;
    WindowManager::height = _height;
    WindowManager::viewportWidth = _width;
    WindowManager::viewportHeight = _height;
    title = _title;

    windowedWidth = _width;
    windowedHeight = _height;

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
    ptrWindow = glfwCreateWindow(width, height, title, NULL, NULL);
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

void WindowManager::ToggleFullscreen() {
    if (isFullscreen) {
        // Restore to windowed mode
        glfwSetWindowMonitor(ptrWindow, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
    }
    else {
        // Save current window position and size
        glfwGetWindowPos(ptrWindow, &windowedPosX, &windowedPosY);
        glfwGetWindowSize(ptrWindow, &windowedWidth, &windowedHeight);

        // Get the primary monitor and its video mode
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

        // Switch to fullscreen
        glfwSetWindowMonitor(ptrWindow, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    isFullscreen = !isFullscreen; // Toggle fullscreen state
}



void WindowManager::UpdateViewportDimensions() {
    if (DuckEngine::isEditor) {
        Vec2 newViewportSize = DuckEngine::editorContentRegion;
        viewportWidth = static_cast<GLint>(newViewportSize.x);
        viewportHeight = static_cast<GLint>(newViewportSize.y);
    }
    else {
        // Fallback to window dimensions if ImGui is not initialized
        viewportWidth = width;
        viewportHeight = height;
    }
}

/// <summary>
/// Returns a pointer to the current GLFW window managed by the window manager.
/// </summary>
/// <returns>A pointer to the GLFWwindow object.</returns>
GLFWwindow* WindowManager::getWindow() {
    return ptrWindow;
}

void WindowManager::SetWindowShouldClose()
{
    glfwSetWindowShouldClose(ptrWindow, 1);
}

/// <summary>
/// Checks if the window should close, typically when the user presses a close button.
/// </summary>
/// <returns>Returns true if the window should close, false otherwise.</returns>
bool WindowManager::CloseWindow() {
    if (glfwWindowShouldClose(ptrWindow))
        return true;

    return false;
}

/// <summary>
/// Cleans up the window manager by destroying the GLFW window and terminating GLFW.
/// </summary>
void WindowManager::Exit() {
    glfwDestroyWindow(ptrWindow);
    glfwTerminate();
}

/// <summary>
/// Error callback function that handles GLFW errors. Outputs the error description to the console.
/// </summary>
/// <param name="error">The GLFW error code.</param>
/// <param name="description">A description of the error.</param>
void WindowManager::error_cb(int error, char const* description) {
#ifdef _DEBUG
    std::cerr << "GLFW error: " << description << ", " << error << std::endl;
#else
    (void)error;        // Avoid unused parameter warning
    (void)description;  // Avoid unused parameter warning
#endif
}


/// <summary>
/// Framebuffer size callback function that updates the window's width and height when the window is resized.
/// </summary>
/// <param name="ptr_win">Pointer to the GLFW window that was resized.</param>
/// <param name="_width">The new width of the window in pixels.</param>
/// <param name="_height">The new height of the window in pixels.</param>
void WindowManager::fbsize_cb(GLFWwindow* ptr_win, int _width, int _height) {
    UNREFERENCED_PARAMETER(ptr_win);

#ifdef _DEBUG
    std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    WindowManager::width = _width;
    WindowManager::height = _height;

    glViewport(0, 0, _width, _height);
}

/// <summary>
/// Returns the current width of the window in pixels.
/// </summary>
/// <returns>The width of the window in pixels.</returns>
GLint WindowManager::GetWindowWidth()
{
    return width;
}

/// <summary>
/// Returns the current height of the window in pixels.
/// </summary>
/// <returns>The height of the window in pixels.</returns>
GLint WindowManager::GetWindowHeight()
{
    return height;
}

GLint WindowManager::GetViewportWidth()
{
    std::cout << "viewportW: " << viewportWidth << ", normalW: " << width << "\n";
    return viewportWidth;
}

GLint WindowManager::GetViewportHeight()
{
    return viewportHeight;
}

/// <summary>
/// Sets the title of the window to the specified string.
/// </summary>
/// <param name="_title">The new title of the window.</param>
void WindowManager::SetWindowTitle(const char* _title) {
    glfwSetWindowTitle(ptrWindow, _title);
}