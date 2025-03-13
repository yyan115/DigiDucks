/******************************************************************************/
/*!
\file       HowToPlayLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223 (50%)
\author
\par        h.yonghengernestt@digipen.edu,
\date       Feb 6 2025
\brief      Definition of all Chopping Board Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "DuckEngine.h"
#include "GameLogicComponent.h"
#include "PauseMenuLogic.h"

class MainMenu;

class HowToPlayLogic : public GameLogic
{
public:
    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<HowToPlayLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    void Show();  // Call this to show the HowToPlay screen

private:
    void UpdateJournalPage();  // Updates the journal page texture

    Entity* howToPlayScreen = nullptr;
    MainMenu* mainMenu = nullptr;
    Entity* mainMenuScreen = nullptr;
    Entity* ExitButton = nullptr;
    Entity* NextButton = nullptr;
    Entity* BackButton = nullptr;
    Entity* JournalPage = nullptr;

    SpriteRendererComponent* howToPlayScreenSpriteRenderer = nullptr;
    SpriteRendererComponent* mainMenuScreenSpriteRenderer = nullptr;
    SpriteRendererComponent* JournalSprite = nullptr;

	std::shared_ptr<PauseMenuLogic> pauseMenuLogic = nullptr;

    int pageNum = 1;  // Tracks the current journal page
};
