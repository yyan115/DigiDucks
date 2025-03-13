/******************************************************************************/
/*!
\file       ProjectSettingsWindow.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Declares the ProjectSettingsWindow class, which provides a UI
            interface for modifying project settings using ImGui. This window
            allows users to adjust window properties, frame rate, VSync, and
            audio volumes dynamically within the LevelEditor.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

class ProjectSettingsWindow
{
public:
	// Show the ImGui window if `open == true`
	static void Show(bool& open);
};
