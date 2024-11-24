/******************************************************************************/
/*!
\file     AssetsBrowser.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the AssetsBrowser class
		  which is responsible for displaying the assets explorer UI in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <string>
#include "GraphicsManager.h"

class AssetsBrowser {
public:
    /****************************************************************
	* @brief Displays the assets explorer UI
    *
    * @return void
    ***************************************************************/
    static void ShowAssets();
    static void HandleFileDrop(GLFWwindow* window, int count, const char** paths);
private:
    /****************************************************************
	* @brief Renders the directory tree in the left pane
    *
    * @return void
    ***************************************************************/
    static void RenderDirectoryTree();

    /****************************************************************
	* @brief Renders the asset grid in the right pane
    *
	* @param path The path to the folder containing the assets
    * @return void
    ***************************************************************/
    static void RenderAssetGrid(const std::string& path);

    /****************************************************************
	* @brief Renders the prefab grid in the right pane
    *
    * @return void
    ***************************************************************/
    static void RenderPrefabsGrid();

    /****************************************************************
	* @brief Replaces a texture asset with a new one
    *
    * @return void
    ***************************************************************/
    static void ReplaceAsset(const std::string& oldPath, const std::string& newPath);

    
    static std::string selectedFolderPath;
    static std::string selectedFolderName;
};

