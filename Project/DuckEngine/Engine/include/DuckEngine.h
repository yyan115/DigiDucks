#pragma once
#define WIN32_LEAN_AND_MEAN
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

class  DuckEngine
{
public:
    static DUCKENGINE_API ComponentManager DUCKENGINE_ComponentManager;
    static DUCKENGINE_API EntityManager DUCKENGINE_EntityManager;
    static DUCKENGINE_API SystemManager DUCKENGINE_SystemManager;
    static DUCKENGINE_API SceneManager DUCKENGINE_SceneManager;
    static DUCKENGINE_API AssetManager DUCKENGINE_AssetManager;
    static DUCKENGINE_API EntityFactory DUCKENGINE_EntityFactory;
    static DUCKENGINE_API PrefabManager DUCKENGINE_PrefabManager;

    void DUCKENGINE_API Initialize();
    void DUCKENGINE_API Update();
    void DUCKENGINE_API StartDraw();
    void DUCKENGINE_API Draw();
    void DUCKENGINE_API EndDraw();
    void DUCKENGINE_API Exit();

    bool DUCKENGINE_API Running();

    static DUCKENGINE_API bool WKeyDown;
    static DUCKENGINE_API bool AKeyDown;
    static DUCKENGINE_API bool SKeyDown;
    static DUCKENGINE_API bool DKeyDown;
    static DUCKENGINE_API bool ToggleEditor;

    static DUCKENGINE_API int GetWindowWidth();
    static DUCKENGINE_API int GetWindowHeight();
    static DUCKENGINE_API void SetWindowTitle(std::string title);
    static DUCKENGINE_API float DeltaTime();
    static DUCKENGINE_API float FPS();

    static DUCKENGINE_API void SetCameraPosition(const float x, const float y);
    static DUCKENGINE_API void SetCameraHeight(const int height);

    static DUCKENGINE_API void SetBackgroundColor(const float r, const float g, const float b, const float a);

    static DUCKENGINE_API void RenderText(const std::string& text, const Vector2D& position, float scale, const Color& color);

    static DUCKENGINE_API void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);
    static DUCKENGINE_API void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);
    //static void DrawRectangle(const Vector2D& position, const Vector2D& size, float rotation, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static DUCKENGINE_API void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);

    static void DUCKENGINE_API DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, float rotation = 0.f, const Color& color = { 255.f, 0.f, 0.f, 255.f }, bool relativeToCamera = true);
};
