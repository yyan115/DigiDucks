#ifndef DUCKENGINE_H
#define DUCKENGINE_H

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class DUCKENGINE_API DuckEngine
{
public:
    static ComponentManager DUCKENGINE_ComponentManager;
    static EntityManager DUCKENGINE_EntityManager;
    static SystemManager DUCKENGINE_SystemManager;

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
    static float DeltaTime();

    static void SetCameraPosition(const float x, const float y);
    static void SetCameraHeight(const int height);

    static void SetBackgroundColor(const float r, const float g, const float b, const float a);



    static void DrawPoint(const Vector2D& position, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawRectangle(const Vector2D& position, const Vector2D& size, const Color& color = { 255.f, 0.f, 0.f, 255.f });
    static void DrawCircle(const Vector2D& position, float radius, const Color& color = { 255.f, 0.f, 0.f, 255.f });

};

#endif // DUCKENGINE_H
