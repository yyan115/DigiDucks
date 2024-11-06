/******************************************************************************/
/*!
\file     Gizmos.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implmentation of the Gizmos class
          which is responsible for rendering gizmos in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Gizmos.h"
#include "SceneWindow.h"
#include "DuckEngine.h"
#include "TransformComponent.h"
#include <iostream>

void Gizmos::RenderGizmoForSelectedEntity(Entity* selectedEntity)
{
    if (!selectedEntity) return;

    auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(selectedEntity->entityID);
    if (!transform) return;

    Vector2D entityScreenPos = SceneWindow::ConvertWorldToScreen(transform->position);

    if (DrawMoveHandle(entityScreenPos, { 1.0f, 0.0f }, ImVec4(1, 0, 0, 1))) // X Axis handle
    {
        transform->position.x += ImGui::GetMouseDragDelta().x * SceneWindow::GetWorldScale().x;
        ImGui::ResetMouseDragDelta();
    }

    if (DrawMoveHandle(entityScreenPos, { 0.0f, 1.0f }, ImVec4(0, 1, 0, 1))) // Y Axis handle
    {
        transform->position.y -= ImGui::GetMouseDragDelta().y * SceneWindow::GetWorldScale().y;
        ImGui::ResetMouseDragDelta();
    }
}

bool Gizmos::DrawMoveHandle(const Vector2D& screenPos, const Vector2D& direction, const ImVec4& color)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 start(screenPos.x, screenPos.y);
    ImVec2 end(start.x + direction.x * 50, start.y + direction.y * 50); // Arrow length

    // Draw arrow line
    drawList->AddLine(start, end, ImGui::ColorConvertFloat4ToU32(color), 2.0f);

    // Draw arrowhead triangle
    ImVec2 arrowHead1(end.x + -direction.y * 5, end.y + direction.x * 5);
    ImVec2 arrowHead2(end.x + direction.y * 5, end.y - direction.x * 5);
    drawList->AddTriangleFilled(end, arrowHead1, arrowHead2, ImGui::ColorConvertFloat4ToU32(color));

    // Check for drag interaction
    ImGui::SetCursorScreenPos(end);
    ImGui::InvisibleButton("##dragHandle", ImVec2(10, 10));
    return ImGui::IsItemActive();
}
