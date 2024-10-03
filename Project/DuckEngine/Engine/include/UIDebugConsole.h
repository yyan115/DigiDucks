/******************************************************************************/
/*!
\file    UIDebugConsole.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration to handle debug console in ImGui

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <imgui.h>

class UIDebugConsole 
{
public:
    /****************************************************************
    * @brief Adds a log message to the debug console with a specified log level
    *
    * @param level - The log level (INFO, WARNING, ERROR)
    * @param fmt - The format string for the log message
    *
    * @return void
    ***************************************************************/
    void AddLog(const char* fmt, const char* level = nullptr, ...);

    /****************************************************************
    * @brief Adds a general debug message to the console log
    *
    * @param fmt - The format string for the debug message
    *
    * @return void
    ***************************************************************/
    void AddDebugLog(const char* fmt, ...);

    /****************************************************************
    * @brief Clears all entries from the debug console log
    *
    * @return void
    ***************************************************************/
    void Clear();

    /****************************************************************
    * @brief Renders the debug console UI in ImGui, allowing interaction and viewing of log entries
    *
    * @param p_open - Pointer to a bool that indicates whether the console is open
    *
    * @return void
    ***************************************************************/
    void Render(bool* p_open);

    static UIDebugConsole debugConsole;

private:
    /****************************************************************
    * @brief Gets the color associated with a specific log level for display
    *
    * @param level - The log level as a string (INFO, WARNING, ERROR)
    *
    * @return The color to be used for the specified log level
    ***************************************************************/
    ImVec4 GetColorByLevel(const std::string& level);  
};
