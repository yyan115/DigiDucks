/******************************************************************************/
/*!
\file       PrefabEditor.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Declares the PrefabEditor class, which provides a graphical
            interface for editing prefab objects in the game engine. This
            includes functionality for rendering prefab properties, managing
            components, and previewing prefabs.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <string>
#include <memory>
#include "PrefabManager.h"
#include "imgui.h"

class PrefabEditor
{
public:
    /**************************************************************************
    * @brief Opens the Prefab Editor for the specified prefab.
    * @param prefabName The name of the prefab to edit.
    **************************************************************************/
    static void OpenPrefabEditor(const std::string& prefabName);

    /**************************************************************************
    * @brief Renders the Prefab Editor window and its components.
    **************************************************************************/
    static void Render();

private:
    static bool isOpen;
    static std::string currentPrefabName;
    static std::shared_ptr<Prefab> currentPrefab;

    /**************************************************************************
    * @brief Renders the properties of the selected prefab, including its components.
    **************************************************************************/
    static void RenderPrefabProperties();

    /**************************************************************************
    * @brief Renders a preview of the selected prefab.
    **************************************************************************/
    static void RenderPrefabPreview();

    /**************************************************************************
    * @brief Adds a new component to the selected prefab.
    **************************************************************************/
    static void AddComponent();

    /**************************************************************************
    * @brief Displays a context menu for a specific component in the prefab.
    * @param componentIndex The index of the component in the prefab's data.
    **************************************************************************/
    static void ComponentMenu(int componentIndex);

};
