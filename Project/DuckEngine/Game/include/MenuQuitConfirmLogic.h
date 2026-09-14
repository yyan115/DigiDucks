/******************************************************************************/
/*!
\file       MenuQuitConfirmLogic.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\brief      Declaration of the main menu's quit confirmation.

            The main menu's EXIT button used to close the window on the click,
            with nothing asked. This puts a confirmation in front of it, and in
            front of the window's own close button, so neither can lose a
            player's progress by accident.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class MenuQuitConfirmLogic : public GameLogic
{
private:
    Entity* quitCfmBg = nullptr;
    SpriteRendererComponent* quitCfmBgSpt = nullptr;
    TextComponent* titleText = nullptr;
    TextComponent* questionText = nullptr;

    ButtonComponent* yesButton = nullptr;
    SpriteRendererComponent* yesBtnSpt = nullptr;
    TransformComponent* yesBtnTransform = nullptr;
    Vec2 yesBtnOriginalScale{ 1.f, 1.f };
    Texture yesBtn_Normal{};
    Texture yesBtn_Hover{};
    SoundComponent* yesBtnSound = nullptr;

    ButtonComponent* noButton = nullptr;
    SpriteRendererComponent* noBtnSpt = nullptr;
    TransformComponent* noBtnTransform = nullptr;
    Vec2 noBtnOriginalScale{ 1.f, 1.f };
    Texture noBtn_Normal{};
    Texture noBtn_Hover{};
    SoundComponent* noBtnSound = nullptr;

    // The same 10% every other selected button in this game grows by.
    const float buttonScaleIncrease = 1.1f;

    // NO is the default, because this dialogue guards a destructive action and
    // the safe answer should be the one a player lands on.
    enum class MenuSelection { YES = 0, NO = 1 };
    MenuSelection currentSelection = MenuSelection::NO;
    bool isUsingController = false;
    float controllerNavigationCooldown = 0.0f;
    const float controllerNavigationDelay = 0.2f;

    void UpdateMenuSelection();
    void SelectButton(MenuSelection selection);
    void DeselectAllButtons();
    void ActivateSelectedButton();
    void SetPartsVisible(bool state);

public:
    MenuQuitConfirmLogic() : GameLogic(nullptr) {}
    MenuQuitConfirmLogic(GameLogicComponent* component) : GameLogic(nullptr)
    {
        UNREFERENCED_PARAMETER(component);
    }
    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<MenuQuitConfirmLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    void Start() override;
    void Update() override;
    void FixedUpdate() override {}

    /*!
    \brief  Show or hide the confirmation. Showing it selects NO, so a player
            arriving on a gamepad is on the safe answer.
    */
    void Show(bool state);
    bool isShowing() const;
};
