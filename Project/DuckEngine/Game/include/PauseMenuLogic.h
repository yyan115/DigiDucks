/******************************************************************************/
/*!
\file       PauseMenuLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       March 4 2025
\brief      Declartion of all Pause Menu Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class PauseMenuLogic : public GameLogic
{
private:
	//Pause Menu
	Entity* gamePauseBg = nullptr;
	SpriteRendererComponent* gamePauseBgSpt = nullptr;
	SpriteRendererComponent* gamePauseBgSpt2 = nullptr;

	// Resume Button
	ButtonComponent* gameResumeButton = nullptr;
	SpriteRendererComponent* gameResumeBtnSpt = nullptr;
	Texture gameResumeBtn_Normal{};
	Texture gameResumeBtn_Hover{};
	SoundComponent* gameResumeBtnSound = nullptr;

	// Exit Button
	ButtonComponent* gameExitButton = nullptr;
	SpriteRendererComponent* gameExitBtnSpt = nullptr;
	Texture gameExitBtn_Normal{};
	Texture gameExitBtn_Hover{};
	SoundComponent* gameExitBtnSound = nullptr;

	// How To Play Button
	ButtonComponent* gameHTPButton = nullptr;
	SpriteRendererComponent* gameHTPBtnSpt = nullptr;
	Texture gameHTPBtn_Normal{};
	Texture gameHTPBtn_Hover{};
	SoundComponent* gameHTPBtnSound = nullptr;

	// Main Menu Button
	ButtonComponent* gameMainMenuButton = nullptr;
	SpriteRendererComponent* gameMainMenuBtnSpt = nullptr;
	Texture gameMainMenuBtn_Normal{};
	Texture gameMainMenuBtn_Hover{};
	SoundComponent* gameMainMenuBtnSound = nullptr;

	// HTP Menu
	Entity* gameJournal = nullptr;
	SpriteRendererComponent* gameJournalSpt = nullptr;

	// Exit Confirm
	Entity* gameExitCfmBg = nullptr;
	SpriteRendererComponent* gameExitCfmBgSpt = nullptr;
	ButtonComponent* gameExitYesButton = nullptr;
	SpriteRendererComponent* gameExitYesBtnSpt = nullptr;
	Texture gameExitYesBtn_Normal{};
	Texture gameExitYesBtn_Hover{};
	SoundComponent* gameExitYesBtnSound = nullptr;

	ButtonComponent* gameExitNoButton = nullptr;
	SpriteRendererComponent* gameExitNoBtnSpt = nullptr;
	Texture gameExitNoBtn_Normal{};
	Texture gameExitNoBtn_Hover{};
	SoundComponent* gameExitNoBtnSound = nullptr;

	// UI
	Entity* ui = nullptr;
	SpriteRendererComponent* uiSprite = nullptr;
	SoundComponent* PauseMenuSound = nullptr;
	// Gamepad navigation
	enum class MenuSelection 
	{
		RESUME = 0,
		HOW_TO_PLAY = 1,
		MAIN_MENU = 2,
		COUNT
	};

	MenuSelection currentSelection = MenuSelection::RESUME;
	bool isUsingController = false;
	float controllerNavigationCooldown = 0.0f;
	const float controllerNavigationDelay = 0.2f;

	void UpdateMenuSelection();
	void SelectButton(MenuSelection selection);
	void DeselectAllButtons();
	void ActivateSelectedButton();


public:
	// HTP Page
	int pageNumb = 1;
	bool isPaused = false;

	PauseMenuLogic() : GameLogic(nullptr) {}

	PauseMenuLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<PauseMenuLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	/****************************************************************
	* @brief Start function for the Pause Menu Logic
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Pause Menu Logic
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for the Pause Menu Logic
	* ****************************************************************/
	void FixedUpdate() override;

	/****************************************************************
	* @brief Function to Pause/Unpause the game
	* ****************************************************************/
	void PauseGame(bool state);

	/****************************************************************
	* @brief Function to Enable/Disable the Exit Confirmation menu
	* 
	* @param state - true to disable, false to enable
	* ****************************************************************/
	void ExitConfirm(bool state);

	/****************************************************************
	* @brief Function to Enable/Disable the Buttons
	* 
	* @param state - true to disable, false to enable
	* ****************************************************************/
	void DisableButtons(bool state);

	void playPauseSound();
};
