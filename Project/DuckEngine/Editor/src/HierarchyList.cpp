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


void Hierarchy::ShowHierarchy(int& selectedEntityID)
{
    // Get all entities
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    Texture prefabIconTexture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("prefab_icon");
    Texture gameobjectIconTexture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("gameobject_icon");

    // Get root entities (entities that are not children of any other entity)
    auto rootEntities = GetRootEntities(entities);

    // Make the root level a drop target
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_HIERARCHY"))
        {
            int draggedEntityID = *(const int*)payload->Data;
            auto draggedEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(draggedEntityID);

            if (draggedEntity)
            {
                // Remove the dragged entity from its current parent
                for (auto& e : DuckEngine::DUCKENGINE_EntityManager.GetEntities())
                {
                    auto& children = e->childEntities;
                    children.erase(std::remove(children.begin(), children.end(), draggedEntity), children.end());
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Display root entities and their children recursively
    for (const auto& root : rootEntities)
    {
        DisplayEntity(root, selectedEntityID, prefabIconTexture, gameobjectIconTexture);
    }
}

void Hierarchy::StartRenamingEntity(int entityID) {
	renamingEntityID = entityID;
	Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID).get();
	if (entity) {
		strncpy_s(nameBuffer, entity->name.c_str(), sizeof(nameBuffer) - 1);
		nameBuffer[sizeof(nameBuffer) - 1] = '\0';
	}
}

bool Hierarchy::IsRootEntity(const std::vector<std::shared_ptr<Entity>>& allEntities, const Entity* entity)
{
	for (const auto& e : allEntities)
	{
		for (const auto& child : e->childEntities)
		{
			if (child->entityID == entity->entityID)
			{
				return false;
			}
		}
	}
	return true;
}

std::vector<std::shared_ptr<Entity>> Hierarchy::GetRootEntities(const std::vector<std::shared_ptr<Entity>>& entities)
{
	std::vector<std::shared_ptr<Entity>> roots;
	for (const auto& entity : entities)
	{
		if (IsRootEntity(entities, entity.get()))
		{
			roots.push_back(entity);
		}
	}
	return roots;
}

void Hierarchy::DisplayEntity(std::shared_ptr<Entity> entity,
    int& selectedEntityID,
    const Texture& prefabIcon,
    const Texture& gameobjectIcon)
{
    // If there's no custom name, give the entity a placeholder
    std::string entityLabel = entity->name.empty()
        ? "GameObject " + std::to_string(entity->entityID)
        : entity->name;

    // Basic tree node flags
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    // If no children, mark as leaf so ImGui doesn’t show an arrow
    if (entity->childEntities.empty())
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    // Highlight selection
    if (selectedEntityID == entity->entityID)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;

    // Push a unique ID to keep ImGui calls separate per-entity
    ImGui::PushID(entity->entityID);

    // -- RENAME LOGIC FIRST --
    // If this is the entity we're renaming, skip the tree node and show InputText
    if (entity->entityID == renamingEntityID)
    {
        // Place keyboard focus so the user can type immediately
        ImGui::SetKeyboardFocusHere();
        if (ImGui::InputText("##Rename", nameBuffer, sizeof(nameBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue))
        {
            entity->name = nameBuffer;
            LevelManager::SaveSceneChanges(
                DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
            renamingEntityID = -1;
        }

        // If the input lost focus in any other way, just exit rename mode
        if (ImGui::IsItemDeactivated())
        {
            renamingEntityID = -1;
        }

        ImGui::PopID();
        return;  // Don’t display children, because we’re in rename mode
    }

    // -- OTHERWISE DISPLAY ICON + TREE NODE --
    // Align images with text baseline
    ImGui::AlignTextToFramePadding();

    // Show either the prefab icon or the normal gameobject icon
    if (!entity->prefabName.empty() && prefabIcon)
    {
        ImGui::Image((void*)(intptr_t)prefabIcon, ImVec2(16, 16));
    }
    else
    {
        ImGui::Image((void*)(intptr_t)gameobjectIcon, ImVec2(16, 16));
    }
    ImGui::SameLine();

    // Create the collapsible tree node with the entity's label
    bool nodeOpen = ImGui::TreeNodeEx(entityLabel.c_str(), nodeFlags);

    // If user clicks the tree node, mark this entity as selected
    if (ImGui::IsItemClicked())
        selectedEntityID = entity->entityID;

    // -- DRAG SOURCE: only valid if the current TreeNode is active/held --
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        // Set the payload (the entity ID)
        ImGui::SetDragDropPayload("ENTITY_HIERARCHY", &entity->entityID, sizeof(int));
        // Optional: a quick preview label
        ImGui::Text("Moving %s", entityLabel.c_str());
        ImGui::EndDragDropSource();
    }

    // -- DROP TARGET: if something is dragged over this node, adopt it --
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_HIERARCHY"))
        {
            int draggedEntityID = *(const int*)payload->Data;
            auto draggedEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(draggedEntityID);

            // Avoid dropping an entity onto itself
            if (draggedEntity && draggedEntity->entityID != entity->entityID)
            {
                // First, remove the dragged entity from any previous parent
                for (auto& e : DuckEngine::DUCKENGINE_EntityManager.GetEntities())
                {
                    auto& children = e->childEntities;
                    children.erase(std::remove(children.begin(),
                        children.end(),
                        draggedEntity),
                        children.end());
                    // Also remove the child's name from parent's childNames if you store them
                    auto& childNames = e->childNames;
                    childNames.erase(std::remove(childNames.begin(),
                        childNames.end(),
                        draggedEntity->name),
                        childNames.end());
                }

                // Now adopt it as a child of this entity
                entity->childEntities.push_back(draggedEntity);
                // Optionally add the child’s name so it is saved/loaded properly
                entity->childNames.push_back(draggedEntity->name);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // If the node is open, recursively display children
    if (nodeOpen)
    {
        for (const auto& child : entity->childEntities)
        {
            DisplayEntity(child, selectedEntityID, prefabIcon, gameobjectIcon);
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}
