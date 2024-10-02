#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <GLFW/glfw3.h>
#include <string>

/// <summary>
/// Manages the creation, handling, and cleanup of the main game window. 
/// Provides utility functions to query and modify window properties such as size and title.
/// </summary>
class DUCKENGINE_API WindowManager {
public:
    /// <summary>
    /// Initializes the window manager and creates a GLFW window with the specified dimensions and title.
    /// </summary>
    /// <param name="width">The width of the window in pixels.</param>
    /// <param name="height">The height of the window in pixels.</param>
    /// <param name="title">The title of the window.</param>
    /// <returns>Returns true if the window was successfully created, false otherwise.</returns>
    static bool Initialize(GLint width, GLint height, const char* title);

    /// <summary>
    /// Cleans up and closes the window, releasing any associated resources.
    /// </summary>
    static void Exit();

    /// <summary>
    /// Returns a pointer to the current GLFW window.
    /// </summary>
    /// <returns>A pointer to the GLFWwindow object.</returns>
    static GLFWwindow* getWindow();

    /// <summary>
    /// Checks if the window should close, typically when the user closes the window or presses a close button.
    /// </summary>
    /// <returns>Returns true if the window should close, false otherwise.</returns>
    static bool CloseWindow();

    /// <summary>
    /// Error callback function to handle GLFW errors. Prints the error description to the console.
    /// </summary>
    /// <param name="error">The GLFW error code.</param>
    /// <param name="description">A description of the error.</param>
    static void error_cb(int error, char const* description);

    /// <summary>
    /// Framebuffer size callback function to handle window resize events. Updates the window width and height.
    /// </summary>
    /// <param name="ptr_win">Pointer to the GLFW window that was resized.</param>
    /// <param name="width">The new width of the window in pixels.</param>
    /// <param name="height">The new height of the window in pixels.</param>
    static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);

    /// <summary>
    /// Returns the current width of the window.
    /// </summary>
    /// <returns>The width of the window in pixels.</returns>
    static GLint GetWindowWidth();

    /// <summary>
    /// Returns the current height of the window.
    /// </summary>
    /// <returns>The height of the window in pixels.</returns>
    static GLint GetWindowHeight();

    /// <summary>
    /// Sets the title of the window.
    /// </summary>
    /// <param name="title">The new title of the window.</param>
    static void SetWindowTitle(const char* title);

private:
    /// <summary>
    /// Pointer to the GLFW window managed by this class.
    /// </summary>
    static GLFWwindow* ptrWindow;

    /// <summary>
    /// The width of the window in pixels.
    /// </summary>
    static GLint width;

    /// <summary>
    /// The height of the window in pixels.
    /// </summary>
    static GLint height;

    /// <summary>
    /// The title of the window.
    /// </summary>
    static const char* title;
};
