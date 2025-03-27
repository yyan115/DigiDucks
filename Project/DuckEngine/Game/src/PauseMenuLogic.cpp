/******************************************************************************/
/*!
\file       PauseMenuLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 4 2025
\brief      Definition of all Pause Menu Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "PauseMenuLogic.h"
#include "GameManager.h"
#include "ProjectSettings.h"

/****************************************************************
* @brief Start function for the Pause Menu Logic
* ****************************************************************/
void PauseMenuLogic::Start()
{
	// Pause Menu
	{
		gamePauseBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
		if (gamePauseBg)
		{
			gamePauseBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseBg->entityID);
			if (gamePauseBgSpt)
			{
				gamePauseBgSpt->isVisible = false;
			}
			gamePauseBgSpt2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gamePauseBg->childEntities[0]->entityID);
		}

		auto gameResumeBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Resume_Btn").get();
		if (gameResumeBtn)
		{
			gameResumeBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameResumeBtn->entityID);
			gameResumeBtn_Normal = AssetManager::GetTextureByName("pause_resumegame");
			gameResumeBtn_Hover = AssetManager::GetTextureByName("pause_resumegame_hover");
			gameResumeButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameResumeBtn->entityID);
			gameResumeBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameResumeBtn->entityID);
			if (gameResumeButton)
			{
				gameResumeBtnSpt->texture = gameResumeBtn_Normal;
				gameResumeButton->onClick = [this]() { 
					if (isPaused) { 
						gameResumeBtnSound->Resume();
						gameResumeBtnSound->Play();
						PauseGame(false);					
					} };
				gameResumeButton->onHover = [this]() {
					gameResumeBtnSound->Resume();
					gameResumeBtnSound->Play(1);
					gameResumeBtnSpt->texture = gameResumeBtn_Hover;
					};
				gameResumeButton->onFinishHover = [this]() { gameResumeBtnSpt->texture = gameResumeBtn_Normal; };
			}
		}

		auto gameExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Btn").get();
		if (gameExitBtn)
		{
			gameExitBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitBtn->entityID);
			gameExitBtn_Normal = AssetManager::GetTextureByName("pause_mainmenu");
			gameExitBtn_Hover = AssetManager::GetTextureByName("pause_mainmenu_hover");
			gameExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitBtn->entityID);
			gameExitBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitBtn->entityID);
			if (gameExitButton)
			{
				gameExitBtnSpt->texture = gameExitBtn_Normal;
				gameExitButton->onClick = [this]() { 
					gameExitBtnSound->Resume();
					gameExitBtnSound->Play();
					ExitConfirm(true); };
				gameExitButton->onHover = [this]() { 
					gameExitBtnSound->Resume();
					gameExitBtnSound->Play(1);
					gameExitBtnSpt->texture = gameExitBtn_Hover; };
				gameExitButton->onFinishHover = [this]() { gameExitBtnSpt->texture = gameExitBtn_Normal; };
			}
		}

		auto gameHTPBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Btn").get();
		if (gameHTPBtn)
		{
			gameHTPBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPBtn->entityID);
			gameHTPBtn_Normal = AssetManager::GetTextureByName("pause_howtoplay");
			gameHTPBtn_Hover = AssetManager::GetTextureByName("pause_howtoplay_hover");
			gameHTPButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBtn->entityID);
			gameHTPBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameHTPBtn->entityID);

			if (gameHTPButton)
			{
				gameHTPBtnSpt->texture = gameHTPBtn_Normal;
				gameHTPButton->onClick = [this]() { 
					if (gameJournal) {
						gameHTPBtnSound->Resume();
						gameHTPBtnSound->Play();
						gameJournalSpt->isVisible = true;

						// Disable Pause Menu Buttons
						DisableButtons(true);
					} };						
				gameHTPButton->onHover = [this]() {  
					gameHTPBtnSound->Resume();
					gameHTPBtnSound->Play(1);
					gameHTPBtnSpt->texture = gameHTPBtn_Hover; };
				gameHTPButton->onFinishHover = [this]() { gameHTPBtnSpt->texture = gameHTPBtn_Normal; };
			}
		}
	}

	// H.T.P Menu
	{
		gameJournal = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlayScreen").get();
		if (gameJournal)
		{
			gameJournalSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameJournal->entityID);
			if (gameJournalSpt)
			{
				gameJournalSpt->isVisible = false;
			}
		}
	}

	// Exit Confirmation
	{
		gameExitCfmBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_Cfm_Bg").get();
		if (gameExitCfmBg)
		{
			gameExitCfmBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitCfmBg->entityID);
			if (gameExitCfmBgSpt)
			{
				gameExitCfmBgSpt->isVisible = false;
			}
		}
	}

	ui = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("UI").get();
	if (ui)
	{
		uiSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(ui->entityID);
	}

	isPaused = false;
	pageNumb = 1;

	PauseGame(false);
}

/****************************************************************
* @brief Update function for the Pause Menu Logic
* ****************************************************************/
void PauseMenuLogic::Update()
{
	if (isPaused)
	{
		UpdateMenuSelection();
	}
}

void PauseMenuLogic::UpdateMenuSelection()
{
	// First, check if any submenus are open - don't allow pause menu navigation if they are
	bool submenusOpen = false;

	// Check if How To Play screen is open
	if (gameJournal && gameJournalSpt && gameJournalSpt->isVisible) {
		submenusOpen = true;
	}

	// Check if Quit confirmation screen is open
	if (gameExitCfmBg && gameExitCfmBgSpt && gameExitCfmBgSpt->isVisible) {
		submenusOpen = true;
	}

	if (submenusOpen && DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1)) 
	{
		if (DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B)) {
			// Close Exit confirmation if it's open
			if (gameExitCfmBg && gameExitCfmBgSpt && gameExitCfmBgSpt->isVisible) 
			{
				ExitConfirm(false); 
			}
		}

		return;
	}

	if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
	{
		if (controllerNavigationCooldown > 0)
		{
			controllerNavigationCooldown -= DuckEngine::PauseDeltaTime();
		}

		float verticalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);
		bool dpadUp = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP);
		bool dpadDown = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN);

		bool hasControllerInput = std::abs(verticalInput) > 0.3f || dpadUp || dpadDown ||
			DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

		if (hasControllerInput && !isUsingController)
		{
			isUsingController = true;
			SelectButton(MenuSelection::RESUME);
			controllerNavigationCooldown = controllerNavigationDelay;
		}

		if (isUsingController)
		{
			if (controllerNavigationCooldown <= 0 && (verticalInput < -0.3f || dpadUp))
			{
				int newSelection = static_cast<int>(currentSelection) - 1;
				if (newSelection < 0)
					newSelection = static_cast<int>(MenuSelection::COUNT) - 1;

				SelectButton(static_cast<MenuSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			else if (controllerNavigationCooldown <= 0 && (verticalInput > 0.3f || dpadDown))
			{
				int newSelection = (static_cast<int>(currentSelection) + 1) % static_cast<int>(MenuSelection::COUNT);
				SelectButton(static_cast<MenuSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			if (DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
			{
				ActivateSelectedButton();
			}
		}
	}
	else
	{
		if (isUsingController)
		{
			DeselectAllButtons();
			isUsingController = false;
		}
	}

	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
		DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_RIGHT))
	{
		if (isUsingController)
		{
			DeselectAllButtons();
			isUsingController = false;
		}
	}
}

void PauseMenuLogic::SelectButton(MenuSelection selection)
{
	// Deselect all buttons first
	DeselectAllButtons();

	// Update current selection
	currentSelection = selection;

	// Apply hover effect to the selected button
	switch (selection)
	{
	case MenuSelection::RESUME:
		gameResumeBtnSpt->texture = gameResumeBtn_Hover;
		gameResumeBtnSound->Play();
		break;

	case MenuSelection::HOW_TO_PLAY:
		gameHTPBtnSpt->texture = gameHTPBtn_Hover;
		gameHTPBtnSound->Play();
		break;

	case MenuSelection::MAIN_MENU:
		gameExitBtnSpt->texture = gameExitBtn_Hover;
		gameExitBtnSound->Play();
		break;

	default:
		break;
	}
}

void PauseMenuLogic::DeselectAllButtons()
{
	gameResumeBtnSpt->texture = gameResumeBtn_Normal;
	gameHTPBtnSpt->texture = gameHTPBtn_Normal;
	gameExitBtnSpt->texture = gameExitBtn_Normal;
}

void PauseMenuLogic::ActivateSelectedButton()
{
	switch (currentSelection)
	{
	case MenuSelection::RESUME:
		if (isPaused) {
			gameResumeBtnSound->Resume();
			gameResumeBtnSound->Play();
			PauseGame(false);
		}
		break;

	case MenuSelection::HOW_TO_PLAY:
		if (gameJournal) {
			gameHTPBtnSound->Resume();
			gameHTPBtnSound->Play();
			gameJournalSpt->isVisible = true;
			DisableButtons(true);
		}
		break;

	case MenuSelection::MAIN_MENU:
		gameExitBtnSound->Resume();
		gameExitBtnSound->Play();
		ExitConfirm(true);
		break;

	default:
		break;
	}
}

/****************************************************************
* @brief FixedUpdate function for the Pause Menu Logic
* ****************************************************************/
void PauseMenuLogic::FixedUpdate()
{

}


/****************************************************************
* @brief Pause the game and display the pause menu.
* @param isPaused - true if the game is paused, false if the game
* is unpaused.
* ****************************************************************/
void PauseMenuLogic::PauseGame(bool state)
{
	isPaused = state;

	std::cout << "PauseGame: " << isPaused << std::endl;

	// Hide Texts
	if (ui)
	{
		uiSprite->isVisible = !state;
	}


	// Show Pause Menu
	if (gamePauseBgSpt)
	{
		gamePauseBgSpt->isVisible = state;
	}

	ExitConfirm(false);
	DuckEngine::isGamePaused = state;
	DuckEngine::PauseGame(state);
}




/****************************************************************
* @brief Change the visibility of the exit confirmation menu.
* ****************************************************************/
void PauseMenuLogic::ExitConfirm(bool state)
{

	if (gameExitCfmBg)
	{
		gameExitCfmBgSpt->isVisible = state;
	}

	if (gamePauseBgSpt2)
	{
		gamePauseBgSpt2->isVisible = !state;
	}
	DisableButtons(state);

}


/****************************************************************
* @brief Function to Enable/Disable the Buttons
*
* @param state - true to disable, false to enable
* ****************************************************************/
void PauseMenuLogic::DisableButtons(bool state)
{
	if (gameResumeButton)
	{
		gameResumeButton->isEnabled = !state;
	}
	if (gameExitButton)
	{
		gameExitButton->isEnabled = !state;
	}
	if (gameHTPButton)
	{
		gameHTPButton->isEnabled = !state;
	}
}

