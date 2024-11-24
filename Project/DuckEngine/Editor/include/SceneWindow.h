/******************************************************************************
\file       SceneWindow.h
\author     Lucas Yee 2301212 (50%)
\par        l.yee@digipen.edu
\author     Muhammad Zikry Bin Zakaria , 2201751 (50%)
\par        muhammadzikry.b@digipen.edu
\date       November 6, 2024
\brief      Declaration of the SceneWindow class, which provides functionalities
            for managing the scene viewport, handling input for scene interaction,
            and converting coordinates between screen, FBO, and world space.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "imgui.h"

typedef unsigned int GLuint;

class SceneWindow
{
public:

    /**************************************************************************
    @brief Initializes the scene window by setting the FBO dimensions.
    **************************************************************************/
    static void Initialize();

    /**************************************************************************
    @brief Renders the scene window, updating the FBO and handling interactions.
    @param newWidth The new width of the window.
    @param newHeight The new height of the window.
    **************************************************************************/
    static void RenderSceneWindow(int newWidth, int newHeight);

    /**************************************************************************
    @brief Checks if the mouse cursor is within the FBO region.
    @return True if the mouse is within the FBO, false otherwise.
    **************************************************************************/
    static bool IsMouseInFBO();

    /**************************************************************************
    @brief Converts the current screen coordinates to FBO coordinates.
    @return The converted FBO coordinates as a Vector2D.
    **************************************************************************/
    static Vector2D ConvertScreenToFBO();

    /**************************************************************************
    @brief Converts screen coordinates to world coordinates.
    @return The converted world coordinates as a Vector2D.
    **************************************************************************/
    static Vector2D ConvertScreenToWorld();

    /**************************************************************************
    @brief Handles dragging of selected entities within the scene.
    **************************************************************************/
    static void HandleEntityDragging();

    /**************************************************************************
    @brief Gets a list of entities located at a specific world position.
    @param worldPos The world coordinates to check for entities.
    @return A vector of Entity pointers at the given position.
    **************************************************************************/
    static std::vector<Entity*> GetEntitiesAtPosition(const Vector2D& worldPos);

    /**************************************************************************
    @brief Converts world coordinates to screen coordinates.
    @param worldPos The world coordinates to convert.
    @return The converted screen coordinates as a Vector2D.
    **************************************************************************/
    static Vector2D ConvertWorldToScreen(const Vector2D& worldPos);

    /**************************************************************************
    @brief Gets the current scaling factor between world and FBO space.
    @return The world scale as a Vector2D.
    **************************************************************************/
    static Vector2D GetWorldScale();

    /**************************************************************************
    @brief Checks if the scene is in play mode.
    @return True if the scene is playing, false otherwise.
    **************************************************************************/
    static bool GetIsPlaying() { return isPlaying; }

    /**************************************************************************
    @brief Checks if the mouse is currently within the scene’s FBO area.
    @return True if the mouse is in the FBO, false otherwise.
    **************************************************************************/
    static bool GetInSceneFBO() { return inSceneFBO; }

    /**************************************************************************
    @brief Handles dragging a prefab into the scene from an external source.
    @param prefabName The name of the prefab being dragged.
    @param position The position at which the prefab is dropped in the scene.
    **************************************************************************/
    static void OnPrefabDraggedIntoScene(const std::string& prefabName, Vec2 position);

private:
    static bool isPlaying;
    static bool isPaused;
    static int width;
    static int height;
    static bool inSceneFBO;
    static Entity* selectedEntity;
    static Vector2D initialMousePos;
    static Vector2D initialEntityPos;
    static std::vector<Entity*> entitiesUnderMouse;
    static int currentEntityIndex;
    static Vector2D lastMousePos;
};