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

	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectScreen").get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();
	HTPScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("HowToPlayScreen").get();

	StartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Start").get();
	auto start = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(StartButton->entityID);
	StartSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(StartButton->entityID);
	
	//level select button
	LevelSelectButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectButton").get();
	auto levelSelect = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(LevelSelectButton->entityID);
	levelSelect->onClick = [this]()
		{
			if (!isFadingOut)
			{
				StartSound->Play(1);
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible = true;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
			}
		};

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

	start->onClick = [this]()
		{
			StartSound->Play(1);

			// Disable all buttons when start button is clicked
			for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
			{
				ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
				if (!button) continue;

				button->isEnabled = false;
			}

			std::string sceneToLoad;

			if (LevelSelectScreenLogic::currentStage >= 5)
			{
				sceneToLoad = "Level3";
				std::cout << "Starting Level 3" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 4)
			{
				sceneToLoad = "Level3";
				std::cout << "Starting Level 3" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 3)
			{
				sceneToLoad = "Level2_5";
				std::cout << "Starting Level 2.5" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 2)
			{
				sceneToLoad = "Level2";
				std::cout << "Starting Level 2" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 1)
			{
				sceneToLoad = "Level1_5";
				std::cout << "Starting Level 1.5" << std::endl;
			}
			else if (LevelSelectScreenLogic::currentStage == 0)
			{
				sceneToLoad = "Level1";
				std::cout << "Starting Level 1" << std::endl;
			}
			else
			{
				sceneToLoad = "Level0"; // Default to tutorial if no progress
				std::cout << "Starting Tutorial Level 0" << std::endl;
			}

			OnPlayButtonClicked(sceneToLoad);
		};
	QuitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit").get();
	auto exit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(QuitButton->entityID);
	QuitSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(QuitButton->entityID);
	exit->onClick = [this]() 
		{ 
			if (!isFadingOut)
			{
				QuitSound->Play(1); GameManager::Engine.CloseWindow();
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
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible = true;
				DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
			}
		};

	startButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(StartButton->entityID);
	levelSelectButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(LevelSelectButton->entityID);
	quitButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(QuitButton->entityID);
	htpButtonSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HtpButton->entityID);
	

	startNormalTexture = AssetManager::GetTextureByName("start");
	startHoverTexture = AssetManager::GetTextureByName("start_click");

	levelSelectNormalTexture = AssetManager::GetTextureByName("levelselect");
	levelSelectHoverTexture = AssetManager::GetTextureByName("levelselect_hover");

	quitNormalTexture = AssetManager::GetTextureByName("quit");
	quitHoverTexture = AssetManager::GetTextureByName("quit_click");

	htpNormalTexture = AssetManager::GetTextureByName("mainmenu_howtoplay");
	htpHoverTexture = AssetManager::GetTextureByName("mainmenu_howtoplay_hover");

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

	// For LevelSelectButton:
	levelSelect->onHover = [this]()
		{
			if (!isUsingController) {
				StartSound->Play();
				levelSelectButtonSpriteRenderer->texture = levelSelectHoverTexture;
			}
		};

	levelSelect->onFinishHover = [this]()
		{
			if (!isUsingController) {
				levelSelectButtonSpriteRenderer->texture = levelSelectNormalTexture;
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
void MainMenu::PostUpdate()
{
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
		float verticalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);
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

	case MenuSelection::LEVEL_SELECT:
		levelSelectButtonSpriteRenderer->texture = levelSelectHoverTexture;
		StartSound->Play();
		break;

	case MenuSelection::HOW_TO_PLAY:
		htpButtonSpriteRenderer->texture = htpHoverTexture;
		HtpSound->Play();
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
	levelSelectButtonSpriteRenderer->texture = levelSelectNormalTexture;
	htpButtonSpriteRenderer->texture = htpNormalTexture;
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
		// Call the OnPlayButtonClicked function with the appropriate scene name
		if (!isFadingOut)
		{
			std::string sceneToLoad;

			if (LevelSelectScreenLogic::currentStage >= 5)
				sceneToLoad = "Level3_5";
			else if (LevelSelectScreenLogic::currentStage == 4)
				sceneToLoad = "Level3";
			else if (LevelSelectScreenLogic::currentStage == 3)
				sceneToLoad = "Level2_5";
			else if (LevelSelectScreenLogic::currentStage == 2)
				sceneToLoad = "Level2";
			else if (LevelSelectScreenLogic::currentStage == 1)
				sceneToLoad = "Level1_5";
			else if (LevelSelectScreenLogic::currentStage == 0)
				sceneToLoad = "Level1";
			else
				sceneToLoad = "Level0"; // Default to tutorial if no progress

			OnPlayButtonClicked(sceneToLoad);
		}
		break;

	case MenuSelection::LEVEL_SELECT:
		if (!isFadingOut)
		{
			StartSound->Play(1);
			if (levelSelectScreen && mainMenuScreen)
			{
				if (auto* levelRenderer = DuckEngine::DUCKENGINE_ComponentManager
					.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID))
				{
					levelRenderer->isVisible = true;
				}
				if (auto* menuRenderer = DuckEngine::DUCKENGINE_ComponentManager
					.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID))
				{
					menuRenderer->isVisible = false;
				}
			}
		}
		break;

	case MenuSelection::HOW_TO_PLAY:
		if (!isFadingOut)
		{
			HtpSound->Play(1);
			if (HTPScreen && mainMenuScreen)
			{
				if (auto* howToPlayRenderer = DuckEngine::DUCKENGINE_ComponentManager
					.GetComponent<SpriteRendererComponent>(HTPScreen->entityID))
				{
					howToPlayRenderer->isVisible = true;
				}
				if (auto* menuRenderer = DuckEngine::DUCKENGINE_ComponentManager
					.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID))
				{
					menuRenderer->isVisible = false;
				}
			}
		}
		break;

	case MenuSelection::QUIT:
		if (!isFadingOut)
		{
			QuitSound->Play(1);
			GameManager::Engine.CloseWindow();
		}
		break;

	default:
		break;
	}
}
