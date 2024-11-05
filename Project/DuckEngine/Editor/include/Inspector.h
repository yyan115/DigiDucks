#pragma once
#include <unordered_map>
#include <unordered_set>
#include "ComponentManager.h"
#include "imgui.h"

class InspectorRenderer 
{
public:
    static std::unordered_map<int, bool> entityChanges;
    static void RenderLayer(int entityID);
    static void RenderComponents(int entityID);
    static void AddComponents(int entityID, bool& hasChanged);
private:
    static bool IsAllowedExtension(const std::string& filePath, const std::unordered_set<std::string>& allowedExtensions);

    // List of available component types for adding
    static const std::vector<std::string> componentTypes;
    static void AddComponentToEntity(const std::string& componentName, int entityID);
};
