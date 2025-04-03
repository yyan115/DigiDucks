/******************************************************************************/
/*!
\file     GameSettingsLogic.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the GameSettingsLogic class
		  which is responsible for handling the logic of the game settings.
		  The class is responsible for toggling the visibility of the settings menu
		  and updating the master volume slider based on the current master volume.
		  The class also updates the master volume based on the slider's current value.
		  The class is also responsible for updating the volume of the sound categories
		  based on the sliders in the settings menu.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameSettingsLogic.h"
#include "SoundSystem.h"
#include "SaveLoadManager.h"
#include <iostream>

/****************************************************************
* @brief Start function for the Game Settings Logic
****************************************************************/
void GameSettingsLogic::Start() {
    auto gameSettingsBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Btn").get();
    if (gameSettingsBtn)
    {
        gameSettingsBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSettingsBtn->entityID);
        gameSettingsBtn_Normal = AssetManager::GetTextureByName("optionbutton");
        gameSettingsBtn_Hover = AssetManager::GetTextureByName("optionbutton_hover");
        gameSettingsButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSettingsBtn->entityID);
		gameSettingsBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameSettingsBtn->entityID);
        if (gameSettingsButton) {
            gameSettingsButton->onClick = [this]() {
                settingsVisible = !settingsVisible;
				gameSettingsBtnSound->Play();
                ShowSettings(settingsVisible);
                };
            gameSettingsButton->onHover = [this]() { 
                gameSettingsBtnSpt->texture = gameSettingsBtn_Hover;
                gameSettingsBtnSound->Play(1);
                };
            gameSettingsButton->onFinishHover = [this]() { gameSettingsBtnSpt->texture = gameSettingsBtn_Normal; };
        }
    }
    // Settings Background
    settingsMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Menu").get();
    if (settingsMenu) {
        settingsMenuSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(settingsMenu->entityID);
        if (settingsMenuSpt) { settingsMenuSpt->isVisible = false; }
    }

    // Volume Sliders
    masterVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Slider").get();
    if (masterVolumeSlider) {
        masterVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(masterVolumeSlider->entityID);
        if (masterVolumeSliderComp) {
            masterVolumeSliderComp->currentValue = ProjectSettings::GetMasterVolume();
            masterVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Value").get()->entityID);
            masterVolumeSliderComp->isEnable = false;
        }
    }

    bgmVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Slider").get();
    if (bgmVolumeSlider) {
        bgmVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(bgmVolumeSlider->entityID);
        if (bgmVolumeSliderComp) {
            bgmVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("BGM");
            bgmVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Value").get()->entityID);
			bgmVolumeSliderComp->isEnable = false;
        }
    }

    sfxVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Slider").get();
    if (sfxVolumeSlider) {
        sfxVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(sfxVolumeSlider->entityID);
        if (sfxVolumeSliderComp) {
            sfxVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("SFX");
            sfxVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Value").get()->entityID);
			sfxVolumeSliderComp->isEnable = false;
        }
    }

    // FPS Slider
    fpsSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_Slider").get();
    if (fpsSlider) {
        fpsSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(fpsSlider->entityID);
        if (fpsSliderComp) {
            fpsSliderComp->currentValue = static_cast<float>(ProjectSettings::GetTargetFPS());
            fpsText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_Value").get()->entityID);
			fpsSliderComp->isEnable = false;
        }
    }

    // VSync Toggle
    vsyncToggle = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("VSync_Toggle").get();
    if (vsyncToggle) {
        vsyncButtonComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(vsyncToggle->entityID);
        if (vsyncButtonComp) {
            vsyncToggleSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(vsyncToggle->entityID);
			vsyncToggle_Enabled = AssetManager::GetTextureByName("vsyncbox_tick");
			vsyncToggle_Disabled = AssetManager::GetTextureByName("vsyncbox");
			vsyncBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(vsyncToggle->entityID);
			vsyncToggleSpt->texture = ProjectSettings::GetUseVSync() ? vsyncToggle_Enabled : vsyncToggle_Disabled;
			vsyncButtonComp->isEnabled = false;
            vsyncButtonComp->onClick = [this]() {
				vsyncBtnSound->Play();
                bool vsync = !ProjectSettings::GetUseVSync();
                ProjectSettings::SetUseVSync(vsync);
				SaveLoadManager::useVSync = vsync;
                vsyncToggleSpt->texture = ProjectSettings::GetUseVSync() ? vsyncToggle_Enabled : vsyncToggle_Disabled;
                };
        }
    }

	// close settings button
    auto CloseSettingsBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("CloseSettings_Button").get();
    if (CloseSettingsBtn)
    {
        closeSettingsBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CloseSettingsBtn->entityID);
        closeSettingsBtn_Normal = AssetManager::GetTextureByName("options_close");
        closeSettingsBtn_Hover = AssetManager::GetTextureByName("options_close_hover");
        closeSettingsButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(CloseSettingsBtn->entityID);
        closeSettingsBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CloseSettingsBtn->entityID);
		
        if (closeSettingsButton) {
            closeSettingsButton->isEnabled = false;

            closeSettingsButton->onClick = [this]() {
                settingsVisible = !settingsVisible;
                closeSettingsBtnSound->Play();
                ShowSettings(settingsVisible);
                };
            closeSettingsButton->onHover = [this]() {
                closeSettingsBtnSpt->texture = closeSettingsBtn_Hover;
                closeSettingsBtnSound->Play(1);
                };
            closeSettingsButton->onFinishHover = [this]() { closeSettingsBtnSpt->texture = closeSettingsBtn_Normal; };
        }
    }
}

/****************************************************************
* @brief Update function for the Game Settings Logic
****************************************************************/
void GameSettingsLogic::Update() {
    if (isSettingsOpen) {
        UpdateSliders();
    }
}

/****************************************************************
* @brief Show or hide the settings menu.
****************************************************************/
void GameSettingsLogic::ShowSettings(bool state) {
    isSettingsOpen = state;
    if (settingsMenuSpt) { 
        settingsMenuSpt->isVisible = state; 
        masterVolumeSliderComp->isEnable = state;
		bgmVolumeSliderComp->isEnable = state;
		sfxVolumeSliderComp->isEnable = state;
		fpsSliderComp->isEnable = state;
		vsyncButtonComp->isEnabled = state;
        closeSettingsButton->isEnabled = state;
    }
}

/****************************************************************
* @brief Update slider values in real time.
****************************************************************/
void GameSettingsLogic::UpdateSliders() {
    if (masterVolumeSliderComp) {
        SoundSystem::SetMasterVolume(masterVolumeSliderComp->currentValue);
		SaveLoadManager::masterVolume = masterVolumeSliderComp->currentValue;
        masterVolumeText->text = std::to_string(static_cast<int>(masterVolumeSliderComp->currentValue * 100));
    }

    if (bgmVolumeSliderComp) {
		SoundSystem::SetCategoryVolume("BGM", bgmVolumeSliderComp->currentValue);
		SaveLoadManager::musicVolume = bgmVolumeSliderComp->currentValue;
        bgmVolumeText->text = std::to_string(static_cast<int>(bgmVolumeSliderComp->currentValue * 100));
    }

    if (sfxVolumeSliderComp) {
		SoundSystem::SetCategoryVolume("SFX", sfxVolumeSliderComp->currentValue);
		SaveLoadManager::sfxVolume = sfxVolumeSliderComp->currentValue;
        sfxVolumeText->text = std::to_string(static_cast<int>(sfxVolumeSliderComp->currentValue * 100));
    }

    if (fpsSliderComp) {
        ProjectSettings::SetTargetFPS(static_cast<int>(fpsSliderComp->currentValue));
		SaveLoadManager::targetFPS = static_cast<int>(fpsSliderComp->currentValue);
        fpsText->text = std::to_string(static_cast<int>(fpsSliderComp->currentValue));
    }
}
