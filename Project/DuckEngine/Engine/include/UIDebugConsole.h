#pragma once

#include <string>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API UIDebugConsole {
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
