#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "UIDebugConsole.h"
#include "imgui.h"

UIDebugConsole UIDebugConsole::debugConsole;

// Add a message to the log
void UIDebugConsole::AddLog(const char* fmt, ...) {
    
    const int bufferSize = 1024;
    char message[bufferSize];

    // Initialize variadic argument list
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, bufferSize, fmt, args);
    va_end(args);

    // Append the formatted message to the log
    buffer.append(message);
    buffer.append("\n");

    // Clear the buffer if it exceeds 200 lines
    if (std::count(buffer.begin(), buffer.end(), '\n') > 200) {
        Clear();
    }
}

// Clear the log
void UIDebugConsole::Clear() {
    buffer.clear();
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
        AddLog(inputBuf);

        // Execute command
        if (strcmp(inputBuf, "clear") == 0) {
            Clear();  // Clear the log if the command is "clear"
        }
        else {
            AddLog("Unknown command");
        }

        // Clear input buffer after each command
        inputBuf[0] = '\0';
    }

    ImGui::Separator();

    // Display log area
    ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextUnformatted(buffer.c_str());

    // Scroll to the bottom if needed
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::End();
}