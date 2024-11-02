#pragma once

#include "DuckEngine.h"
#include "imgui.h"

typedef unsigned int GLuint;

class SceneWindow
{
public:
    static void Initialize();
    static void RenderSceneWindow(int newWidth, int newHeight);
    static bool IsMouseInFBO();
    static Vector2D ConvertScreenToFBO();
    static Vector2D ConvertScreenToWorld();
    static void HandleEntityDragging();
    static std::vector<Entity*> GetEntitiesAtPosition(const Vector2D& worldPos);
    static Vector2D ConvertWorldToScreen(const Vector2D& worldPos);
    static Vector2D GetWorldScale();

    static bool GetIsPlaying() { return isPlaying; }
    static bool GetInSceneFBO() { return inSceneFBO; }

    static void OnPrefabDraggedIntoScene(const std::string& prefabName, Vec2 position);

private:
    static bool isPlaying;
    static int width;
    static int height;
    static bool inSceneFBO;
    static Entity* selectedEntity;
    static Vector2D initialMousePos;
    static Vector2D initialEntityPos;
    static std::vector<Entity*> entitiesUnderMouse;
    static int currentEntityIndex;
    static Vector2D lastMousePos;
};