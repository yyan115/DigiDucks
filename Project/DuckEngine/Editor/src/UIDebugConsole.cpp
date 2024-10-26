/******************************************************************************/
/*!
\file    UIDebugConsole.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation to handle debug console in ImGui

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <cstdio>

#include "UIDebugConsole.h"

// GLOBALS
UIDebugConsole UIDebugConsole::debugConsole;


// Add a message to the log
void UIDebugConsole::AddLog(const char* level, const char* fmt, ...) {
    const int bufferSize = 4096;
    char message[bufferSize];

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, bufferSize, fmt, args);
    va_end(args);

    // Append the level and message to the log entries
    logEntries.emplace_back(std::string(level), std::string(message));

    // Clear the log if it exceeds 200 entries
    if (logEntries.size() > 200) {
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

    // Add as regular text with empty level (for debug logs)
    logEntries.emplace_back("INFO", std::string(message));

    // Clear the log if it exceeds 200 entries
    if (logEntries.size() > 200) {
        Clear();
    }
}

// Clear the log
void UIDebugConsole::Clear() {
    logEntries.clear();
}

// Return log entries (for rendering outside this class)
const std::vector<std::tuple<std::string, std::string>>& UIDebugConsole::GetLogEntries() const {
    return logEntries;
}
