#pragma once

#include <string>

class UIDebugConsole 
{
public:
    // Add a message to the log
    void AddLog(const char* fmt, ...);

    // Clear the log
    void Clear();

    // Render the console in ImGui
    void Render(bool* p_open);

    static UIDebugConsole debugConsole;

private:
    std::string buffer;  // Buffer to hold the log
};
