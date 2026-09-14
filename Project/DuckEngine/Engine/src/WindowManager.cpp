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
#include <cstdlib>

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "DuckEngine.h"

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (void)(P)
#endif

namespace
{
	// Visible but confined to the window. GLFW gained this mode in 3.4, which
	// is why the build asks for at least that version.
	int ConfinedCursorMode()
	{
		return GLFW_CURSOR_CAPTURED;
	}

	// GLFW confines the cursor by grabbing the pointer, and the X server
	// refuses a grab on a window that is not on screen yet. Creating the
	// window and moving it between fullscreen and windowed both leave it that
	// way for a short while, and neither GLFW nor the server says when it is
	// over, so one request made at the wrong moment is simply lost and the
	// game runs with a free cursor. Asking again on each of the next few
	// frames costs nothing and removes the race.
	int cursorConfinementRequests = 0;
	const int kCursorConfinementRetries = 15;

	void RequestCursorConfinement()
	{
		cursorConfinementRequests = kCursorConfinementRetries;
	}

	// A cursor made of one fully transparent pixel. GLFW's cursor modes cannot
	// express "confined and invisible" at once: GLFW_CURSOR_HIDDEN hides the
	// pointer but does not confine it, and GLFW_CURSOR_CAPTURED confines it but
	// draws it. Setting a transparent image while the mode stays CAPTURED gives
	// both, and leaves the pointer where it is, which GLFW_CURSOR_DISABLED
	// would not.
	GLFWcursor* blankCursor = nullptr;

	bool cursorHideAllowed = false;
	bool cursorHidden = false;
	double lastPointerMove = 0.0;
	double lastPointerX = 0.0;
	double lastPointerY = 0.0;

	// Long enough that it does not flicker while a player nudges the mouse,
	// short enough that the pointer is gone for the whole of a served order.
	const double kCursorIdleSeconds = 2.0;

	void CreateBlankCursor()
	{
		if (blankCursor) { return; }
		unsigned char transparent[4] = { 0, 0, 0, 0 };
		GLFWimage image{};
		image.width = 1;
		image.height = 1;
		image.pixels = transparent;
		blankCursor = glfwCreateCursor(&image, 0, 0);
	}
}

GLFWwindow* WindowManager::ptrWindow = nullptr;
GLint WindowManager::width;
GLint WindowManager::height;
GLint WindowManager::viewportWidth;
GLint WindowManager::viewportHeight;
const char* WindowManager::title;

bool WindowManager::isFocused = true;
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

#if defined(__linux__) && \
    (GLFW_VERSION_MAJOR > 3 || \
     (GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 4))
    // GLEW initializes desktop OpenGL through GLX. Newer GLFW versions can
    // otherwise select Wayland at runtime, which creates an EGL context that
    // GLEW cannot initialize correctly.
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

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

    if (std::getenv("QUACK_KITCHEN_SMOKE_TEST")) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

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
    glfwSetWindowFocusCallback(ptrWindow, window_focus_callback);

    RequestCursorConfinement();
    CreateBlankCursor();

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

    // Moving the window between a monitor and the desktop drops the pointer
    // grab that confines the cursor, so ask for it again.
    RequestCursorConfinement();
}

void WindowManager::SetVSync(bool enabled) {
    if (ptrWindow) {
        glfwSwapInterval(enabled ? 1 : 0);
    }
}

void WindowManager::MinimizeWindow() {
    glfwIconifyWindow(ptrWindow);  // Minimizes the window
}

//void WindowManager::WindowFocusCallback(GLFWwindow* window, int focused) {
//    if (!focused) {
//        DuckEngine::MinimizeWindow(); // Or pause, mute audio, etc.
//        std::cout << "Window lost focus - minimized.\n";
//    }
//}

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

    // std::cout << "viewport w h: " << viewportWidth << ", " << viewportHeight << "\n";
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
    if (blankCursor) {
        glfwDestroyCursor(blankCursor);
        blankCursor = nullptr;
    }
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

    // Call GraphicsManager to update UI positions based on new window size
    //GraphicsManager::OnWindowResize(_width, _height);
}

/// <summary>
/// Returns the current width of the window in pixels.
/// </summary>
/// <returns>The width of the window in pixels.</returns>
GLint WindowManager::GetWindowWidth()
{
    if (DuckEngine::isEditor) {
        return viewportWidth;
    }
    else {
        return width;
    }
}

/// <summary>
/// Returns the current height of the window in pixels.
/// </summary>
/// <returns>The height of the window in pixels.</returns>
GLint WindowManager::GetWindowHeight()
{
    if (DuckEngine::isEditor) {
        return viewportHeight;
    }
    else {
        return height;
    }
}

GLint WindowManager::GetViewportWidth()
{
    //std::cout << "viewportW: " << viewportWidth << ", normalW: " << width << "\n";
    return viewportWidth;
}

GLint WindowManager::GetViewportHeight()
{
    return viewportHeight;
}

GLint WindowManager::GetEntireWindowWidth() {
    return width;
}

GLint WindowManager::GetEntireWindowHeight() {
    return height;
}

/// <summary>
/// Sets the title of the window to the specified string.
/// </summary>
/// <param name="_title">The new title of the window.</param>
void WindowManager::MaintainCursorConfinement() {
    if (cursorConfinementRequests <= 0 || !ptrWindow || DuckEngine::isEditor) {
        return;
    }
    --cursorConfinementRequests;

    // Only while the game has focus. Asking for a confined cursor on a window
    // the player has alt-tabbed away from would take the pointer back off them.
    if (glfwGetWindowAttrib(ptrWindow, GLFW_FOCUSED) != GLFW_TRUE) {
        return;
    }

    // GLFW returns early when the mode it is given is the mode it already
    // holds, and after a lost grab the mode it holds is already the confined
    // one. Passing through GLFW_CURSOR_NORMAL makes the request take effect.
    glfwSetInputMode(ptrWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(ptrWindow, GLFW_CURSOR, ConfinedCursorMode());
}

void WindowManager::AllowCursorHiding() {
    cursorHideAllowed = true;
}

void WindowManager::MaintainCursorVisibility() {
    // Read and clear, so the answer is only ever about this frame. Whatever is
    // playing has to ask again next frame or the cursor comes back, which is
    // what makes leaving a level or opening a panel need no code of its own.
    const bool allowed = cursorHideAllowed;
    cursorHideAllowed = false;

    if (!ptrWindow || DuckEngine::isEditor) {
        return;
    }

    double x = 0.0;
    double y = 0.0;
    glfwGetCursorPos(ptrWindow, &x, &y);
    const double now = glfwGetTime();
    if (x != lastPointerX || y != lastPointerY) {
        lastPointerX = x;
        lastPointerY = y;
        lastPointerMove = now;
    }

    const bool hide = allowed && blankCursor != nullptr &&
                      (now - lastPointerMove) >= kCursorIdleSeconds;
    if (hide == cursorHidden) {
        return;
    }
    cursorHidden = hide;
    glfwSetCursor(ptrWindow, hide ? blankCursor : nullptr);
}

void WindowManager::SetWindowTitle(const char* _title) {
    glfwSetWindowTitle(ptrWindow, _title);
}

void WindowManager::window_focus_callback(GLFWwindow* window, int focused) {

	// Confine the pointer while the game has focus, release it when it does
	// not, so alt-tabbing away hands the cursor back to the desktop. The
	// editor is left alone, it needs a free cursor across its panels.
	if (!DuckEngine::isEditor)
	{
		glfwSetInputMode(
			window,
			GLFW_CURSOR,
			focused ? ConfinedCursorMode() : GLFW_CURSOR_NORMAL);
	}

    if (!focused && !isFullscreen && !DuckEngine::isEditor)  glfwIconifyWindow(ptrWindow);  // Minimizes the window

    UNREFERENCED_PARAMETER(window);
    isFocused = focused != 0;
}
