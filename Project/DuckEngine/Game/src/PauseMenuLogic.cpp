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

		auto gameExitYesBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_Yes_Btn").get();
		if (gameExitYesBtn)
		{
			gameExitYesBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitYesBtn->entityID);
			gameExitYesBtn_Normal = AssetManager::GetTextureByName("exit_yes");
			gameExitYesBtn_Hover = AssetManager::GetTextureByName("exit_yes_hover");
			gameExitYesButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitYesBtn->entityID);
			gameExitYesBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitYesBtn->entityID);
			if (gameExitYesButton)
			{
				gameExitYesButton->onClick = [this]() {
					gameExitYesBtnSound->Resume();
					gameExitYesBtnSound->Play();
					// need to transition
					GameManager::SetActiveScene("MainMenu"); };
				gameExitYesButton->onHover = [this]() { 
					gameExitYesBtnSound->Resume();
					gameExitYesBtnSound->Play(1);
					gameExitYesBtnSpt->texture = gameExitYesBtn_Hover; };
				gameExitYesButton->onFinishHover = [this]() { gameExitYesBtnSpt->texture = gameExitYesBtn_Normal; };
			}
		}

		auto gameExitNoBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_No_Btn").get();
		if (gameExitNoBtn)
		{
			gameExitNoBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitNoBtn->entityID);
			gameExitNoBtn_Normal = AssetManager::GetTextureByName("exit_no");
			gameExitNoBtn_Hover = AssetManager::GetTextureByName("exit_no_hover");
			gameExitNoButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitNoBtn->entityID);
			gameExitNoBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitNoBtn->entityID);
			if (gameExitNoButton)
			{
				gameExitNoButton->onClick = [this]() { 
					gameExitNoBtnSound->Resume();
					gameExitNoBtnSound->Play();
					PauseGame(true); };
				gameExitNoButton->onHover = [this]() { 
					gameExitNoBtnSound->Resume();
					gameExitNoBtnSound->Play(1);
					gameExitNoBtnSpt->texture = gameExitNoBtn_Hover; };
				gameExitNoButton->onFinishHover = [this]() { gameExitNoBtnSpt->texture = gameExitNoBtn_Normal; };
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