/******************************************************************************/
/*!
\file     HierarchyList.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the Hierarchy class
          which is responsible for displaying the hierarchy list UI in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "EntityManager.h"

class Hierarchy {
public:
    /****************************************************************
	* @brief Displays the hierarchy list UI
    *
	* @param SelectedEntityID The ID of the currently selected entity
    * @return void
    ***************************************************************/
    static void ShowHierarchy(int& SelectedEntityID);

    /****************************************************************
	* @brief Starts renaming the currently selected entity
    *
	* @param entityID The ID of the currently selected entity
    * @return void
    ***************************************************************/
    static void StartRenamingEntity(int entityID);



    static void DisplayEntity(std::shared_ptr<Entity> entity, int& selectedEntityID, const Texture& prefabIcon, const Texture& gameobjectIcon);

    static bool IsRootEntity(const std::vector<std::shared_ptr<Entity>>& allEntities, const Entity* entity);
    static std::vector<std::shared_ptr<Entity>> GetRootEntities(const std::vector<std::shared_ptr<Entity>>& entities);

};
