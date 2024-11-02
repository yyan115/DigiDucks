#ifndef GIZMOS_H
#define GIZMOS_H

#include "Vector2.h"
#include "Entity.h"
#include "imgui.h"

class Gizmos {
public:
    static void RenderGizmoForSelectedEntity(Entity* selectedEntity);  // Render gizmo for the selected entity

private:
    static bool DrawMoveHandle(const Vector2D& screenPos, const Vector2D& direction, const ImVec4& color);
};

#endif // GIZMOS_H
