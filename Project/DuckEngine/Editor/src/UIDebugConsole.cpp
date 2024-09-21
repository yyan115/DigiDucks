//---------------------------------------------------------
// File:    UIDebugConsole.cpp
//authors:	Muhammad Zikry Bin Zakaria
// email:	muhammadzikry.b\@digipen.edu
// 
//
// Brief:     Contains the definition that is used to manage the debug console in the editor
//
// Copyright © 2024 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "UIDebugConsole.h"
#include "imgui.h"

// GLOBALS
UIDebugConsole UIDebugConsole::debugConsole;
std::vector<std::tuple<std::string, ImVec4>> logEntries;  // Store message and color


// Add a message to the log
void UIDebugConsole::AddLog(const char* level, const char* fmt, ...) {
    
    const int bufferSize = 4096;
    char message[bufferSize];

    // Initialize variadic argument list
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, bufferSize, fmt, args);
    va_end(args);

    // provide color for each log level
    ImVec4 color;  
    color = GetColorByLevel(level);


    // Append to the log entries
    logEntries.emplace_back(std::string(message), color);

    // Clear the log if it exceeds 200 entries
    if(logEntries.size() > 200) {
        Clear();
    }
}

void UIDebugConsole::AddDebugLog(const char* fmt, ...) {
    const int bufferSize = 1024;
    char message[bufferSize];

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, bufferSize, fmt, args);
    va_end(args);

    // Add as regular text
    logEntries.emplace_back(std::string(message), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Clear the log if it exceeds 200 entries
    if (logEntries.size() > 200) {
        Clear();
    }
}

// Clear the log
void UIDebugConsole::Clear() {
    logEntries.clear();
}

// Render the console in ImGui
void UIDebugConsole::Render(bool* p_open) {
    if (!ImGui::Begin("Debug Console", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::End();
        return;
    }

    // Input field for commands
    static char inputBuf[256] = "";
    if (ImGui::InputText("Command", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        // Add input text to the log
        AddDebugLog(inputBuf);

        // Execute command
        if (strcmp(inputBuf, "clear") == 0) {
            Clear();  // Clear the log
        }
        else {
            AddLog("WARNING","Unknown command");
        }

        inputBuf[0] = '\0';
    }

    ImGui::Separator();

    // Display log area
    ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Render all log entries with their colors
    for (const auto& [message, color] : logEntries) {
        ImGui::PushStyleColor(ImGuiCol_Text, color);  
        ImGui::TextUnformatted(message.c_str());      
        ImGui::PopStyleColor();                       
    }

    // Scroll to the bottom if needed
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::End();
}

ImVec4 UIDebugConsole::GetColorByLevel(const std::string& level) {
    if (level == "INFO") {
        return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green for info
    }
    else if (level == "WARNING") {
        return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow for warning
    }
    else if (level == "ERROR") {
        return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red for error
    }

    // white for default
    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}