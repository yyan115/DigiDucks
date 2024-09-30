#pragma once

#include <vector>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"
#include "Serialization.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API Prefab
{
public:
    Prefab();
    Prefab(const char* name, const char* texturePath = "", Vec2 scale = { 1.0f, 1.0f });
    ~Prefab();

    void AddComponent(const std::shared_ptr<Component>& component);
    Entity* Instantiate(Vec2 newPosition);
    nlohmann::json& GetComponentsData();
    void SetComponentsData(nlohmann::json& components);

private:
    struct Impl;
    Impl* impl;
};
