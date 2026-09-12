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
#include <algorithm>
#include <cmath>
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
				gameSettingsBtnSound->Play();
				ShowSettings(!isSettingsOpen);
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
            masterVolumeSliderComp->currentValue = SaveLoadManager::masterVolume;
            masterVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Value").get()->entityID);
            masterVolumeSliderComp->isEnable = false;
        }
    }

    bgmVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Slider").get();
    if (bgmVolumeSlider) {
        bgmVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(bgmVolumeSlider->entityID);
        if (bgmVolumeSliderComp) {
            bgmVolumeSliderComp->currentValue = SaveLoadManager::musicVolume;
            bgmVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Value").get()->entityID);
			bgmVolumeSliderComp->isEnable = false;
        }
    }

    sfxVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Slider").get();
    if (sfxVolumeSlider) {
        sfxVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(sfxVolumeSlider->entityID);
        if (sfxVolumeSliderComp) {
            sfxVolumeSliderComp->currentValue = SaveLoadManager::sfxVolume;
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
            fpsSliderComp->currentValue = static_cast<float>(SaveLoadManager::targetFPS);
            fpsText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_Value").get()->entityID);
			fpsSliderComp->isEnable = false;
        }
    }

    // VSync Toggle
    vsyncToggle = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("VSync_Toggle").get();
    if (vsyncToggle) {
        vsyncToggleTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(vsyncToggle->entityID);
        if (vsyncToggleTransform) {
            vsyncToggleNormalScale = vsyncToggleTransform->scale;
        }
        vsyncButtonComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(vsyncToggle->entityID);
        if (vsyncButtonComp) {
            vsyncToggleSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(vsyncToggle->entityID);
			vsyncToggle_Enabled = AssetManager::GetTextureByName("vsyncbox_tick");
			vsyncToggle_Disabled = AssetManager::GetTextureByName("vsyncbox");
			vsyncBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(vsyncToggle->entityID);
			vsyncToggleSpt->texture = SaveLoadManager::useVSync ? vsyncToggle_Enabled : vsyncToggle_Disabled;
			vsyncButtonComp->isEnabled = false;
            vsyncButtonComp->onClick = [this]() {
				ToggleVSync();
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
                closeSettingsBtnSound->Play();
                ShowSettings(false);
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
    if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
        DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_RIGHT)) {
        if (isUsingController) {
            DeselectSettings();
            isUsingController = false;
        }
    }

    if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1)) {
        if (!isSettingsOpen && DuckEngine_Input::IsGamepadButtonPressed(
            DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_Y)) {
            ShowSettings(true);
            isUsingController = true;
            SelectSetting(SettingSelection::MASTER_VOLUME);
            controllerNavigationCooldown = controllerNavigationDelay;
        }
        else if (isSettingsOpen) {
            UpdateControllerInput();
        }
    }

    if (isSettingsOpen) {
        UpdateSliders();
    }
}

/****************************************************************
* @brief Show or hide the settings menu.
****************************************************************/
void GameSettingsLogic::ShowSettings(bool state) {
    isSettingsOpen = state;
    settingsVisible = state;
    if (settingsMenuSpt) { 
        settingsMenuSpt->isVisible = state; 
		if (masterVolumeSliderComp) masterVolumeSliderComp->isEnable = state;
		if (bgmVolumeSliderComp) bgmVolumeSliderComp->isEnable = state;
		if (sfxVolumeSliderComp) sfxVolumeSliderComp->isEnable = state;
		if (fpsSliderComp) fpsSliderComp->isEnable = state;
		if (vsyncButtonComp) vsyncButtonComp->isEnabled = state;
        if (closeSettingsButton) closeSettingsButton->isEnabled = state;
    }

    if (!state) {
        DeselectSettings();
        isUsingController = false;
    }
}

void GameSettingsLogic::UpdateControllerInput() {
    if (DuckEngine_Input::IsGamepadButtonPressed(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B) ||
        DuckEngine_Input::IsGamepadButtonPressed(
            DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_Y)) {
        if (closeSettingsBtnSound) closeSettingsBtnSound->Play();
        ShowSettings(false);
        return;
    }

    if (controllerNavigationCooldown > 0.0f) {
        controllerNavigationCooldown -= DuckEngine::DeltaTime();
    }

    const float leftHorizontal = DuckEngine_Input::GetGamepadAxisValue(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
    const float rightHorizontal = DuckEngine_Input::GetGamepadAxisValue(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_X);
    const float leftVertical = DuckEngine_Input::GetGamepadAxisValue(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);
    const float rightVertical = DuckEngine_Input::GetGamepadAxisValue(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_Y);

    const bool navigateUp = leftVertical < -0.3f || rightVertical < -0.3f ||
        DuckEngine_Input::IsGamepadButtonDown(
            DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP);
    const bool navigateDown = leftVertical > 0.3f || rightVertical > 0.3f ||
        DuckEngine_Input::IsGamepadButtonDown(
            DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN);
    const bool navigateLeft = leftHorizontal < -0.3f || rightHorizontal < -0.3f ||
        DuckEngine_Input::IsGamepadButtonDown(
            DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT);
    const bool navigateRight = leftHorizontal > 0.3f || rightHorizontal > 0.3f ||
        DuckEngine_Input::IsGamepadButtonDown(
            DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);
    const bool activate = DuckEngine_Input::IsGamepadButtonPressed(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

    if (!isUsingController && (navigateUp || navigateDown || navigateLeft || navigateRight || activate)) {
        isUsingController = true;
        SelectSetting(SettingSelection::MASTER_VOLUME);
        controllerNavigationCooldown = controllerNavigationDelay;
        return;
    }

    if (!isUsingController) return;

    if (controllerNavigationCooldown <= 0.0f && navigateUp) {
        int next = static_cast<int>(currentSelection) - 1;
        if (next < 0) next = static_cast<int>(SettingSelection::COUNT) - 1;
        SelectSetting(static_cast<SettingSelection>(next));
        controllerNavigationCooldown = controllerNavigationDelay;
    }
    else if (controllerNavigationCooldown <= 0.0f && navigateDown) {
        const int next = (static_cast<int>(currentSelection) + 1) %
            static_cast<int>(SettingSelection::COUNT);
        SelectSetting(static_cast<SettingSelection>(next));
        controllerNavigationCooldown = controllerNavigationDelay;
    }
    else if (controllerNavigationCooldown <= 0.0f && (navigateLeft || navigateRight)) {
        AdjustSelectedSlider(navigateRight ? 1.0f : -1.0f);
        controllerNavigationCooldown = controllerNavigationDelay;
    }

    if (activate) {
        if (currentSelection == SettingSelection::VSYNC) {
            ToggleVSync();
        }
        else if (currentSelection == SettingSelection::CLOSE) {
            if (closeSettingsBtnSound) closeSettingsBtnSound->Play();
            ShowSettings(false);
        }
    }
}

void GameSettingsLogic::SelectSetting(SettingSelection selection) {
    DeselectSettings();
    currentSelection = selection;

    const Color selectedColor(218.0f, 121.0f, 52.0f, 255.0f);
    switch (selection) {
    case SettingSelection::MASTER_VOLUME:
        if (masterVolumeText) masterVolumeText->color = selectedColor;
        break;
    case SettingSelection::BGM_VOLUME:
        if (bgmVolumeText) bgmVolumeText->color = selectedColor;
        break;
    case SettingSelection::SFX_VOLUME:
        if (sfxVolumeText) sfxVolumeText->color = selectedColor;
        break;
    case SettingSelection::FPS_TARGET:
        if (fpsText) fpsText->color = selectedColor;
        break;
    case SettingSelection::VSYNC:
        if (vsyncToggleTransform) vsyncToggleTransform->scale = vsyncToggleNormalScale * 1.25f;
        break;
    case SettingSelection::CLOSE:
        if (closeSettingsBtnSpt) closeSettingsBtnSpt->texture = closeSettingsBtn_Hover;
        break;
    default:
        break;
    }

    if (gameSettingsBtnSound) gameSettingsBtnSound->Play(1);
}

void GameSettingsLogic::DeselectSettings() {
    const Color normalColor(10.0f, 10.0f, 10.0f, 255.0f);
    if (masterVolumeText) masterVolumeText->color = normalColor;
    if (bgmVolumeText) bgmVolumeText->color = normalColor;
    if (sfxVolumeText) sfxVolumeText->color = normalColor;
    if (fpsText) fpsText->color = normalColor;
    if (vsyncToggleTransform) vsyncToggleTransform->scale = vsyncToggleNormalScale;
    if (closeSettingsBtnSpt) closeSettingsBtnSpt->texture = closeSettingsBtn_Normal;
}

void GameSettingsLogic::AdjustSelectedSlider(float direction) {
    SliderComponent* slider = nullptr;
    switch (currentSelection) {
    case SettingSelection::MASTER_VOLUME:
        slider = masterVolumeSliderComp;
        break;
    case SettingSelection::BGM_VOLUME:
        slider = bgmVolumeSliderComp;
        break;
    case SettingSelection::SFX_VOLUME:
        slider = sfxVolumeSliderComp;
        break;
    case SettingSelection::FPS_TARGET:
        slider = fpsSliderComp;
        break;
    default:
        return;
    }

    if (!slider) return;
    slider->currentValue = std::clamp(
        slider->currentValue + direction * slider->step,
        slider->minValue,
        slider->maxValue);
}

void GameSettingsLogic::ToggleVSync() {
    if (vsyncBtnSound) vsyncBtnSound->Play();
    SaveLoadManager::useVSync = !SaveLoadManager::useVSync;
    if (vsyncToggleSpt) {
        vsyncToggleSpt->texture = SaveLoadManager::useVSync ? vsyncToggle_Enabled : vsyncToggle_Disabled;
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
		SaveLoadManager::targetFPS = static_cast<int>(fpsSliderComp->currentValue);
        fpsText->text = std::to_string(static_cast<int>(fpsSliderComp->currentValue));
    }
}
