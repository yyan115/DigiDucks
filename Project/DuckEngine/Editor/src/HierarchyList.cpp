/******************************************************************************/
/*!
\file     HierarchyList.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the definition of the Hierarchy class
		  which is responsible for displaying the hierarchy list UI in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "HierarchyList.h"
#include "DuckEngine.h"
#include "imgui.h"

#include <unordered_map>

int renamingEntityID = -1;
char nameBuffer[128] = {};

void Hierarchy::ShowHierarchy(int& selectedEntityID) {
    // Get all entities
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    for (auto& entity : entities) {
        std::string entityLabel;

		// Check if the entity is a prefab
        if (!entity.prefabName.empty()) {
			// If the entity is a prefab, use the prefab name
            if (!entity.name.empty()) {
                entityLabel = entity.name;
				
            }
            else {
				// Generate a label for prefab entities
                entityLabel = entity.prefabName;
                entityLabel += " (" + std::to_string(entity.entityID) + ")";
            }
        }
        else {
			// If not a prefab, use the entity name or a default label
            entityLabel = entity.name.empty() ? "GameObject " + std::to_string(entity.entityID) : entity.name;
        }

		// If the entity is not being renamed, update the entity name
        if (entity.entityID != renamingEntityID) {
            entity.name = entityLabel;
        }

		// Set node flags for tree node
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (selectedEntityID == entity.entityID) {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool nodeOpen;

        // Display prefab icon
        ImGui::PushID(entity.entityID);
        auto prefabIconTexture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("prefab_icon");
        if (!entity.prefabName.empty() && prefabIconTexture) {
            ImGui::Image((void*)(intptr_t)prefabIconTexture, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0)); // Render prefab icon
            ImGui::SameLine();
        }
        else {
            auto gameobjectIconTexture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("gameobject_icon");
            ImGui::Image((void*)(intptr_t)gameobjectIconTexture, ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0)); // Render prefab icon
            ImGui::SameLine();
        }

        // Display a renaming input field for the selected entity
        if (entity.entityID == renamingEntityID) {
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##Rename", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                entity.name = nameBuffer;  
				// Save scene changes
                LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
                renamingEntityID = -1;  
            }
            // Exit if focus lost
            if (ImGui::IsItemDeactivated()) {
                renamingEntityID = -1; 
            }
            nodeOpen = false;
        }
        else {
            nodeOpen = ImGui::TreeNodeEx(entityLabel.c_str(), nodeFlags);
            // Handle selection on click
            if (ImGui::IsItemClicked()) {
                selectedEntityID = (selectedEntityID == entity.entityID) ? -1 : entity.entityID;
            }
        }

        // Show child nodes if expanded
        if (nodeOpen) {
            ImGui::Text("Entity ID: %d", entity.entityID);
            ImGui::Text("Layer name: %s", entity.layerName.c_str());
            ImGui::Text("Prefab name: %s", entity.prefabName.c_str());
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

void Hierarchy::StartRenamingEntity(int entityID) {
    renamingEntityID = entityID;
    Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
    if (entity) {
        strncpy_s(nameBuffer, entity->name.c_str(), sizeof(nameBuffer) - 1);
        nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    }
}
