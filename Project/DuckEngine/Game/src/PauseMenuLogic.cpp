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
		}

		auto gameResumeBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Resume_Btn").get();
		if (gameResumeBtn)
		{
			gameResumeBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameResumeBtn->entityID);
			gameResumeBtn_Normal = AssetManager::GetTextureByName("pause_resumegame");
			gameResumeBtn_Hover = AssetManager::GetTextureByName("pause_resumegame_hover");
			gameResumeButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameResumeBtn->entityID);
			if (gameResumeButton)
			{
				gameResumeButton->onClick = [this]() { if (isPaused) { PauseGame(false); } };
				gameResumeButton->onHover = [this]() { gameResumeBtnSpt->texture = gameResumeBtn_Hover; };
				gameResumeButton->onFinishHover = [this]() { gameResumeBtnSpt->texture = gameResumeBtn_Normal; };
			}
		}

		auto gameExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Btn").get();
		if (gameExitBtn)
		{
			gameExitBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitBtn->entityID);
			gameExitBtn_Normal = AssetManager::GetTextureByName("pause_quitgame");
			gameExitBtn_Hover = AssetManager::GetTextureByName("pause_quitgame_hover");
			gameExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitBtn->entityID);
			if (gameExitButton)
			{
				gameExitButton->onClick = [this]() { ExitConfirm(true); };
				gameExitButton->onHover = [this]() { gameExitBtnSpt->texture = gameExitBtn_Hover; };
				gameExitButton->onFinishHover = [this]() { gameExitBtnSpt->texture = gameExitBtn_Normal; };
			}
		}

		/*auto gameHTPBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Btn").get();
		if (gameHTPBtn)
		{
			gameHTPBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameHTPBtn->entityID);
			gameHTPBtn_Normal = AssetManager::GetTextureByName("pause_howtoplay");
			gameHTPBtn_Hover = AssetManager::GetTextureByName("pause_howtoplay_hover");
			gameHTPButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBtn->entityID);
			if (gameHTPButton)
			{
				gameHTPButton->onClick = [this]() { HTPShow(true); };
				gameHTPButton->onHover = [this]() {  gameHTPBtnSpt->texture = gameHTPBtn_Hover; };
				gameHTPButton->onFinishHover = [this]() { gameHTPBtnSpt->texture = gameHTPBtn_Normal; };
			}
		}*/

		// Settings Button
		auto gameSettingsBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Btn").get();
		if (gameSettingsBtn)
		{
			gameSettingsBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSettingsBtn->entityID);
			gameHTPBtn_Normal = AssetManager::GetTextureByName("pause_howtoplay");
			gameHTPBtn_Hover = AssetManager::GetTextureByName("pause_howtoplay_hover");
			gameSettingsButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSettingsBtn->entityID);
			if (gameSettingsButton) { 
				gameSettingsButton->onClick = [this]() { ShowSettings(true); }; 
				gameSettingsButton->onHover = [this]() { gameSettingsBtnSpt->texture = gameHTPBtn_Hover; };
				gameSettingsButton->onFinishHover = [this]() { gameSettingsBtnSpt->texture = gameHTPBtn_Normal; };
			}
		}
	}

	// H.T.P Menu
	{
		gameJournal = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Journal").get();
		if (gameJournal)
		{
			gameJournalSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameJournal->entityID);
			if (gameJournalSpt)
			{
				gameJournalSpt->isVisible = false;
			}
		}

		gameHTPExitBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Exit_Btn").get();
		if (gameHTPExitBtn)
		{
			gameHTPExitButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPExitBtn->entityID);
			if (gameHTPExitButton)
			{
				gameHTPExitButton->onClick = [this]() { std::cout << "EXIT\n"; HTPShow(false); };
			}
		}

		gameHTPBackBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Back_Btn").get();
		if (gameHTPBackBtn)
		{
			gameHTPBackButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPBackBtn->entityID);
			if (gameHTPBackButton)
			{
				gameHTPBackButton->onClick = [this]() { std::cout << "BACK\n"; if (pageNumb > 1) { pageNumb--; changePage(); } };
			}
		}

		gameHTPNextBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HTP_Next_Btn").get();
		if (gameHTPNextBtn)
		{
			gameHTPNextButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameHTPNextBtn->entityID);
			if (gameHTPNextButton)
			{
				gameHTPNextButton->onClick = [this]() { std::cout << "NEXT\n"; if (pageNumb < 3) { pageNumb++; changePage(); } };
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
			if (gameExitYesButton)
			{
				gameExitYesButton->onClick = []() { GameManager::DuckEngine.CloseWindow(); };
				gameExitYesButton->onHover = [this]() { gameExitYesBtnSpt->texture = gameExitYesBtn_Hover; };
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
			if (gameExitNoButton)
			{
				gameExitNoButton->onClick = [this]() { ExitConfirm(false); };
				gameExitNoButton->onHover = [this]() { gameExitNoBtnSpt->texture = gameExitNoBtn_Hover; };
				gameExitNoButton->onFinishHover = [this]() { gameExitNoBtnSpt->texture = gameExitNoBtn_Normal; };
			}
		}
	}

	

	// Settings Background
	settingsBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Menu").get();
	if (settingsBg)
	{
		settingsBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(settingsBg->entityID);
		if (settingsBgSpt) { settingsBgSpt->isVisible = false; }
	}

	// Volume Sliders
	masterVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Slider").get();
	if (masterVolumeSlider)
	{
		masterVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(masterVolumeSlider->entityID);
		if (masterVolumeSliderComp) { 
			masterVolumeSliderComp->currentValue = ProjectSettings::GetMasterVolume();
			masterVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MasterVolume_Text").get()->entityID);
		}
	}

	bgmVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Slider").get();
	if (bgmVolumeSlider)
	{
		bgmVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(bgmVolumeSlider->entityID);
		if (bgmVolumeSliderComp) { 
			bgmVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("BGM");
			bgmVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BGMVolume_Text").get()->entityID);
		}
	}

	sfxVolumeSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Slider").get();
	if (sfxVolumeSlider)
	{
		sfxVolumeSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(sfxVolumeSlider->entityID);
		if (sfxVolumeSliderComp) { 
			sfxVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("SFX");
			sfxVolumeText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXVolume_Text").get()->entityID);
		}
	}

	// FPS Slider
	fpsSlider = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_Slider").get();
	if (fpsSlider)
	{
		fpsSliderComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(fpsSlider->entityID);
		if (fpsSliderComp) { 
			fpsSliderComp->currentValue = static_cast<float>(ProjectSettings::GetTargetFPS());
			fpsText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPSTarget_Text").get()->entityID);
		}
	}

	// VSync Toggle
	vsyncToggle = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("VSync_Toggle").get();
	if (vsyncToggle)
	{
		vsyncButtonComp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(vsyncToggle->entityID);
		if (vsyncButtonComp)
		{
			vsyncButtonComp->onClick = []() {
				bool vsync = !ProjectSettings::GetUseVSync();
				ProjectSettings::SetUseVSync(vsync);
				std::cout << "VSync: " << (vsync ? "Enabled" : "Disabled") << std::endl;
				};
		}
	}

	ui = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("UI").get();
	if (ui)
	{
		uiSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(ui->entityID);
	}

	isPaused = false;
	pageNumb = 1;
	isSettingsOpen = false;

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

	HTPShow(false);
	ExitConfirm(false);
	ShowSettings(false);
	DuckEngine::SetPaused(state);
}


/****************************************************************
* @brief Display the How To Play menu.
* ****************************************************************/
void PauseMenuLogic::HTPShow(bool state)
{
	if (gameJournalSpt)
	{
		gameJournalSpt->isVisible = state;
	}

	// Disable Quit and Resume Btn
	if (gameResumeButton)
	{
		gameResumeButton->isEnabled = !state;
	}
	if (gameExitButton)
	{
		gameExitButton->isEnabled = !state;
	}
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

	// Disable HTP  and Quit Btn
	if (gameHTPButton)
	{
		gameHTPButton->isEnabled = !state;
	}
	if (gameResumeButton)
	{
		gameResumeButton->isEnabled = !state;
	}

}


/****************************************************************
* @brief Change the current page of the How To Play menu.
* ****************************************************************/
void PauseMenuLogic::changePage()
{
	switch (pageNumb)
	{
	case 1:
		if (gameJournalSpt)
			gameJournalSpt->texture = AssetManager::GetTextureByName("journal_1");
		break;
	case 2:
		if (gameJournalSpt)
			gameJournalSpt->texture = AssetManager::GetTextureByName("journal_2");
		break;
	case 3:
		if (gameJournalSpt)
			gameJournalSpt->texture = AssetManager::GetTextureByName("journal_3");
		break;
	default:
		break;
	};
}

void PauseMenuLogic::ShowSettings(bool state)
{
	isSettingsOpen = state;
	if (settingsBgSpt) { settingsBgSpt->isVisible = state; }

	if (masterVolumeSliderComp)
	{
		masterVolumeSliderComp->currentValue = ProjectSettings::GetMasterVolume();
		masterVolumeText->text = std::to_string(static_cast<int>(masterVolumeSliderComp->currentValue * 100));
	}

	if (bgmVolumeSliderComp)
	{
		bgmVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("BGM");
		bgmVolumeText->text = std::to_string(static_cast<int>(bgmVolumeSliderComp->currentValue * 100));
	}

	if (sfxVolumeSliderComp)
	{
		sfxVolumeSliderComp->currentValue = ProjectSettings::GetVolumeCategory("SFX");
		sfxVolumeText->text = std::to_string(static_cast<int>(sfxVolumeSliderComp->currentValue * 100));
	}

	if (fpsSliderComp)
	{
		fpsSliderComp->currentValue = static_cast<float>(ProjectSettings::GetTargetFPS());
		fpsText->text = std::to_string(static_cast<int>(fpsSliderComp->currentValue));
	}

	// Disable Quit and Resume Btn
	if (gameResumeButton)
	{
		gameResumeButton->isEnabled = !state;
	}
	if (gameExitButton)
	{
		gameExitButton->isEnabled = !state;
	}
}