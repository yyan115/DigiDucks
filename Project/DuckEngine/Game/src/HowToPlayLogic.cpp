#include "HowToPlayLogic.h"
#include "MainMenu.h"

void HowToPlayLogic::Start()
{
    // Get UI elements
    howToPlayScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
    mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();

    ExitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTPExit").get();
    NextButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTPNext").get();
    BackButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTPBack").get();
    JournalPage = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTPJournal").get();
    SoundComponent* SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(ExitButton->entityID);
    if (JournalPage) {
        JournalSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(JournalPage->entityID);
    }

    howToPlayScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(howToPlayScreen->entityID);
    mainMenuScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID);

    mainMenu = DuckEngine::DUCKENGINE_SceneManager.GetScene<MainMenu>("MainMenu").get();

    // Set initial page
    pageNum = 1;
    UpdateJournalPage();

    // Button interactions
    if (ExitButton) {
        auto back = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(ExitButton->entityID);
        back->onClick = [this, SFX]() {
			SFX->Play();
            howToPlayScreenSpriteRenderer->isVisible = false;
            mainMenuScreenSpriteRenderer->isVisible = true;
            };
    }

    if (NextButton) {
        auto next = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(NextButton->entityID);
        next->onClick = [this, SFX]() {
            SFX->Play();
            if (pageNum < 3) {
                pageNum++;
                UpdateJournalPage();
            }
            };
    }

    if (BackButton) {
        auto backBtn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(BackButton->entityID);
        backBtn->onClick = [this, SFX]() {
            SFX->Play();
            if (pageNum > 1) {
                pageNum--;
                UpdateJournalPage();
            }
            };
    }
}

void HowToPlayLogic::UpdateJournalPage()
{
    if (JournalSprite) {
        std::string textureName = "Resources/Sprites/HowToPlay/journal_" + std::to_string(pageNum) + ".png";
        JournalSprite->texture = *AssetManager::GetTexture(textureName).get();
    }
}

void HowToPlayLogic::Update() {}

void HowToPlayLogic::FixedUpdate() {}

