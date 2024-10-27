#include "HierarchyList.h"
#include "DuckEngine.h"
#include "imgui.h"

// Define the static selectedEntityID
int Hierarchy::selectedEntityID = -1;

void Hierarchy::ShowHierarchy() {
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Get all entities
    std::vector<Entity> entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

    // Iterate through entities and create tree nodes for each one
    for (Entity& entity : entities) {
        if (entity.entityID == 0) continue;

        // Generate a unique label for the entity
        std::string entityLabel = entity.name.empty() ? "GameObject " + std::to_string(entity.entityID) : entity.name;

        // Check if this entity is selected and apply color styling if needed
        if (selectedEntityID == entity.entityID) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Highlighted text color
        }

        // Create a TreeNode for each entity
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx(entityLabel.c_str(), flags);

        //// Handle selecting entity on click
        //if (ImGui::IsItemClicked()) {
        //    selectedEntityID = entity.entityID;
        //}

        //// Drag-and-drop source: Start dragging this entity
        //if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        //    ImGui::SetDragDropPayload("ENTITY_PAYLOAD", &entity.entityID, sizeof(int));
        //    ImGui::Text("Move %s", entityLabel.c_str());
        //    ImGui::EndDragDropSource();
        //}

        //// Drag-and-drop target: Accept dragged entities to make them children
        //if (ImGui::BeginDragDropTarget()) {
        //    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_PAYLOAD")) {
        //        int payloadEntityID = *(const int*)payload->Data;

        //        // Set entity with `payloadEntityID` as a child of the current `entity`
        //        DuckEngine::DUCKENGINE_EntityManager.SetParent(payloadEntityID, entity.entityID);
        //    }
        //    ImGui::EndDragDropTarget();
        //}

        //if (opened) {
        //    // Recursively display children (if any)
        //    //ShowChildHierarchy(entity.entityID);
        //    ImGui::TreePop();
        //}

        //if (selectedEntityID == entity.entityID) {
        //    //ImGui::PopStyleColor();
        //}
    }

    ImGui::End();
}

// Recursive function to show children of an entity
void Hierarchy::ShowChildHierarchy(int parentID) {
    //const auto& children = DuckEngine::DUCKENGINE_EntityManager.GetChildren(parentID);

    //for (const auto& child : children) {
    //    std::string childLabel = child->name.empty() ? "GameObject " + std::to_string(child->entityID) : child->name;

    //    bool childOpened = ImGui::TreeNodeEx(childLabel.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);

    //    if (ImGui::IsItemClicked()) {
    //        selectedEntityID = child->entityID;
    //    }

    //    // Drag-and-drop source for child
    //    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
    //        ImGui::SetDragDropPayload("ENTITY_PAYLOAD", &child->entityID, sizeof(int));
    //        ImGui::Text("Move %s", childLabel.c_str());
    //        ImGui::EndDragDropSource();
    //    }

    //    // Accept other entities as children of this child
    //    if (ImGui::BeginDragDropTarget()) {
    //        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_PAYLOAD")) {
    //            int payloadEntityID = *(const int*)payload->Data;

    //            // Set entity with `payloadEntityID` as a child of the current child entity
    //            DuckEngine::DUCKENGINE_EntityManager.SetParent(payloadEntityID, child->entityID);
    //        }
    //        ImGui::EndDragDropTarget();
    //    }

    //    if (childOpened) {
    //        ShowChildHierarchy(child->entityID); // Recurse for grandchildren
    //        ImGui::TreePop();
    //    }
    //}
}
