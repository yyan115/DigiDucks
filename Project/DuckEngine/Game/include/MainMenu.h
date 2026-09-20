/******************************************************************************/
/*!
\file       MainMenu.h
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Definition of the MainMenu class, it loads up the screen to show the instructions on how to play the game whenever it is triggered.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "Scene.h"
#include "DuckEngine.h"
#include "LevelSelectScreenLogic.h"
#include "MenuQuitConfirmLogic.h"
#include "GameSettingsLogic.h"

class MainMenu : public Scene {
public:
	/****************************************************************
	* @brief Load all necessary resources for the scene.
	* This function is called before the scene starts.
	* ****************************************************************/
	void Load() override;

	/****************************************************************
   * @brief Starts the scene, initializing game objects and setting up the initial state.
   * This function is called when the scene begins running.
   * ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Updates the scene logic each frame, including processing input and updating game objects.
	* This function is called every frame.
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Performs any operations after the main update logic, such as cleanup or post-processing.
	* This function is called every frame, after the Update() method.
	* ****************************************************************/
	void PostUpdate() override;

	/****************************************************************
	* @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
	* ****************************************************************/
	void Exit() override;

	/****************************************************************
	* @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
	* ****************************************************************/
	void Unload() override;

	void OnPlayButtonClicked(std::string sceneName);

private:
	SoundComponent* menusound = nullptr;

	Entity* levelSelectScreen = nullptr;
	Entity* mainMenuScreen = nullptr;
	Entity* HTPScreen = nullptr;

	Entity* StartButton = nullptr;
	Entity* QuitButton = nullptr;
	Entity* HtpButton = nullptr;
	Entity* CreditsButton = nullptr;
	Entity* OptionsButton = nullptr;
	Entity* FadeOutScreen = nullptr;

	// The quit confirmation, which the EXIT button opens instead of closing
	// the window outright.
	std::shared_ptr<MenuQuitConfirmLogic> quitConfirmLogic = nullptr;
	void ResolveQuitConfirmLogic();

	// QUIT GAME, from the mouse or the pad: puts the confirmation up.
	void AskToQuit();

	// HOW TO PLAY and CREDITS open the same journal on their own pages.
	void OpenJournal(int firstPage, int lastPage);

	// START opens the level select screen. There is no separate LEVEL SELECT
	// item: its row went to OPTIONS.
	void OpenLevelSelect();
	/// Close the level select or the journal and bring the menu back. Escape
	/// and the pad's B both do it.
	void CloseSubmenus();

	// OPTIONS opens the same options panel every level has. The menu's own
	// items lie partly under it, so they are switched off while it is open
	// and back on when it closes.
	std::shared_ptr<GameSettingsLogic> optionsLogic = nullptr;
	bool optionsWereOpen = false;
	void ResolveOptionsLogic();
	void OpenOptions(bool fromPad);
	void SetMenuButtonsEnabled(bool enabled);
	void SetMenuShown(bool shown);
	bool OptionsOpen();


	SpriteRendererComponent* startButtonSpriteRenderer = nullptr;
	SpriteRendererComponent* quitButtonSpriteRenderer = nullptr;
	SpriteRendererComponent* htpButtonSpriteRenderer = nullptr;
	SpriteRendererComponent* creditsButtonSpriteRenderer = nullptr;
	SpriteRendererComponent* optionsButtonSpriteRenderer = nullptr;
	SpriteRendererComponent* FadeOutSpriteRenderer = nullptr;

	Texture startNormalTexture = 0;
	Texture startHoverTexture = 0;

	Texture quitNormalTexture = 0;
	Texture quitHoverTexture = 0;

	Texture htpNormalTexture = 0;
	Texture htpHoverTexture = 0;

	Texture creditsNormalTexture = 0;
	Texture creditsHoverTexture = 0;

	Texture optionsNormalTexture = 0;
	Texture optionsHoverTexture = 0;

	SoundComponent* StartSound = nullptr;
	SoundComponent* QuitSound = nullptr;
	SoundComponent* HtpSound = nullptr;
	SoundComponent* CreditsSound = nullptr;
	SoundComponent* OptionsSound = nullptr;

	float fadeOutDuration = 3.0f;
	float fadeElapsedTime = 0.0f;
	bool isFadingOut = false;
	float fadeInDuration = 3.0f;
	float fadeInElapsedTime = 0.0f;
	bool isFadingIn = true;

	bool shouldClose = false;

	std::string nextScene = "";

	TextComponent* FPSText = nullptr;

	enum class MenuSelection {
		START = 0,
		HOW_TO_PLAY = 1,
		OPTIONS = 2,
		CREDITS = 3,
		QUIT = 4,
		COUNT
	};

	MenuSelection currentSelection = MenuSelection::START;
	bool isUsingController = false;
	float controllerNavigationCooldown = 0.0f;
	const float controllerNavigationDelay = 0.2f;

	void UpdateMenuSelection();
	void SelectButton(MenuSelection selection);
	void DeselectAllButtons();
	void ActivateSelectedButton();
};