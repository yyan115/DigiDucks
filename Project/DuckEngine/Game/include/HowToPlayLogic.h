/******************************************************************************/
/*!
\file       HowToPlayLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
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

    //void Show();  // Call this to show the HowToPlay screen

    // The journal's pages are two books in one: how to play on pages one to
    // four, and the credits on five and six. Each is opened on its own first
    // page, and its arrows and the pad turn pages only within it, so How To
    // Play never runs on into the credits.
    static const int kFirstInstructionsPage = 1;
    static const int kLastInstructionsPage = 4;
    static const int kFirstCreditsPage = 5;
    static const int kLastCreditsPage = 6;

    /*!
    \brief  Show the journal open at firstPage, turning no further than
            lastPage either way.
    */
    void Open(int first, int last);

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

	float controllerNavigationCooldown = 0.0f;
	const float controllerNavigationDelay = 0.3f;

    int pageNum = 1;  // Tracks the current journal page

    // Pages five and six are the credits: the team on one, the game and the
    // DigiPen and FMOD notices on the other. They are journal pages because
    // the journal's pages carry their words in the image, so the credits need
    // no entity in any scene; the main menu's CREDITS opens the journal on
    // them, and How To Play stops before them.
    int firstPage = kFirstInstructionsPage;
    int lastPage = kLastInstructionsPage;
};
