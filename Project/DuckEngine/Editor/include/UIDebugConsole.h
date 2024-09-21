//---------------------------------------------------------
// File:    UIDebugConsole.h
//authors:	Muhammad Zikry Bin Zakaria
// email:	muhammadzikry.b\@digipen.edu
// 
//
// Brief:     Contains the declaration that is used to manage the debug console in the editor
//
// Copyright © 2024 DigiPen, All rights reserved.
//---------------------------------------------------------

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
    // Helper to determine color
    ImVec4 GetColorByLevel(const std::string& level);  
};
