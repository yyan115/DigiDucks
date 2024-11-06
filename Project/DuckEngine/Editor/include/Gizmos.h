/******************************************************************************/
/*!
\file     Gizmos.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the Gizmos class
		  which is responsible for rendering gizmos in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
