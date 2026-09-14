/******************************************************************************/
/*!
\file     GameSettingsLogic.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the GameSettingsLogic class
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

#pragma once

#include "GameLogic.h"
#include "ProjectSettings.h"
#include "UISliderLogic.h"
#include "DuckEngine.h"

class GameSettingsLogic : public GameLogic
{
private:
	SpriteRendererComponent* gameSettingsBtnSpt = nullptr;
    Texture gameSettingsBtn_Normal {};
	Texture gameSettingsBtn_Hover{};
	ButtonComponent* gameSettingsButton = nullptr;
	SoundComponent* gameSettingsBtnSound = nullptr;

    SpriteRendererComponent* closeSettingsBtnSpt = nullptr;
    Texture closeSettingsBtn_Normal{};
    Texture closeSettingsBtn_Hover{};
    ButtonComponent* closeSettingsButton = nullptr;
    SoundComponent* closeSettingsBtnSound = nullptr;

    Entity* settingsMenu = nullptr;
    SpriteRendererComponent* settingsMenuSpt = nullptr;

    Entity* masterVolumeSlider = nullptr;
    SliderComponent* masterVolumeSliderComp = nullptr;
    TextComponent* masterVolumeText = nullptr;

    Entity* bgmVolumeSlider = nullptr;
    SliderComponent* bgmVolumeSliderComp = nullptr;
    TextComponent* bgmVolumeText = nullptr;

    Entity* sfxVolumeSlider = nullptr;
    SliderComponent* sfxVolumeSliderComp = nullptr;
    TextComponent* sfxVolumeText = nullptr;

    Entity* fpsSlider = nullptr;
    SliderComponent* fpsSliderComp = nullptr;
    TextComponent* fpsText = nullptr;

    Entity* vsyncToggle = nullptr;
    SpriteRendererComponent* vsyncToggleSpt = nullptr;
    Texture vsyncToggle_Enabled{};
    Texture vsyncToggle_Disabled{};
    ButtonComponent* vsyncButtonComp = nullptr;
	SoundComponent* vsyncBtnSound = nullptr;

    

    bool isSettingsOpen = false;
    bool settingsVisible = false;
    bool settingsDirty = false;

    // Gamepad navigation. The panel is the one menu a pad cannot reach at all:
    // `ButtonSystem` fires a button only from IsMouseButtonPressed, and the
    // gear that opens this is the only way in. The cadence below is the one
    // every other menu uses, a 0.2 second cooldown between moves.
    enum class SettingsControl { MASTER, BGM, SFX, FPS, VSYNC, CLOSE, COUNT };
    SettingsControl selectedControl = SettingsControl::MASTER;
    bool usingController = false;
    float controllerNavigationCooldown = 0.0f;
    const float controllerNavigationDelay = 0.2f;

    // The four sliders are drawn with a colour and no texture, so a colour is
    // what marks the selected one.
    // Both halves of each slider, because the fill sits on top of the track:
    // recolouring only the track is invisible on a full bar, and recolouring
    // only the fill is invisible on an empty one.
    SpriteRendererComponent* masterTrackSpt = nullptr;
    SpriteRendererComponent* bgmTrackSpt = nullptr;
    SpriteRendererComponent* sfxTrackSpt = nullptr;
    SpriteRendererComponent* fpsTrackSpt = nullptr;
    SpriteRendererComponent* masterFillSpt = nullptr;
    SpriteRendererComponent* bgmFillSpt = nullptr;
    SpriteRendererComponent* sfxFillSpt = nullptr;
    SpriteRendererComponent* fpsFillSpt = nullptr;

    // The two buttons are marked the way every other menu marks a selected
    // button, by growing it 10%. The VSync toggle draws its own texture and
    // ignores the colour, so a colour cannot mark it, and the close button's
    // hover art is written by the mouse handler and would be lost the next
    // time the pointer crossed it.
    TransformComponent* vsyncToggleTransform = nullptr;
    TransformComponent* closeSettingsBtnTransform = nullptr;
    Vec2 vsyncToggleOriginalScale{ 1.f, 1.f };
    Vec2 closeSettingsBtnOriginalScale{ 1.f, 1.f };
    const float buttonScaleIncrease = 1.1f;

    void UpdateControllerNavigation();
    void ShowSelection();
    void ClearSelection();
    void NudgeSelectedSlider(float direction);
    void ActivateSelected();
    SliderComponent* SelectedSlider() const;
public:
    GameSettingsLogic() : GameLogic(nullptr) {}

    GameSettingsLogic(GameLogicComponent* component) : GameLogic(nullptr)
    {
        UNREFERENCED_PARAMETER(component);
    }

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<GameSettingsLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    /****************************************************************
	* @brief Start function for the Game Settings Logic
    ****************************************************************/
    void Start() override;

    /****************************************************************
	* @brief Update function for the Game Settings Logic
    ****************************************************************/
    void Update() override;

    /****************************************************************
	* @brief Show or hide the settings menu.
	* @param state     true to show the panel, false to hide it.
	* @param fromPad   true when a gamepad opened it, which selects the first
	*                  control straight away. Opening it with the mouse leaves
	*                  nothing selected until the pad is used, the same as
	*                  every other menu.
    ****************************************************************/
    void ShowSettings(bool state, bool fromPad = false);

    /****************************************************************
	* @brief Update slider values in real time.
    ****************************************************************/
    void UpdateSliders();

    /****************************************************************
	* @brief Check if the settings menu is visible
    ****************************************************************/
	bool isSettingsVisible() { return settingsVisible; }
};
