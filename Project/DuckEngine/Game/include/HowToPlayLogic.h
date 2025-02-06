#pragma once

#include "DuckEngine.h"
#include "GameLogicComponent.h"

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

    int pageNum = 1;  // Tracks the current journal page
};
