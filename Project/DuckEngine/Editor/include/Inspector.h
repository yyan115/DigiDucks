#pragma once
#include <unordered_map>
#include <unordered_set>
#include "ComponentManager.h"
#include "imgui.h"

class InspectorRenderer 
{
public:
    static std::unordered_map<int, bool> entityChanges;
    static void RenderComponents(int entityID);
private:
    static bool IsAllowedExtension(const std::string& filePath, const std::unordered_set<std::string>& allowedExtensions);

    
};