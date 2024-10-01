#pragma once

#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "EntityFactory.h"
#include "PrefabManager.h"
#include "Serialization.h"
#include "LevelManager.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API DuckEngine
{
public:
    static ComponentManager DUCKENGINE_ComponentManager;
    static EntityManager DUCKENGINE_EntityManager;
    static SystemManager DUCKENGINE_SystemManager;
    static SceneManager DUCKENGINE_SceneManager;
    static AssetManager DUCKENGINE_AssetManager;
    static EntityFactory DUCKENGINE_EntityFactory;
    static PrefabManager DUCKENGINE_PrefabManager;

    void Initialize();
    void Update();
    void StartDraw();
    void Draw();
    void EndDraw();
    void Exit();

    bool Running();

    static bool WKeyDown;
    static bool AKeyDown;
    static bool SKeyDown;
    static bool DKeyDown;

    static int GetWindowWidth();
    static int GetWindowHeight();
    static void SetWindowTitle(std::string title);
    static float DeltaTime();
    static float FPS();

    static void SetCameraPosition(const float x, const float y);
    static void SetCameraHeight(const int height);

    static void SetBackgroundColor(const float r, const float g, const float b, const float a);

    static void RenderText(const std::string& text, const Vector2D& position, float scale, const Color& color);

    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);
    //static void DrawRectangle(const Vector2D& position, const Vector2D& size, float rotation, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);

    static void DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, float rotation = 0.f, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);
};
