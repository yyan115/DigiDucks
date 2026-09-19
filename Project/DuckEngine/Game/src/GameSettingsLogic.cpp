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
#include "PauseMenuLogic.h"
#include "SoundSystem.h"
#include "SaveLoadManager.h"
#include "WindowManager.h"
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
                // The gear opens the pause menu, which has OPTIONS on it, so
                // there is one settings menu rather than two. Where there is
                // no pause menu it still opens the panel itself.
                if (auto pauseMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu"))
                {
                    if (auto pause = GameLogicManager::GetLogicForEntity<PauseMenuLogic>(pauseMenu->entityID))
                    {
                        if (!pause->isPaused)
                        {
                            pause->PauseGame(true);
                            pause->playPauseSound();
                        }
                        return;
                    }
                }
                settingsVisible = !settingsVisible;
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
            masterVolumeSliderComp->currentValue = SaveLoadManager::masterVolume;
            masterVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(
                DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Value").get()->entityID);
            masterVolumeSliderComp->isEnable = false;
            masterTrackSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(masterVolumeSlider->entityID);
            if (auto fillEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_SliderFill")) { masterFillSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fillEntity->entityID); }
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
            bgmTrackSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(bgmVolumeSlider->entityID);
            if (auto fillEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_SliderFill")) { bgmFillSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fillEntity->entityID); }
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
            sfxTrackSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(sfxVolumeSlider->entityID);
            if (auto fillEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_SliderFill")) { sfxFillSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fillEntity->entityID); }
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
            fpsTrackSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fpsSlider->entityID);
            if (auto fillEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_SliderFill")) { fpsFillSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(fillEntity->entityID); }
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
			vsyncToggleTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(vsyncToggle->entityID);
			if (vsyncToggleTransform) { vsyncToggleOriginalScale = vsyncToggleTransform->scale; }
			vsyncToggleSpt->texture = SaveLoadManager::useVSync ? vsyncToggle_Enabled : vsyncToggle_Disabled;
			vsyncButtonComp->isEnabled = false;
            vsyncButtonComp->onClick = [this]() {
				vsyncBtnSound->Play();
				SaveLoadManager::useVSync = !SaveLoadManager::useVSync;
                ProjectSettings::SetUseVSync(SaveLoadManager::useVSync);
                WindowManager::SetVSync(SaveLoadManager::useVSync);
                settingsDirty = true;
                if (vsyncToggleSpt) {
                    vsyncToggleSpt->texture = SaveLoadManager::useVSync ? vsyncToggle_Enabled : vsyncToggle_Disabled;
                }
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
        closeSettingsBtnTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(CloseSettingsBtn->entityID);
        if (closeSettingsBtnTransform) { closeSettingsBtnOriginalScale = closeSettingsBtnTransform->scale; }
		
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
        UpdateControllerNavigation();
        UpdateSliders();
    }
}

namespace
{
    // The sliders are drawn with a colour and no texture, so a colour is what
    // marks the selected one. Grey 100 and yellow are what the scene files
    // give them, so kTrackNormal and kFillNormal leave an unselected row
    // looking exactly as it does today.
    // A selected row is drawn stronger, not paler: a washed-out bar reads as
    // disabled. The fill deepens towards orange and the empty part of the
    // track darkens, so the selection is legible whether the bar is full or
    // empty. These four are the whole of the look.
    const Color kTrackNormal{ 100.f, 100.f, 100.f, 100.f };
    const Color kTrackSelected{ 120.f, 85.f, 40.f, 200.f };
    const Color kFillNormal{ 255.f, 190.f, 0.f, 255.f };
    const Color kFillSelected{ 255.f, 130.f, 20.f, 255.f };
}

SliderComponent* GameSettingsLogic::SelectedSlider() const
{
    switch (selectedControl)
    {
    case SettingsControl::MASTER: return masterVolumeSliderComp;
    case SettingsControl::BGM:    return bgmVolumeSliderComp;
    case SettingsControl::SFX:    return sfxVolumeSliderComp;
    case SettingsControl::FPS:    return fpsSliderComp;
    default:                      return nullptr;
    }
}

void GameSettingsLogic::ClearSelection()
{
    if (masterTrackSpt) masterTrackSpt->color = kTrackNormal;
    if (bgmTrackSpt)    bgmTrackSpt->color = kTrackNormal;
    if (sfxTrackSpt)    sfxTrackSpt->color = kTrackNormal;
    if (fpsTrackSpt)    fpsTrackSpt->color = kTrackNormal;
    if (masterFillSpt) masterFillSpt->color = kFillNormal;
    if (bgmFillSpt)    bgmFillSpt->color = kFillNormal;
    if (sfxFillSpt)    sfxFillSpt->color = kFillNormal;
    if (fpsFillSpt)    fpsFillSpt->color = kFillNormal;
    if (vsyncToggleSpt) vsyncToggleSpt->texture =
        SaveLoadManager::useVSync ? vsyncToggle_Enabled : vsyncToggle_Disabled;
    if (vsyncToggleTransform) vsyncToggleTransform->scale = vsyncToggleOriginalScale;
    if (closeSettingsBtnTransform) closeSettingsBtnTransform->scale = closeSettingsBtnOriginalScale;
}

void GameSettingsLogic::ShowSelection()
{
    ClearSelection();
    switch (selectedControl)
    {
    case SettingsControl::MASTER:
        if (masterTrackSpt) masterTrackSpt->color = kTrackSelected;
        if (masterFillSpt)  masterFillSpt->color = kFillSelected;
        break;
    case SettingsControl::BGM:
        if (bgmTrackSpt) bgmTrackSpt->color = kTrackSelected;
        if (bgmFillSpt)  bgmFillSpt->color = kFillSelected;
        break;
    case SettingsControl::SFX:
        if (sfxTrackSpt) sfxTrackSpt->color = kTrackSelected;
        if (sfxFillSpt)  sfxFillSpt->color = kFillSelected;
        break;
    case SettingsControl::FPS:
        if (fpsTrackSpt) fpsTrackSpt->color = kTrackSelected;
        if (fpsFillSpt)  fpsFillSpt->color = kFillSelected;
        break;
    case SettingsControl::VSYNC:
        if (vsyncToggleTransform)
            vsyncToggleTransform->scale = vsyncToggleOriginalScale * buttonScaleIncrease;
        break;
    case SettingsControl::CLOSE:
        if (closeSettingsBtnTransform)
            closeSettingsBtnTransform->scale = closeSettingsBtnOriginalScale * buttonScaleIncrease;
        break;
    default: break;
    }
}

void GameSettingsLogic::NudgeSelectedSlider(float direction)
{
    SliderComponent* slider = SelectedSlider();
    if (!slider) { return; }
    const float step = (slider->step > 0.f) ? slider->step : 0.1f;
    float value = slider->currentValue + direction * step;
    if (value < slider->minValue) value = slider->minValue;
    if (value > slider->maxValue) value = slider->maxValue;
    slider->currentValue = value;
}

void GameSettingsLogic::ActivateSelected()
{
    if (selectedControl == SettingsControl::VSYNC && vsyncButtonComp &&
        vsyncButtonComp->onClick)
    {
        vsyncButtonComp->onClick();
        ShowSelection();
    }
    else if (selectedControl == SettingsControl::CLOSE && closeSettingsButton &&
             closeSettingsButton->onClick)
    {
        closeSettingsButton->onClick();
    }
}

void GameSettingsLogic::UpdateControllerNavigation()
{
    if (!DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
    {
        if (usingController)
        {
            usingController = false;
            ClearSelection();
        }
        return;
    }

    if (controllerNavigationCooldown > 0.f)
    {
        controllerNavigationCooldown -= DuckEngine::PauseDeltaTime();
    }

    const float vertical = DuckEngine_Input::GetMenuAxisVertical(DuckEngine_Input::GAMEPAD_1);
    const float horizontal = DuckEngine_Input::GetMenuAxisHorizontal(DuckEngine_Input::GAMEPAD_1);
    const bool up = DuckEngine_Input::IsGamepadButtonDown(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP) || vertical > 0.3f;
    const bool down = DuckEngine_Input::IsGamepadButtonDown(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN) || vertical < -0.3f;
    const bool left = DuckEngine_Input::IsGamepadButtonDown(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT) || horizontal < -0.3f;
    const bool right = DuckEngine_Input::IsGamepadButtonDown(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT) || horizontal > 0.3f;
    const bool accept = DuckEngine_Input::IsGamepadButtonPressed(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);
    const bool cancel = DuckEngine_Input::IsGamepadButtonPressed(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B);

    // Cancel first, and whether or not the pad has been used yet. Closing is
    // the one input with no ambiguity about what it means, so it should never
    // be spent on waking the selection up.
    if (cancel)
    {
        ShowSettings(false);
        return;
    }

    if (!usingController && (up || down || left || right || accept))
    {
        // The first input selects rather than moves, the same as every other
        // menu here, so a player does not skip a row by arriving.
        usingController = true;
        selectedControl = SettingsControl::MASTER;
        ShowSelection();
        controllerNavigationCooldown = controllerNavigationDelay;
        return;
    }
    if (!usingController) { return; }

    if (controllerNavigationCooldown > 0.f) { return; }

    const int count = static_cast<int>(SettingsControl::COUNT);
    if (up || down)
    {
        int index = static_cast<int>(selectedControl) + (down ? 1 : -1);
        if (index < 0) index = count - 1;
        if (index >= count) index = 0;
        selectedControl = static_cast<SettingsControl>(index);
        ShowSelection();
        controllerNavigationCooldown = controllerNavigationDelay;
    }
    else if (left || right)
    {
        NudgeSelectedSlider(right ? 1.f : -1.f);
        controllerNavigationCooldown = controllerNavigationDelay;
    }
    else if (accept)
    {
        ActivateSelected();
        controllerNavigationCooldown = controllerNavigationDelay;
    }
}

/****************************************************************
* @brief Show or hide the settings menu.
****************************************************************/
void GameSettingsLogic::ShowSettings(bool state, bool fromPad) {
    const bool wasOpen = isSettingsOpen;
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

    if (!state && wasOpen)
    {
        ClearSelection();
        usingController = false;
        selectedControl = SettingsControl::MASTER;
    }

    if (state && !wasOpen && fromPad)
    {
        // Opened from the pad, so the player is already holding one. Selecting
        // MASTER here is what stops the next press being eaten by the wake
        // below, which made B have to be pressed twice to close a panel the
        // pad had just opened.
        usingController = true;
        selectedControl = SettingsControl::MASTER;
        ShowSelection();
        controllerNavigationCooldown = controllerNavigationDelay;
    }

    if (!state && wasOpen && settingsDirty) {
        SaveLoadManager::SaveGame();
        settingsDirty = false;
    }
}

/****************************************************************
* @brief Update slider values in real time.
****************************************************************/
void GameSettingsLogic::UpdateSliders() {
    if (masterVolumeSliderComp) {
        const float value = masterVolumeSliderComp->currentValue;
        if (SaveLoadManager::masterVolume != value) {
            SoundSystem::SetMasterVolume(value);
			SaveLoadManager::masterVolume = value;
            ProjectSettings::SetMasterVolume(value);
            settingsDirty = true;
        }
        if (masterVolumeText) masterVolumeText->text = std::to_string(static_cast<int>(value * 100));
    }

    if (bgmVolumeSliderComp) {
		const float value = bgmVolumeSliderComp->currentValue;
        if (SaveLoadManager::musicVolume != value) {
			SoundSystem::SetCategoryVolume("BGM", value);
			SaveLoadManager::musicVolume = value;
            ProjectSettings::SetVolumeCategory("BGM", value);
            settingsDirty = true;
        }
        if (bgmVolumeText) bgmVolumeText->text = std::to_string(static_cast<int>(value * 100));
    }

    if (sfxVolumeSliderComp) {
		const float value = sfxVolumeSliderComp->currentValue;
        if (SaveLoadManager::sfxVolume != value) {
			SoundSystem::SetCategoryVolume("SFX", value);
			SaveLoadManager::sfxVolume = value;
            ProjectSettings::SetVolumeCategory("SFX", value);
            settingsDirty = true;
        }
        if (sfxVolumeText) sfxVolumeText->text = std::to_string(static_cast<int>(value * 100));
    }

    if (fpsSliderComp) {
		const int value = static_cast<int>(fpsSliderComp->currentValue);
        if (SaveLoadManager::targetFPS != value) {
			SaveLoadManager::targetFPS = value;
            ProjectSettings::SetTargetFPS(value);
            settingsDirty = true;
        }
        if (fpsText) fpsText->text = std::to_string(value);
    }
}
