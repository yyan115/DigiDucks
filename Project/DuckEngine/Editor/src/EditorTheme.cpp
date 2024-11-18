/******************************************************************************/
/*!
\file     EditorTheme.CPP
\author   Jovan Chua (100%)
\par      c.shengkaijovan@digipen.edu
\brief    This file contains the definition of the EditorTheme class.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "EditorTheme.h"
#include "imgui.h"

void EditorTheme::SetDuckTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(1.25f); // Match the font scale factor
    ImVec4* colors = style.Colors;

    // General Background colors
    colors[ImGuiCol_WindowBg] = ImVec4(0.85f, 0.75f, 0.4f, 1.0f);     // Main window background
    colors[ImGuiCol_ChildBg] = ImVec4(0.8f, 0.7f, 0.35f, 1.0f);       // Child window background
    colors[ImGuiCol_PopupBg] = ImVec4(0.75f, 0.65f, 0.3f, 1.0f);      // Popup background

    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.6f, 0.5f, 0.2f, 1.0f);         // Yellowish border
    colors[ImGuiCol_BorderShadow] = ImVec4(0.4f, 0.4f, 0.0f, 0.5f);   // Darker yellow shadow

    // Text
    colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);           // Black text
    colors[ImGuiCol_TextDisabled] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);   // Gray text for disabled items

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.9f, 0.7f, 0.3f, 1.0f);         // Header background
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.85f, 0.65f, 0.25f, 1.0f); // Hovered header
    colors[ImGuiCol_HeaderActive] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);   // Active header

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.9f, 0.7f, 0.3f, 1.0f);         // Button background
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.85f, 0.65f, 0.25f, 1.0f); // Hovered button
    colors[ImGuiCol_ButtonActive] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);   // Active button

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.75f, 0.65f, 0.3f, 1.0f);          // Tab background
    colors[ImGuiCol_TabHovered] = ImVec4(0.85f, 0.65f, 0.25f, 1.0f);  // Hovered tab
    colors[ImGuiCol_TabActive] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);      // Active tab
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.7f, 0.6f, 0.25f, 1.0f);  // Unfocused tab
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.65f, 0.3f, 1.0f); // Unfocused active tab

    // Title Bar (for windows)
    colors[ImGuiCol_TitleBg] = ImVec4(0.75f, 0.65f, 0.3f, 1.0f);      // Title background
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);  // Active title background
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.75f, 0.65f, 0.3f, 1.0f); // Collapsed title

    // Menu Bar
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.8f, 0.7f, 0.35f, 1.0f);     // Menu bar background

    // Scrollbars
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.75f, 0.65f, 0.3f, 1.0f);  // Scrollbar background
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);  // Scrollbar grab
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.85f, 0.65f, 0.25f, 1.0f); // Hovered grab
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f); // Active grab

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4(0.8f, 0.6f, 0.2f, 1.0f);     // Slider grab
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.75f, 0.55f, 0.15f, 1.0f); // Active slider

    // Rounding settings for a more polished look
    style.WindowRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
}