//---------------------------------------------------------
// File:    UIManager.h
//authors:	Muhammad Zikry Bin Zakaria
// email:	muhammadzikry.b\@digipen.edu
// 
//
// Brief:     Contains the declaration that is used to manage the UI in the editor
//
// Copyright © 2024 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once

class UIManager {
public:
    // Initialize the UI
    static void Initialize();

    // Update the UI
    static void Render();

    // Exit the UI
    static void Exit();

    
private:
    // Show the main menu
    static void ShowDebugInfo();

    // Show the inspector window
    static void ShowInspector();

    // Show the console window
    static void ShowExplorer();

    // Show the entity spawn window
    static void ShowEntitySpawn();

    // Show the different window ( for debug info )
    static void RenderWindows();

    // Render the ImGui adaptive windows
    static void RenderImGuiWindows(float WidthOffset, float HeightOffset, float PosX = 0, float PosY = 0);

    // Show the performance window
    static void ShowPerformance();

    static void RenderSceneAssets();
    static void RenderGameObjectAssets();
    static void RenderTextureAssets();
};




