/******************************************************************************/
/*!
\file       ProjectSettingsWindow.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       March 13 2025
\brief      Implements the ProjectSettingsWindow class, providing an ImGui-
			based interface for modifying project settings such as window
			resolution, frame rate, VSync, and volume controls. This window
			interacts with the ProjectSettings system and allows users to
			adjust settings dynamically within the LevelEditor.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "ProjectSettingsWindow.h"
#include "ProjectSettings.h"
#include "imgui.h"
#include <string>
#include <cstring>

void ProjectSettingsWindow::Show(bool& open)
{
	if (!open)
	{
		return;
	}

	// A resizable window with no collapse button
	if (ImGui::Begin("Project Settings", &open, ImGuiWindowFlags_NoCollapse))
	{
		// Create TWO columns (Label on the left, Input/Slider on the right).
		ImGui::Columns(2, nullptr, /*border=*/false);

		// -------------------------------------------------------------
		// 1) Start Level
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Start Level");
		ImGui::NextColumn();

		static char levelBuffer[256];
		{
			// Copy from engine to local buffer
			const std::string& startLevel = ProjectSettings::GetStartLevel();
			std::memset(levelBuffer, 0, sizeof(levelBuffer));
			strncpy_s(levelBuffer, sizeof(levelBuffer), startLevel.c_str(), _TRUNCATE);
		}
		ImGui::SetNextItemWidth(-1); // fill the whole right column
		if (ImGui::InputText("##StartLevel", levelBuffer, IM_ARRAYSIZE(levelBuffer)))
		{
			ProjectSettings::SetStartLevel(levelBuffer);
		}
		ImGui::NextColumn(); // Move to first column of the next row

		// -------------------------------------------------------------
		// 2) Window Width
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Window Width");
		ImGui::NextColumn();

		{
			int width = ProjectSettings::GetWindowWidth();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputInt("##WindowWidth", &width, 1, 5))
			{
				ProjectSettings::SetWindowWidth(width);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 3) Window Height
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Window Height");
		ImGui::NextColumn();

		{
			int height = ProjectSettings::GetWindowHeight();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputInt("##WindowHeight", &height, 1, 5))
			{
				ProjectSettings::SetWindowHeight(height);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 4) Target FPS
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Target FPS");
		ImGui::NextColumn();

		{
			int fps = ProjectSettings::GetTargetFPS();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::InputInt("##TargetFPS", &fps, 1, 10))
			{
				ProjectSettings::SetTargetFPS(fps);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 5) Use VSync
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Use VSync");
		ImGui::NextColumn();

		{
			bool vsync = ProjectSettings::GetUseVSync();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::Checkbox("##UseVSync", &vsync))
			{
				ProjectSettings::SetUseVSync(vsync);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 6) Volume: Master
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Master Volume");
		ImGui::NextColumn();

		{
			// Example function calls — you need to implement these
			float masterVol = ProjectSettings::GetMasterVolume();
			ImGui::SetNextItemWidth(-1);
			if (ImGui::SliderFloat("##MasterVolume", &masterVol, 0.0f, 1.0f, "%.2f"))
			{
				ProjectSettings::SetMasterVolume(masterVol);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 7) Volume: Default
		// -------------------------------------------------------------
		ImGui::TextUnformatted("Default Volume");
		ImGui::NextColumn();

		{
			float defaultVol = ProjectSettings::GetVolumeCategory("Default");
			ImGui::SetNextItemWidth(-1);
			if (ImGui::SliderFloat("##DefaultVolume", &defaultVol, 0.0f, 1.0f, "%.2f"))
			{
				ProjectSettings::SetVolumeCategory("Default", defaultVol);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 8) Volume: BGM
		// -------------------------------------------------------------
		ImGui::TextUnformatted("BGM Volume");
		ImGui::NextColumn();

		{
			float bgmVol = ProjectSettings::GetVolumeCategory("BGM");
			ImGui::SetNextItemWidth(-1);
			if (ImGui::SliderFloat("##BGMVolume", &bgmVol, 0.0f, 1.0f, "%.2f"))
			{
				ProjectSettings::SetVolumeCategory("BGM", bgmVol);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 9) Volume: SFX
		// -------------------------------------------------------------
		ImGui::TextUnformatted("SFX Volume");
		ImGui::NextColumn();

		{
			float sfxVol = ProjectSettings::GetVolumeCategory("SFX");
			ImGui::SetNextItemWidth(-1);
			if (ImGui::SliderFloat("##SFXVolume", &sfxVol, 0.0f, 1.0f, "%.2f"))
			{
				ProjectSettings::SetVolumeCategory("SFX", sfxVol);
			}
		}
		ImGui::NextColumn();

		// -------------------------------------------------------------
		// 10) Volume: UI
		// -------------------------------------------------------------
		ImGui::TextUnformatted("UI Volume");
		ImGui::NextColumn();

		{
			float uiVol = ProjectSettings::GetVolumeCategory("UI");
			ImGui::SetNextItemWidth(-1);
			if (ImGui::SliderFloat("##UIVolume", &uiVol, 0.0f, 1.0f, "%.2f"))
			{
				ProjectSettings::SetVolumeCategory("UI", uiVol);
			}
		}
		ImGui::NextColumn();

		// Close columns
		ImGui::Columns(1);

		// Separator
		ImGui::Separator();

		// Save button
		if (ImGui::Button("Save Settings"))
		{
			ProjectSettings::Save("Resources/settings.json");
		}

		ImGui::End();
	}
}
