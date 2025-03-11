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

    

    bool isSettingsOpen = false;
    bool settingsVisible = false;
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
    ****************************************************************/
    void ShowSettings(bool state);

    /****************************************************************
	* @brief Update slider values in real time.
    ****************************************************************/
    void UpdateSliders();

    /****************************************************************
	* @brief Check if the settings menu is visible
    ****************************************************************/
	bool isSettingsVisible() { return settingsVisible; }
};
