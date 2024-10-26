/******************************************************************************/
/*!
\file       DuckEngine.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the DuckEngine class, which serves as the central hub for
            managing various systems in the game engine. It provides methods
            to initialize, update, render, and manage game components,
            including input, scene management, and drawing functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "EntityFactory.h"
#include "PrefabManager.h"
#include "Serialization.h"
#include "LevelManager.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The DuckEngine class provides the main interface for managing all
       game engine components and systems. It offers static methods for
       handling entity management, rendering, input, and more.
*************************************************************************/
class  DuckEngine
{
public:
    static DUCKENGINE_API ComponentManager DUCKENGINE_ComponentManager;
    static DUCKENGINE_API EntityManager DUCKENGINE_EntityManager;
    static DUCKENGINE_API SystemManager DUCKENGINE_SystemManager;
    static DUCKENGINE_API SceneManager DUCKENGINE_SceneManager;
    static DUCKENGINE_API AssetManager DUCKENGINE_AssetManager;
    static DUCKENGINE_API EntityFactory DUCKENGINE_EntityFactory;
    static DUCKENGINE_API PrefabManager DUCKENGINE_PrefabManager;
    static DUCKENGINE_API LevelManager DUCKENGINE_LevelManager;

    /************************************************************************
    @brief Initializes the DuckEngine, setting up necessary systems and
           resources for the game to run.
    *************************************************************************/
    void DUCKENGINE_API Initialize(bool _isEditor = false);

    static void DUCKENGINE_API SetPlaying(bool playing);
    static bool DUCKENGINE_API IsPlaying();

    /************************************************************************
    @brief Updates the game engine by processing input, updating systems,
           and handling game logic for each frame.
    *************************************************************************/
    void DUCKENGINE_API Update();

    /************************************************************************
    @brief Starts the rendering process for a new frame.
    *************************************************************************/
    void DUCKENGINE_API StartDraw();

    /************************************************************************
    @brief Executes the drawing commands to render the current game frame.
    *************************************************************************/
    void DUCKENGINE_API Draw();

    void DUCKENGINE_API StartImguiRender();
    void DUCKENGINE_API EndImguiRender();

    /************************************************************************
    @brief Ends the rendering process for the current frame and displays the result.
    *************************************************************************/
    void DUCKENGINE_API EndDraw();

    /************************************************************************
    @brief Exits the game engine and performs any necessary cleanup.
    *************************************************************************/
    void DUCKENGINE_API Exit();

    /************************************************************************
    @brief Checks if the game engine is still running.
    @return True if the engine is running, otherwise false.
    *************************************************************************/
    bool DUCKENGINE_API Running();

    static DUCKENGINE_API bool WKeyDown;
    static DUCKENGINE_API bool AKeyDown;
    static DUCKENGINE_API bool SKeyDown;
    static DUCKENGINE_API bool DKeyDown;
    static DUCKENGINE_API bool ToggleEditor;

    /************************************************************************
    @brief Retrieves the width of the game window.
    @return The width of the game window.
    *************************************************************************/
    static DUCKENGINE_API int GetWindowWidth();

    /************************************************************************
    @brief Retrieves the height of the game window.
    @return The height of the game window.
    *************************************************************************/
    static DUCKENGINE_API int GetWindowHeight();

    /************************************************************************
    @brief Sets the window title.
    @param title The desired title for the window.
    *************************************************************************/
    static DUCKENGINE_API void SetWindowTitle(std::string title);

    /************************************************************************
    @brief Retrieves the delta time between the current and previous frame.
    @return The time in seconds between frames.
    *************************************************************************/
    static DUCKENGINE_API float DeltaTime();

    /************************************************************************
    @brief Retrieves the current frames per second (FPS).
    @return The current FPS.
    *************************************************************************/
    static DUCKENGINE_API float FPS();

    /************************************************************************
    @brief Sets the camera position within the game world.
    @param x The x-coordinate of the camera position.
    @param y The y-coordinate of the camera position.
    *************************************************************************/
    static DUCKENGINE_API void SetCameraPosition(const float x, const float y);

    /************************************************************************
    @brief Sets the height of the camera, adjusting its view.
    @param height The desired camera height.
    *************************************************************************/
    static DUCKENGINE_API void SetCameraHeight(const int height);

    /************************************************************************
    @brief Sets the background color of the game.
    @param r The red component of the color.
    @param g The green component of the color.
    @param b The blue component of the color.
    @param a The alpha (transparency) component of the color.
    *************************************************************************/
    static DUCKENGINE_API void SetBackgroundColor(const float r, const float g, const float b, const float a);

    /************************************************************************
    @brief Renders text to the screen at a specified position with scaling and color.
    @param text The text to be rendered.
    @param position The position where the text will appear on screen.
    @param scale The scale factor for the text.
    @param color The color of the text.
    *************************************************************************/
    static DUCKENGINE_API void RenderText(const std::string& text, const Vector2D& position, float scale, const Color& color);

    /************************************************************************
    @brief Draws a point on the screen at a specified position with a given size.
    @param position The position of the point.
    @param size The size of the point.
    @param color The color of the point (default is red).
    @param relativeToCamera Determines if the point should be drawn relative to the camera.
    *************************************************************************/
    static DUCKENGINE_API void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);

    /************************************************************************
    @brief Draws a line between two points on the screen with a specified size and color.
    @param start The starting position of the line.
    @param end The ending position of the line.
    @param size The thickness of the line.
    @param color The color of the line (default is red).
    @param relativeToCamera Determines if the line should be drawn relative to the camera.
    *************************************************************************/
    static DUCKENGINE_API void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);

    /************************************************************************
    @brief Draws a circle at a specified position with a given radius and color.
    @param position The position of the circle's center.
    @param radius The radius of the circle.
    @param color The color of the circle (default is red).
    @param relativeToCamera Determines if the circle should be drawn relative to the camera.
    *************************************************************************/
    static DUCKENGINE_API void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);

    /************************************************************************
    @brief Draws a rectangle on the screen using the minimum and maximum corners, with optional rotation.
    @param minCorner The position of the bottom-left corner.
    @param maxCorner The position of the top-right corner.
    @param rotation The rotation of the rectangle (default is 0).
    @param color The color of the rectangle (default is red).
    @param relativeToCamera Determines if the rectangle should be drawn relative to the camera.
    *************************************************************************/
    static void DUCKENGINE_API DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, float rotation = 0.f, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);

    /************************************************************************
    @brief Enables or disables logging in the engine.
    @param enable True to enable logging, false to disable it.
    *************************************************************************/
    static void DUCKENGINE_API EnableLogging(bool enable);

};
