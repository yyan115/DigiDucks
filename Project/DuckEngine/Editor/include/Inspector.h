/******************************************************************************/
/*!
\file     Inspector.h
\author   Lucas Yee JunJie, l.yee, 2301212 (70%)
          Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (30%)
\par      l.yee@digipen.edu, muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the InspectorRenderer class
          which is responsible for rendering the inspector UI in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <unordered_map>
#include <unordered_set>
#include "ComponentManager.h"
#include "imgui.h"

class InspectorRenderer 
{
public:
    static std::unordered_map<int, bool> entityChanges;

    /****************************************************************
	* @brief Renders the inspector UI, the layer
    *
	* @param entityID The ID of the entity to render
    * @return void
    ***************************************************************/
    static void RenderLayer(int entityID);

    /****************************************************************
	* @brief Renders the inspector UI, the components
    *
    * @param entityID The ID of the entity to render
    * @return void
    ***************************************************************/
    static void RenderComponents(int entityID);

    /****************************************************************
	* @brief Adds the different components to the entity
    *
	* @param entityID The ID of the entity to add components to
	* @param hasChanged A reference to a boolean that will be set to true if the entity has changed
    * @return void
    ***************************************************************/
    static void AddComponents(int entityID, bool& hasChanged);

    /****************************************************************
    * @brief Checks if the file extension is allowed
    *
    * @param filePath The path to the file
    * @param allowedExtensions The set of allowed extensions
    * @return bool
    ***************************************************************/
    static bool IsAllowedExtension(const std::string& filePath, const std::unordered_set<std::string>& allowedExtensions);

    /****************************************************************
	* @brief Gets the list of available component types
    *
	* @return std::vector<std::string>
    ***************************************************************/
    static std::vector<std::string> GetComponentTypes();

    /****************************************************************
    * @brief Gets the list of allowed image extensions
    *
    * @return std::unordered_set<std::string>
    ***************************************************************/
    static std::unordered_set<std::string> GetAllowedImageExtensions();



private:
    // List of available component types for adding
    static const std::vector<std::string> componentTypes;

 
    /****************************************************************
	* @brief Adds a component to the entity
    *
	* @param componentName The name of the component to add
	* @param entityID The ID of the entity to add the component to
    * @return void
    ***************************************************************/
    static void AddComponentToEntity(const std::string& componentName, int entityID);
};
