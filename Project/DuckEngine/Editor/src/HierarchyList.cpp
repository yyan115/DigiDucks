#include "HierarchyList.h"
#include "DuckEngine.h"
#include "imgui.h"
#include <unordered_map>

int renamingEntityID = -1;
char nameBuffer[128] = {};

void Hierarchy::ShowHierarchy(int& selectedEntityID) {
    // Get all entities
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    // Track prefab instance counts
    std::unordered_map<std::string, int> prefabInstanceCounts;

    for (auto& entity : entities) {
        std::string entityLabel;

        if (!entity.prefabName.empty()) {
            // If it's a prefab and has a custom name, use the custom name
            if (!entity.name.empty()) {
                entityLabel = entity.name;
            }
            else {
                // Generate a label based on the prefab name and instance count
                int& count = prefabInstanceCounts[entity.prefabName];
                entityLabel = entity.prefabName;
                if (count > 0) {
                    entityLabel += " (" + std::to_string(count) + ")";
                }
                count++;
            }
        }
        else {
            // If not a prefab, use the entity's existing name or a default label
            entityLabel = entity.name.empty() ? "GameObject " + std::to_string(entity.entityID) : entity.name;
        }

        // If the entity isn't being renamed, apply the label to its name
        if (entity.entityID != renamingEntityID) {
            entity.name = entityLabel;
        }

        // Begin a tree node for each entity
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (selectedEntityID == entity.entityID) {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool nodeOpen;

        if (entity.entityID == renamingEntityID) {
            // Display a renaming input field for the selected entity
            ImGui::SetKeyboardFocusHere();
            if (ImGui::InputText("##Rename", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                entity.name = nameBuffer;  
                LevelManager::SaveSceneChanges(DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName());  // Save the scene changes
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
            ImGui::TreePop();
        }
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
