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
    ButtonComponent* vsyncButtonComp = nullptr;

    

    bool isSettingsOpen = false;

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

    void Start() override;
    void Update() override;
    void ShowSettings(bool state);
    void UpdateSliders();
};
