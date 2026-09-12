/******************************************************************************/
/*!
\file       HowToPlayLogic.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu, 
\date       Feb 6 2025
\brief      Definition of all Chopping Board Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
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
	if (mainMenuScreen) {
        mainMenuScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID);
        mainMenu = DuckEngine::DUCKENGINE_SceneManager.GetScene<MainMenu>("MainMenu").get();
	}

	auto pauseMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
    if (pauseMenu)
    {
        pauseMenuLogic = GameLogicManager::GetLogicForEntity<PauseMenuLogic>(pauseMenu->entityID);
    }

    // Set initial page
    pageNum = 1;
	lastGamepadConnected = DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1);
    UpdateJournalPage();

    // Button interactions
    if (ExitButton) {
        auto back = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(ExitButton->entityID);
        back->onClick = [this, SFX]() {
			SFX->Play();
            howToPlayScreenSpriteRenderer->isVisible = false;
            if (mainMenuScreenSpriteRenderer) mainMenuScreenSpriteRenderer->isVisible = true;
            if (pauseMenuLogic) pauseMenuLogic->DisableButtons(false);
            };
    }

    if (NextButton) {
        auto next = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(NextButton->entityID);
        next->onClick = [this, SFX]() {
            SFX->Play();
            if (pageNum < 4) {
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

	if (pauseMenuLogic) 
    {
		pauseMenuLogic->DisableButtons(true);
	}

	pageNum = 1;
	UpdateJournalPage();
}

void HowToPlayLogic::UpdateJournalPage()
{
    if (JournalSprite) {
		const bool showControllerInstructions = pageNum == 1 &&
			DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1);
		std::string textureName = showControllerInstructions
			? "Resources/Sprites/HowToPlay/journal_1_controller.png"
			: "Resources/Sprites/HowToPlay/journal_" + std::to_string(pageNum) + ".png";
        JournalSprite->texture = *AssetManager::GetTexture(textureName).get();
    }
}

void HowToPlayLogic::Update()
{
	const bool gamepadConnected = DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1);
	if (pageNum == 1 && gamepadConnected != lastGamepadConnected)
	{
		lastGamepadConnected = gamepadConnected;
		UpdateJournalPage();
	}

	// Reduce cooldown timer if it's active
	if (controllerNavigationCooldown > 0.0f) 
    {
		controllerNavigationCooldown -= DuckEngine::PauseDeltaTime();
	}

	if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B))
	{
		// Return to main menu
		if (howToPlayScreenSpriteRenderer->isVisible)
		{
			howToPlayScreenSpriteRenderer->isVisible = false;
			if (mainMenuScreenSpriteRenderer) mainMenuScreenSpriteRenderer->isVisible = true;
			if (pauseMenuLogic) pauseMenuLogic->DisableButtons(false);
		}
	}

	// Add joystick navigation for page turning with cooldown
	if (howToPlayScreenSpriteRenderer->isVisible && controllerNavigationCooldown <= 0.0f)
	{
		// Right joystick or d-pad for next page
		if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT) ||
			DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X) > 0.5f ||
			DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_X) > 0.5f)
		{
			if (pageNum < 4) {
				pageNum++;
				UpdateJournalPage();
				controllerNavigationCooldown = controllerNavigationDelay; // Set cooldown after action
			}
		}
		// Left joystick or d-pad for previous page
		else if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT) ||
			DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X) < -0.5f ||
			DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_X) < -0.5f)
		{
			if (pageNum > 1) {
				pageNum--;
				UpdateJournalPage();
				controllerNavigationCooldown = controllerNavigationDelay; // Set cooldown after action
			}
		}
	}
}

void HowToPlayLogic::FixedUpdate() {}
