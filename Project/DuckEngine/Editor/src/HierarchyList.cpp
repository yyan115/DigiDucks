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

void Hierarchy::DisplayEntity(std::shared_ptr<Entity> entity, int& selectedEntityID, const Texture& prefabIcon, const Texture& gameobjectIcon)
{
    if (!entity)
    {
        printf("Error: Null entity pointer in DisplayEntity\n");
        return;
    }

    if (entity->entityID < 0)
    {
        printf("Error: Invalid entity ID: %d\n", entity->entityID);
        return;
    }

    std::string entityLabel = entity->name.empty()
        ? "GameObject " + std::to_string(entity->entityID)
        : entity->name;

    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (entity->childEntities.empty())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    if (selectedEntityID == entity->entityID)
    {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    ImGui::PushID(entity->entityID);

    // Display the icon
    if (!entity->prefabName.empty() && prefabIcon)
    {
        ImGui::Image((void*)(intptr_t)prefabIcon, ImVec2(16, 16));
    }
    else
    {
        ImGui::Image((void*)(intptr_t)gameobjectIcon, ImVec2(16, 16));
    }
    ImGui::SameLine();

    // Drag source for reordering and parenting
    if (ImGui::IsItemHovered() && ImGui::IsItemVisible())
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("ENTITY_HIERARCHY", &entity->entityID, sizeof(int));
            ImGui::Text("Moving %s", entityLabel.c_str());
            ImGui::EndDragDropSource();
        }
    }

    // Drag-and-drop target to accept children
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_HIERARCHY"))
        {
            int draggedEntityID = *(const int*)payload->Data;
            auto draggedEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(draggedEntityID);

            if (draggedEntity && draggedEntity->entityID != entity->entityID)
            {
                // Remove dragged entity from its current parent
                for (auto& e : DuckEngine::DUCKENGINE_EntityManager.GetEntities())
                {
                    auto& children = e->childEntities;
                    children.erase(std::remove(children.begin(), children.end(), draggedEntity), children.end());
                }

                // Add dragged entity as a child of this entity
                entity->childEntities.push_back(draggedEntity);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Handle renaming of the entity
    if (entity->entityID == renamingEntityID)
    {
        ImGui::SetKeyboardFocusHere();
        if (ImGui::InputText("##Rename", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            entity->name = nameBuffer;
            LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());
            renamingEntityID = -1;
        }
        if (ImGui::IsItemDeactivated())
        {
            renamingEntityID = -1;
        }
    }
    else
    {
        bool nodeOpen = ImGui::TreeNodeEx(entityLabel.c_str(), nodeFlags);
        if (ImGui::IsItemClicked())
        {
            selectedEntityID = entity->entityID;
        }

        if (nodeOpen)
        {
            for (const auto& child : entity->childEntities)
            {
                DisplayEntity(child, selectedEntityID, prefabIcon, gameobjectIcon);
            }
            ImGui::TreePop();
        }
    }

    ImGui::PopID();
}
