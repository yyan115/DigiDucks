#include "GameSettingsLogic.h"
#include <iostream>

/****************************************************************
* @brief Start function for the Game Settings Logic
****************************************************************/
void GameSettingsLogic::Start() {
    auto gameSettingsBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Btn").get();
    if (gameSettingsBtn)
    {
        gameSettingsBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSettingsBtn->entityID);
        gameSettingsBtn_Normal = AssetManager::GetTextureByName("pause_howtoplay");
        gameSettingsBtn_Hover = AssetManager::GetTextureByName("pause_howtoplay_hover");
        gameSettingsButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSettingsBtn->entityID);
        if (gameSettingsButton) {
            gameSettingsButton->onClick = [this]() { ShowSettings(true); };
            gameSettingsButton->onHover = [this]() { gameSettingsBtnSpt->texture = gameSettingsBtn_Hover; };
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
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Text").get()->entityID);
            masterVolumeSliderComp->isEnable = false;
        }
    }

    bgmVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Slider").get();
    if (bgmVolumeSlider) {
        bgmVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(bgmVolumeSlider->entityID);
        if (bgmVolumeSliderComp) {
            bgmVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("BGM");
            bgmVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Text").get()->entityID);
			bgmVolumeSliderComp->isEnable = false;
        }
    }

    sfxVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Slider").get();
    if (sfxVolumeSlider) {
        sfxVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(sfxVolumeSlider->entityID);
        if (sfxVolumeSliderComp) {
            sfxVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("SFX");
            sfxVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Text").get()->entityID);
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
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_Text").get()->entityID);
			fpsSliderComp->isEnable = false;
        }
    }

    // VSync Toggle
    vsyncToggle = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("VSync_Toggle").get();
    if (vsyncToggle) {
        vsyncButtonComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(vsyncToggle->entityID);
        if (vsyncButtonComp) {
			vsyncButtonComp->isEnabled = false;
            vsyncButtonComp->onClick = []() {
                bool vsync = !ProjectSettings::GetUseVSync();
                ProjectSettings::SetUseVSync(vsync);
                std::cout << "VSync: " << (vsync ? "Enabled" : "Disabled") << std::endl;
                };
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
    }
}

/****************************************************************
* @brief Update slider values in real time.
****************************************************************/
void GameSettingsLogic::UpdateSliders() {
    if (masterVolumeSliderComp) {
        ProjectSettings::SetMasterVolume(masterVolumeSliderComp->currentValue);
        masterVolumeText->text = std::to_string(static_cast<int>(masterVolumeSliderComp->currentValue * 100));
    }

    if (bgmVolumeSliderComp) {
        ProjectSettings::SetVolumeCategory("BGM", bgmVolumeSliderComp->currentValue);
        bgmVolumeText->text = std::to_string(static_cast<int>(bgmVolumeSliderComp->currentValue * 100));
    }

    if (sfxVolumeSliderComp) {
        ProjectSettings::SetVolumeCategory("SFX", sfxVolumeSliderComp->currentValue);
        sfxVolumeText->text = std::to_string(static_cast<int>(sfxVolumeSliderComp->currentValue * 100));
    }

    if (fpsSliderComp) {
        ProjectSettings::SetTargetFPS(static_cast<int>(fpsSliderComp->currentValue));
        fpsText->text = std::to_string(static_cast<int>(fpsSliderComp->currentValue));
    }
}
