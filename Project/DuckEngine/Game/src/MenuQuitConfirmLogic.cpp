/******************************************************************************/
/*!
\file       MenuQuitConfirmLogic.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\brief      The main menu's quit confirmation.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "MenuQuitConfirmLogic.h"
#include "GameManager.h"
#include "AssetManager.h"
#include "WindowManager.h"

void MenuQuitConfirmLogic::Start()
{
    quitCfmBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Cfm_Bg").get();
    if (quitCfmBg)
    {
        quitCfmBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(quitCfmBg->entityID);
    }

    if (auto title = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Title"))
    {
        titleText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(title->entityID);
    }
    if (auto question = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Question"))
    {
        questionText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(question->entityID);
    }

    auto yesBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Yes_Btn").get();
    if (yesBtn)
    {
        yesBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(yesBtn->entityID);
        yesBtnTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(yesBtn->entityID);
        if (yesBtnTransform) { yesBtnOriginalScale = yesBtnTransform->scale; }
        yesBtn_Normal = AssetManager::GetTextureByName("exit_yes");
        yesBtn_Hover = AssetManager::GetTextureByName("exit_yes_hover");
        yesButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(yesBtn->entityID);
        yesBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(yesBtn->entityID);
        if (yesButton)
        {
            yesButton->onClick = [this]() {
                if (yesBtnSound) { yesBtnSound->Play(); }
                GameManager::Engine.CloseWindow();
                };
            yesButton->onHover = [this]() {
                if (yesBtnSound) { yesBtnSound->Play(1); }
                if (yesBtnSpt) { yesBtnSpt->texture = yesBtn_Hover; }
                if (yesBtnTransform) { yesBtnTransform->scale = yesBtnOriginalScale * buttonScaleIncrease; }
                };
            yesButton->onFinishHover = [this]() {
                if (yesBtnSpt) { yesBtnSpt->texture = yesBtn_Normal; }
                if (yesBtnTransform) { yesBtnTransform->scale = yesBtnOriginalScale; }
                };
        }
    }

    auto noBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_No_Btn").get();
    if (noBtn)
    {
        noBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(noBtn->entityID);
        noBtnTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(noBtn->entityID);
        if (noBtnTransform) { noBtnOriginalScale = noBtnTransform->scale; }
        noBtn_Normal = AssetManager::GetTextureByName("exit_no");
        noBtn_Hover = AssetManager::GetTextureByName("exit_no_hover");
        noButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(noBtn->entityID);
        noBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(noBtn->entityID);
        if (noButton)
        {
            noButton->onClick = [this]() {
                if (noBtnSound) { noBtnSound->Play(); }
                Show(false);
                };
            noButton->onHover = [this]() {
                if (noBtnSound) { noBtnSound->Play(1); }
                if (noBtnSpt) { noBtnSpt->texture = noBtn_Hover; }
                if (noBtnTransform) { noBtnTransform->scale = noBtnOriginalScale * buttonScaleIncrease; }
                };
            noButton->onFinishHover = [this]() {
                if (noBtnSpt) { noBtnSpt->texture = noBtn_Normal; }
                if (noBtnTransform) { noBtnTransform->scale = noBtnOriginalScale; }
                };
        }
    }

    Show(false);
}

void MenuQuitConfirmLogic::SetPartsVisible(bool state)
{
    // The panel, its two buttons and its two lines of text are separate
    // entities, so each has to be told. The buttons are disabled as well as
    // hidden, because ButtonSystem hit-tests a button wherever it is.
    if (quitCfmBgSpt) { quitCfmBgSpt->isVisible = state; }
    if (yesBtnSpt) { yesBtnSpt->isVisible = state; }
    if (noBtnSpt) { noBtnSpt->isVisible = state; }
    if (yesButton) { yesButton->isEnabled = state; }
    if (noButton) { noButton->isEnabled = state; }
    if (titleText) { titleText->isEnabled = state; }
    if (questionText) { questionText->isEnabled = state; }
}

void MenuQuitConfirmLogic::TakeOver()
{
    if (holdingInput) { return; }

    otherButtons.clear();
    for (const auto& [entityId, component] :
        DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
    {
        auto* button = static_cast<ButtonComponent*>(component.get());
        if (!button || button == yesButton || button == noButton) { continue; }
        otherButtons.emplace_back(entityId, button->isEnabled);
        button->isEnabled = false;
    }

    // Paused only if nothing had paused it already, so answering NO over the
    // pause menu goes back to the pause menu rather than into the level.
    pausedTheGame = !DuckEngine::isGamePaused;
    if (pausedTheGame)
    {
        DuckEngine::PauseGame(true);
        DuckEngine::isGamePaused = true;
    }

    DuckEngine_Input::CaptureInput(true);
    holdingInput = true;
}

void MenuQuitConfirmLogic::HandBack()
{
    if (!holdingInput) { return; }

    for (const auto& [entityId, wasEnabled] : otherButtons)
    {
        if (auto* button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(entityId))
        {
            button->isEnabled = wasEnabled;
        }
    }
    otherButtons.clear();

    if (pausedTheGame)
    {
        DuckEngine::isGamePaused = false;
        DuckEngine::PauseGame(false);
        pausedTheGame = false;
    }

    // The input stays held until the next frame. The key or click that
    // answered NO is still a fresh press for the rest of this frame, and
    // releasing it now let the pause menu, which updates later in the same
    // frame, read that Escape as its own and open over the level.
    releaseInputNextFrame = true;
    holdingInput = false;
}

void MenuQuitConfirmLogic::Show(bool state)
{
    SetPartsVisible(state);
    if (state) { TakeOver(); } else { HandBack(); }
    if (state)
    {
        // NO every time it opens, so a player who mashes A does not quit.
        currentSelection = MenuSelection::NO;
        isUsingController = false;
        controllerNavigationCooldown = controllerNavigationDelay;
    }
    DeselectAllButtons();
}

bool MenuQuitConfirmLogic::isShowing() const
{
    return quitCfmBgSpt && quitCfmBgSpt->isVisible;
}

void MenuQuitConfirmLogic::Update()
{
    if (releaseInputNextFrame)
    {
        DuckEngine_Input::CaptureInput(false);
        releaseInputNextFrame = false;
    }

    // The window's own close request, Alt+F4 or its close button, opens this
    // dialogue on every screen that has one. Asking again while it is already
    // open is the player insisting, so the second one closes the game.
    // WindowManager holds the request for a few frames and then closes
    // anyway, so a screen without this dialogue can never trap the player.
    if (WindowManager::TakeCloseRequest())
    {
        if (isShowing())
        {
            GameManager::Engine.CloseWindow();
        }
        else
        {
            Show(true);
        }
        return;
    }

    if (!isShowing()) { return; }

    // Escape answers NO, the same as the button, so the dialogue is never a
    // dead end for a player who reached it by accident.
    if (DuckEngine_Input::Direct::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE))
    {
        if (noBtnSound) { noBtnSound->Play(); }
        Show(false);
        return;
    }

    UpdateMenuSelection();
}

void MenuQuitConfirmLogic::UpdateMenuSelection()
{
    if (!DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
    {
        if (isUsingController)
        {
            isUsingController = false;
            DeselectAllButtons();
        }
        return;
    }

    if (controllerNavigationCooldown > 0.f)
    {
        controllerNavigationCooldown -= DuckEngine::PauseDeltaTime();
    }

    const float horizontal = DuckEngine_Input::Direct::GetMenuAxisHorizontal(DuckEngine_Input::GAMEPAD_1);
    const bool left = DuckEngine_Input::Direct::IsGamepadButtonDown(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT) || horizontal < -0.3f;
    const bool right = DuckEngine_Input::Direct::IsGamepadButtonDown(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT) || horizontal > 0.3f;
    const bool accept = DuckEngine_Input::Direct::IsGamepadButtonPressed(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);
    const bool cancel = DuckEngine_Input::Direct::IsGamepadButtonPressed(
        DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B);

    // B answers NO whether or not the pad has been used yet, so cancelling is
    // never spent on waking the selection up.
    if (cancel)
    {
        if (noBtnSound) { noBtnSound->Play(); }
        Show(false);
        return;
    }

    if (!isUsingController && (left || right || accept))
    {
        isUsingController = true;
        SelectButton(currentSelection);
        controllerNavigationCooldown = controllerNavigationDelay;
        return;
    }
    if (!isUsingController) { return; }

    if (controllerNavigationCooldown > 0.f) { return; }

    if (left || right)
    {
        SelectButton(currentSelection == MenuSelection::YES
                     ? MenuSelection::NO : MenuSelection::YES);
        controllerNavigationCooldown = controllerNavigationDelay;
    }
    else if (accept)
    {
        ActivateSelectedButton();
        controllerNavigationCooldown = controllerNavigationDelay;
    }
}

void MenuQuitConfirmLogic::SelectButton(MenuSelection selection)
{
    DeselectAllButtons();
    currentSelection = selection;
    if (selection == MenuSelection::YES && yesBtnTransform)
    {
        yesBtnTransform->scale = yesBtnOriginalScale * buttonScaleIncrease;
        if (yesBtnSpt) { yesBtnSpt->texture = yesBtn_Hover; }
    }
    else if (selection == MenuSelection::NO && noBtnTransform)
    {
        noBtnTransform->scale = noBtnOriginalScale * buttonScaleIncrease;
        if (noBtnSpt) { noBtnSpt->texture = noBtn_Hover; }
    }
}

void MenuQuitConfirmLogic::DeselectAllButtons()
{
    if (yesBtnTransform) { yesBtnTransform->scale = yesBtnOriginalScale; }
    if (noBtnTransform) { noBtnTransform->scale = noBtnOriginalScale; }
    if (yesBtnSpt) { yesBtnSpt->texture = yesBtn_Normal; }
    if (noBtnSpt) { noBtnSpt->texture = noBtn_Normal; }
}

void MenuQuitConfirmLogic::ActivateSelectedButton()
{
    if (currentSelection == MenuSelection::YES && yesButton && yesButton->onClick)
    {
        yesButton->onClick();
    }
    else if (currentSelection == MenuSelection::NO && noButton && noButton->onClick)
    {
        noButton->onClick();
    }
}
