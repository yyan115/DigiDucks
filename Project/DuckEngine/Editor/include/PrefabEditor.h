#pragma once
#include <string>
#include <memory>
#include "PrefabManager.h"
#include "imgui.h"

class PrefabEditor
{
public:
    static void OpenPrefabEditor(const std::string& prefabName);
    static void Render();

private:
    static bool isOpen;
    static std::string currentPrefabName;
    static std::shared_ptr<Prefab> currentPrefab;

    static void RenderPrefabProperties();
    static void RenderPrefabPreview();
    static void AddComponent();
};
