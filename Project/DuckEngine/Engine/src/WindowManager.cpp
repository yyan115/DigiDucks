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

#define UNREFERENCED_PARAMETER(P) (P)

GLFWwindow* WindowManager::ptrWindow = nullptr;
GLint WindowManager::width;
GLint WindowManager::height;
const char* WindowManager::title;

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
    title = _title;

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

/// <summary>
/// Returns a pointer to the current GLFW window managed by the window manager.
/// </summary>
/// <returns>A pointer to the GLFWwindow object.</returns>
GLFWwindow* WindowManager::getWindow() {
    return ptrWindow;
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

/// <summary>
/// Sets the title of the window to the specified string.
/// </summary>
/// <param name="_title">The new title of the window.</param>
void WindowManager::SetWindowTitle(const char* _title) {
    glfwSetWindowTitle(ptrWindow, _title);
}