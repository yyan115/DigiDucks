/******************************************************************************/
/*!
\file       MainMenu.cpp
\author     Jovan Chua, c.shengkaijovan, 2301244
\par        c.shengkaijovan@digipen.edu
\date       November 30 2024
\brief      Implementation of the MainMenu class, it loads up the necessary assets needed for the UI before going into the GameScene.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "MainMenu.h"
#include "ButtonSystem.h"
#include "ButtonComponent.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include <random>
#include <chrono>
#include <map>
#include "Scene.h"
#include "GameManager.h"
#include "SoundSystem.h"
#include "HowToPlayLogic.h"
#include "SaveLoadManager.h"

/****************************************************************
* @brief Load all necessary resources for the scene.
* This function is called before the scene starts.
* ****************************************************************/

void MainMenu::Load()
{
	Scene::Load();
	DuckEngine::EnableLogging(false);
	CameraManager::SetHeight(20);
	SaveLoadManager::InitializeSaveFile();

	// Apply the saved audio settings
	SoundSystem::SetMasterVolume(SaveLoadManager::masterVolume);
	SoundSystem::SetCategoryVolume("BGM", SaveLoadManager::musicVolume);
	SoundSystem::SetCategoryVolume("SFX", SaveLoadManager::sfxVolume);

	// The scene object outlives each visit to the menu, and every load makes
	// new entities and new logic. Anything looked up on a previous visit
	// points at what that visit freed, so it is dropped here and found again.
	quitConfirmLogic = nullptr;
	optionsLogic = nullptr;
	optionsWereOpen = false;

	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectScreen").get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();
	HTPScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlayScreen").get();

	StartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Start").get();
	auto start = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(StartButton->entityID);
	StartSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(StartButton->entityID);
	

	menusound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MenuBGM").get()->entityID);
	if (menusound)
	{
		SoundSystem::SetSoundVolume(menusound->soundID[0], 0.0f); // Start at 0 volume
	}
	FadeOutScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeOutMenu").get();
	// Ensure the FadeOutSprite is reset
	FadeOutSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeOutScreen->entityID);
	FadeOutSpriteRenderer->color.a = 0;
	FadeOutSpriteRenderer->isVisible = true; 
	fadeOutDuration = 3.0f;
	fadeElapsedTime = 0.0f;
	isFadingOut = false;
	fadeInDuration = 3.0f;
	fadeInElapsedTime = 0.0f;
	isFadingIn = true;

	// START opens the level select screen, where a player picks a level
	// rather than being put straight into the next one.
	start->onClick = [this]()
		{
			if (!isFadingOut)
			{
				StartSound->Play(1);
				OpenLevelSelect();
			}
		};
	QuitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit").get();
	auto exit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(QuitButton->entityID);
	QuitSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(QuitButton->entityID);
	exit->onClick = [this]() 
		{ 
			if (!isFadingOut)
			{
				// Ask first. This used to close the window on the click with
				// nothing said, which is the one button on this screen a
				// player cannot take back.
				QuitSound->Play(1);
				AskToQuit();
			}
		};
	HtpButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlay").get();
	auto htp = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(HtpButton->entityID);
	HtpSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(HtpButton->entityID);
	htp->onClick = [this]() 
		{
			if (!isFadingOut)
			{
				HtpSound->Play(1); 
				OpenJournal(HowToPlayLogic::kFirstInstructionsPage, HowToPlayLogic::kLastInstructionsPage);
			}
		};

	// CREDITS opens the same journal as HOW TO PLAY, on the two credits pages.
	CreditsButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Credits").get();
	auto credits = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(CreditsButton->entityID);
	CreditsSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(CreditsButton->entityID);
	credits->onClick = [this]()
		{
			if (!isFadingOut)
			{
				CreditsSound->Play(1);
				OpenJournal(HowToPlayLogic::kFirstCreditsPage, HowToPlayLogic::kLastCreditsPage);
			}
		};

	// OPTIONS opens the options panel every level has.
	OptionsButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Options").get();
	auto options = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(OptionsButton->entityID);
	OptionsSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(OptionsButton->entityID);
	options->onClick = [this]()
		{
			if (!isFadingOut)
			{
				OptionsSound->Play(1);
				OpenOptions(false);
			}
		};

	startButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(StartButton->entityID);
	quitButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(QuitButton->entityID);
	htpButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HtpButton->entityID);
	creditsButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(CreditsButton->entityID);
	optionsButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(OptionsButton->entityID);
	

	startNormalTexture = AssetManager::GetTextureByName("start");
	startHoverTexture = AssetManager::GetTextureByName("start_click");

	quitNormalTexture = AssetManager::GetTextureByName("quit");
	quitHoverTexture = AssetManager::GetTextureByName("quit_click");

	htpNormalTexture = AssetManager::GetTextureByName("mainmenu_howtoplay");
	htpHoverTexture = AssetManager::GetTextureByName("mainmenu_howtoplay_hover");

	creditsNormalTexture = AssetManager::GetTextureByName("credits");
	creditsHoverTexture = AssetManager::GetTextureByName("credits_hover");

	optionsNormalTexture = AssetManager::GetTextureByName("options");
	optionsHoverTexture = AssetManager::GetTextureByName("options_hover");

	// Modify the button callback lambda functions in the Load() method
	// Change each of the button hover callbacks as follows:

	// For StartButton:
	start->onHover = [this]()
		{
			if (!isUsingController) {  // Only perform hover action if not using controller
				StartSound->Play();
				startButtonSpriteRenderer->texture = startHoverTexture;
			}
		};

	start->onFinishHover = [this]()
		{
			if (!isUsingController) {  // Only perform finish hover if not using controller
				startButtonSpriteRenderer->texture = startNormalTexture;
			}
		};

	// For OptionsButton:
	options->onHover = [this]()
		{
			if (!isUsingController) {
				OptionsSound->Play();
				optionsButtonSpriteRenderer->texture = optionsHoverTexture;
			}
		};

	options->onFinishHover = [this]()
		{
			if (!isUsingController) {
				optionsButtonSpriteRenderer->texture = optionsNormalTexture;
			}
		};

	// For QuitButton:
	exit->onHover = [this]()
		{
			if (!isUsingController) {
				QuitSound->Play();
				quitButtonSpriteRenderer->texture = quitHoverTexture;
			}
		};

	exit->onFinishHover = [this]()
		{
			if (!isUsingController) {
				quitButtonSpriteRenderer->texture = quitNormalTexture;
			}
		};

	// For HtpButton:
	htp->onHover = [this]()
		{
			if (!isUsingController) {
				HtpSound->Play();
				htpButtonSpriteRenderer->texture = htpHoverTexture;
			}
		};

	htp->onFinishHover = [this]()
		{
			if (!isUsingController) {
				htpButtonSpriteRenderer->texture = htpNormalTexture;
			}
		};

	credits->onHover = [this]()
		{
			if (!isUsingController) {
				CreditsSound->Play();
				creditsButtonSpriteRenderer->texture = creditsHoverTexture;
			}
		};

	credits->onFinishHover = [this]()
		{
			if (!isUsingController) {
				creditsButtonSpriteRenderer->texture = creditsNormalTexture;
			}
		};

	auto fpsTextEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPS_Text");
	if (fpsTextEntity) {
		FPSText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(fpsTextEntity->entityID);
		if (GameManager::GetGlobalVariable("ShowFPS").empty()) { FPSText->isEnabled = false; } 
		else if (GameManager::GetGlobalVariable("ShowFPS") == "true") { FPSText->isEnabled = true; }
		else { FPSText->isEnabled = false; }
	}
}


/****************************************************************
* @brief Starts the scene, initializing game objects and setting up the initial state.
* This function is called when the scene begins running.
* ****************************************************************/
void MainMenu ::Start()
{
	Scene::Start();
}

/****************************************************************
* @brief Updates the scene logic each frame, including processing input and updating game objects.
* This function is called every frame.
* ****************************************************************/
void MainMenu ::Update()
{
	if (isFadingIn && FadeOutSpriteRenderer && menusound)
	{
		if ((fadeInElapsedTime += DuckEngine::DeltaTime()) >= fadeInDuration)
		{
			// Fadein
			FadeOutSpriteRenderer->color.a = 0;
			isFadingIn = false;
			fadeInElapsedTime = 0.0f;

			// Set the final volume for the background music
			SoundSystem::SetSoundVolume(menusound->soundID[0], menusound->volume);
		}
		else
		{
			// Gradually decrease alpha from 255 to 0
			float fadeProgress = fadeInElapsedTime / fadeInDuration;
			FadeOutSpriteRenderer->color.a = static_cast<unsigned char>((1.0f - fadeProgress) * 255);
		}
	}


	if (isFadingOut && FadeOutSpriteRenderer && menusound) {
		if ((fadeElapsedTime += DuckEngine::DeltaTime()) >= fadeOutDuration) {
			SoundSystem::SetSoundVolume(menusound->soundID[0], 0.0f);
			SoundSystem::StopSounds(menusound->soundID[0]);
			FadeOutSpriteRenderer->color.a = 0;
			isFadingOut = false;
			fadeElapsedTime = 0.0f;

			GameManager::SetActiveScene(nextScene);
			return;
		}

		// Calculate fade progress
		float fadeProgress = fadeElapsedTime / fadeOutDuration;
		SoundSystem::SetSoundVolume(menusound->soundID[0], menusound->volume * (1.0f - fadeProgress));
		FadeOutSpriteRenderer->color.a = static_cast<unsigned char>(fadeProgress * 255);
	}

	UpdateMenuSelection();

	if (FPSText != nullptr) {
		FPSText->text = "FPS: " + std::to_string(static_cast<int>(DuckEngine::FPS()));
	}

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

}

/****************************************************************
* @brief Performs any operations after the main update logic, such as cleanup or post-processing.
* This function is called every frame, after the Update() method.
* ****************************************************************/
void MainMenu::ResolveQuitConfirmLogic()
{
	// Looked up on use rather than in Start: the scene's logic instances are
	// not in GameLogicManager's map yet when Start runs, so a lookup there
	// comes back empty and the EXIT button falls through to closing the
	// window without asking.
	if (quitConfirmLogic) { return; }
	if (auto quitCfm = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit_Cfm_Bg"))
	{
		quitConfirmLogic = GameLogicManager::GetLogicForEntity<MenuQuitConfirmLogic>(quitCfm->entityID);
	}
}

void MainMenu::AskToQuit()
{
	ResolveQuitConfirmLogic();
	if (quitConfirmLogic)
	{
		quitConfirmLogic->Show(true);
	}
	else
	{
		GameManager::Engine.CloseWindow();
	}
}

void MainMenu::OpenJournal(int firstPage, int lastPage)
{
	if (!HTPScreen || !mainMenuScreen) return;
	if (auto howToPlay = GameLogicManager::GetLogicForEntity<HowToPlayLogic>(HTPScreen->entityID))
	{
		howToPlay->Open(firstPage, lastPage);
	}
	else if (auto* journal = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID))
	{
		journal->isVisible = true;
	}
	if (auto* menu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID))
	{
		menu->isVisible = false;
	}
}

void MainMenu::OpenLevelSelect()
{
	if (!levelSelectScreen || !mainMenuScreen) return;
	if (auto* levels = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID))
	{
		levels->isVisible = true;
	}
	if (auto* menu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID))
	{
		menu->isVisible = false;
	}
}

void MainMenu::ResolveOptionsLogic()
{
	if (optionsLogic) return;
	if (auto panel = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Settings_Menu"))
	{
		optionsLogic = GameLogicManager::GetLogicForEntity<GameSettingsLogic>(panel->entityID);
	}
}

bool MainMenu::OptionsOpen()
{
	ResolveOptionsLogic();
	return optionsLogic && optionsLogic->isSettingsVisible();
}

void MainMenu::SetMenuButtonsEnabled(bool enabled)
{
	for (Entity* item : { StartButton, HtpButton, OptionsButton, CreditsButton, QuitButton })
	{
		if (!item) continue;
		if (auto* button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(item->entityID))
		{
			button->isEnabled = enabled;
			button->isHovered = false;
		}
	}
}

void MainMenu::OpenOptions(bool fromPad)
{
	ResolveOptionsLogic();
	if (!optionsLogic) return;
	DeselectAllButtons();
	SetMenuButtonsEnabled(false);
	// The menu steps aside for the panel, the way it does for How To Play and
	// the level select, rather than showing its labels round the panel's edge.
	SetMenuShown(false);
	optionsWereOpen = true;
	optionsLogic->ShowSettings(true, fromPad);
}

void MainMenu::SetMenuShown(bool shown)
{
	// The logo and the five items only. The duck art behind them stays, so the
	// panel sits on the menu's own picture rather than on black.
	Entity* logo = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Logo").get();
	for (Entity* item : { logo, StartButton, HtpButton, OptionsButton, CreditsButton, QuitButton })
	{
		if (!item) continue;
		if (auto* sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(item->entityID))
		{
			sprite->isVisible = shown;
		}
	}
}

void MainMenu::PostUpdate()
{
	// The window's own close request is taken by MenuQuitConfirmLogic, which
	// every scene carries, so it asks the same question on every screen.

	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (FPSText) {
			FPSText->isEnabled = !FPSText->isEnabled;
			GameManager::SetGlobalVariable("ShowFPS", FPSText->isEnabled ? "true" : "false");
		}
	}
}

/****************************************************************
* @brief Exits the scene, performing any cleanup necessary before the scene is unloaded.
* ****************************************************************/
void MainMenu::Exit()
{

}

/****************************************************************
* @brief Unloads the scene and frees any resources that were loaded during the Load() phase.
* ****************************************************************/
void MainMenu::Unload()
{
	// base unload
	Scene::Unload();
}

void MainMenu::OnPlayButtonClicked(std::string sceneNaming)
{
	if (menusound && !isFadingOut) 
	{
		StartSound->Play(1);
		fadeOutDuration = 3.0f; // 3 seconds fade duration
		fadeElapsedTime = 0.0f;
		isFadingOut = true;
		nextScene = sceneNaming;
		std::cout << "LOADING SCENE NAME: " << sceneNaming << std::endl;
		GameManager::SetGlobalVariable("LastPlayedScene", nextScene);
	}
}

void MainMenu::UpdateMenuSelection()
{
	// First, check if any submenus are open - don't allow main menu navigation if they are
	bool submenusOpen = false;

	if (levelSelectScreen && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible) {
		submenusOpen = true;
	}

	if (HTPScreen && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible) {
		submenusOpen = true;
	}

	// The options panel, which has its own navigation and its own B. Escape
	// closes it here, as it does in a level. The menu's items come back on
	// the frame after it closes, so the click or press that closed it cannot
	// also land on one of them.
	if (OptionsOpen())
	{
		optionsWereOpen = true;
		if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE))
		{
			optionsLogic->ShowSettings(false);
		}
		return;
	}
	if (optionsWereOpen)
	{
		optionsWereOpen = false;
		SetMenuShown(true);
		SetMenuButtonsEnabled(true);
		if (isUsingController) SelectButton(currentSelection);
		return;
	}

	// The quit confirmation, which has its own navigation and its own B. This
	// returns rather than falling through to the back-button handling below,
	// because that handler closes level select and How To Play and would have
	// nothing to do here while still swallowing the press.
	ResolveQuitConfirmLogic();
	if (quitConfirmLogic && quitConfirmLogic->isShowing()) {
		return;
	}

	// Check for the back button to return from submenus
	if (submenusOpen && DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1)) {
		if (DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B)) {
			// Close level select if it's open
			if (levelSelectScreen && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible) {
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible = false;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = true;
			}

			// Close how to play if it's open
			if (HTPScreen && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible) {
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible = false;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = true;
			}
		}

		// Don't process main menu navigation when submenus are open
		return;
	}

	// Check for gamepad connectivity
	if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
	{
		// Decrease cooldown timer for navigation
		if (controllerNavigationCooldown > 0)
		{
			controllerNavigationCooldown -= DuckEngine::DeltaTime();
		}

		// Get joystick/dpad input
		float verticalInput = DuckEngine_Input::GetMenuAxisVertical(DuckEngine_Input::GAMEPAD_1);
		bool dpadUp = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP);
		bool dpadDown = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN);

		// Check for any controller input
		bool hasControllerInput = std::abs(verticalInput) > 0.3f || dpadUp || dpadDown ||
			DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

		// If this is the first controller input, select the start button
		if (hasControllerInput && !isUsingController)
		{
			isUsingController = true;
			SelectButton(MenuSelection::START);
			controllerNavigationCooldown = controllerNavigationDelay;
		}

		// Only process navigation if we're using controller
		if (isUsingController)
		{
			// Navigate up
			if (controllerNavigationCooldown <= 0 && (verticalInput < -0.3f || dpadUp))
			{
				int newSelection = static_cast<int>(currentSelection) - 1;
				if (newSelection < 0)
					newSelection = static_cast<int>(MenuSelection::COUNT) - 1;

				SelectButton(static_cast<MenuSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			// Navigate down
			else if (controllerNavigationCooldown <= 0 && (verticalInput > 0.3f || dpadDown))
			{
				int newSelection = (static_cast<int>(currentSelection) + 1) % static_cast<int>(MenuSelection::COUNT);
				SelectButton(static_cast<MenuSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			// Activate selected button with X button (A on Xbox)
			if (DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
			{
				ActivateSelectedButton();
			}
		}
	}
	else
	{
		// Reset controller usage flag when no gamepad is connected
		if (isUsingController)
		{
			DeselectAllButtons();
			isUsingController = false;
		}
	}

	// Switch back to mouse mode if mouse movement is detected
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

/****************************************************************
* @brief Selects a button in the menu for controller navigation
* @param selection - The menu item to select
* ****************************************************************/
void MainMenu::SelectButton(MenuSelection selection)
{
	// Deselect all buttons first
	DeselectAllButtons();

	// Update current selection
	currentSelection = selection;

	// Apply hover effect to the selected button
	switch (selection)
	{
	case MenuSelection::START:
		startButtonSpriteRenderer->texture = startHoverTexture;
		StartSound->Play();
		break;

	case MenuSelection::OPTIONS:
		optionsButtonSpriteRenderer->texture = optionsHoverTexture;
		OptionsSound->Play();
		break;

	case MenuSelection::HOW_TO_PLAY:
		htpButtonSpriteRenderer->texture = htpHoverTexture;
		HtpSound->Play();
		break;

	case MenuSelection::CREDITS:
		creditsButtonSpriteRenderer->texture = creditsHoverTexture;
		CreditsSound->Play();
		break;

	case MenuSelection::QUIT:
		quitButtonSpriteRenderer->texture = quitHoverTexture;
		QuitSound->Play();
		break;

	default:
		break;
	}
}

/****************************************************************
* @brief Deselects all buttons in the menu
* ****************************************************************/
void MainMenu::DeselectAllButtons()
{
	startButtonSpriteRenderer->texture = startNormalTexture;
	optionsButtonSpriteRenderer->texture = optionsNormalTexture;
	htpButtonSpriteRenderer->texture = htpNormalTexture;
	creditsButtonSpriteRenderer->texture = creditsNormalTexture;
	quitButtonSpriteRenderer->texture = quitNormalTexture;
}

/****************************************************************
* @brief Activates the currently selected button
* ****************************************************************/
void MainMenu::ActivateSelectedButton()
{
	if (GameManager::GamepadDown)
	{
		GameManager::GamepadDown = false;
		return;
	}
	switch (currentSelection)
	{
	case MenuSelection::START:
		if (!isFadingOut)
		{
			StartSound->Play(1);
			OpenLevelSelect();
		}
		break;

	case MenuSelection::OPTIONS:
		if (!isFadingOut)
		{
			OptionsSound->Play(1);
			OpenOptions(true);
		}
		break;

	case MenuSelection::HOW_TO_PLAY:
		if (!isFadingOut)
		{
			HtpSound->Play(1);
			OpenJournal(HowToPlayLogic::kFirstInstructionsPage, HowToPlayLogic::kLastInstructionsPage);
		}
		break;

	case MenuSelection::CREDITS:
		if (!isFadingOut)
		{
			CreditsSound->Play(1);
			OpenJournal(HowToPlayLogic::kFirstCreditsPage, HowToPlayLogic::kLastCreditsPage);
		}
		break;

	case MenuSelection::QUIT:
		// The same question the mouse gets. This closed the window on the
		// spot, so a player on a pad could quit without being asked.
		if (!isFadingOut)
		{
			QuitSound->Play(1);
			AskToQuit();
		}
		break;

	default:
		break;
	}
}
