/******************************************************************************/
/*!
\file    UIManager.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the UIManager class

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

class UIManager {
public:
    /****************************************************************
    * @brief Initializes the ImGui context and platform/renderer bindings
    *
    * @return void
    ***************************************************************/
    static void Initialize();

    /****************************************************************
    * @brief Renders the ImGui elements including main menu and various windows
    *
    * @return void
    ***************************************************************/
    static void StartRender();
    static void Render();
    static void EndRender();

    /****************************************************************
    * @brief Cleans up ImGui resources and destroys the context
    *
    * @return void
    ***************************************************************/
    static void Exit();

    
private:
    /****************************************************************
    * @brief Displays a window containing debug information and system stats
    *
    * @return void
    ***************************************************************/
    static void ShowDebugInfo();

    /****************************************************************
    * @brief Displays the inspector window to inspect and modify entity properties
    *
    * @return void
    ***************************************************************/
    static void ShowInspector();

    /****************************************************************
    * @brief Displays the explorer window in ImGui for managing assets
    *
    * @return void
    ***************************************************************/
    static void ShowExplorer();

    /****************************************************************
    * @brief Displays a window to control entity spawning and removal
    *
    * @return void
    ***************************************************************/
    static void ShowHierarchy();

    /****************************************************************
    * @brief Renders visible windows based on their state
    *
    * @return void
    ***************************************************************/
    static void RenderWindows();

    /****************************************************************
    * @brief Renders ImGui windows based on size and position offsets
    *
    * @param WidthOffset - The width scaling factor for the window
    * @param HeightOffset - The height scaling factor for the window
    * @param PosX - X position offset for the window
    * @param PosY - Y position offset for the window
    *
    * @return void
    ***************************************************************/
    static void RenderImGuiWindows(float WidthOffset, float HeightOffset, float PosX = 0, float PosY = 0);

    /****************************************************************
    * @brief Displays a window showing performance
    *
    * @return void
    ***************************************************************/
    static void ShowPerformance();

    /****************************************************************
    * @brief Renders buttons to switch between scenes in the engine
    *
    * @return void
    ***************************************************************/
    static void RenderSceneAssets();

    /****************************************************************
    * @brief Renders buttons for spawning or interacting with game objects
    *
    * @return void
    ***************************************************************/
    static void RenderGameObjectAssets();

    /****************************************************************
    * @brief Renders buttons for controlling audio assets
    *
    * @return void
    ***************************************************************/
    static void RenderAudioAssets();

    static void RenderConsole();

    static void CreateDockSpace();

    static void ShowMenuBar();
};




