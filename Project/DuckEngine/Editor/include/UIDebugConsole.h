#pragma once

#include <imgui.h>

class UIDebugConsole 
{
public:
    // For regular logging with severity levels
    void AddLog(const char* fmt, const char* level = nullptr, ...);

    // For normal debug text
    void AddDebugLog(const char* fmt, ...);

    // Clear the log
    void Clear();

    // Render the console in ImGui
    void Render(bool* p_open);

    static UIDebugConsole debugConsole;

private:

    

    ImVec4 GetColorByLevel(const std::string& level);  // Helper to determine color
};
