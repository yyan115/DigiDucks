#pragma once
#include <unordered_map>
#include "ComponentManager.h"
#include "imgui.h"

class InspectorRenderer 
{
public:
    static std::unordered_map<int, bool> entityChanges;
    static void RenderComponents(int entityID);

    
};