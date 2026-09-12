/******************************************************************************/
/*!
\file       EndGame.cpp
\author     Tan Yan Kai, yankai.tan, 2301312
\par        yankai.tan@digipen.edu
\date       November 30 2024
\brief      Implementation of the EndScene class, it loads up the necessary assets needed for the UI and display of the game score before going back to main menu.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "EndGame.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include <random>
#include <chrono>
#include <map>
#include "Scene.h"
#include "GameManager.h"
#include "ScoreLogic.h"
#include "LevelSelectScreenLogic.h"
#include "SoundSystem.h"
SoundComponent* MainMenuSound = nullptr;
bool isQuitButtonClicked = false;
bool isrestartButtonClicked = false;
bool isnextButtonClicked = false;
bool isBGMSoundFadingIn = false;
float bgmFadeInTimer = 0.0f;

SpriteRendererComponent* backgroundSR;

void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	CameraManager::SetHeight(20);

	MainMenuButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit").get();
	if (MainMenuButton != nullptr) {
		MainMenu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(MainMenuButton->entityID);
		MainMenuSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(MainMenuButton->entityID);
		MainMenu_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(MainMenuButton->entityID);
	}

	auto LastMainMenuBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenu").get();
	if (LastMainMenuBtn) {
		LastMainMenu = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(LastMainMenuBtn->entityID);
		if (LastMainMenu)
		{
			LastMainMenu->onClick = [this]()
				{
					MainMenuSound->Play(1);
					isQuitButtonClicked = true;
					isFadingIn = true; // Start fade-in
					fadeInElapsedTime = 0.0f;
				};
			LastMainMenu->onHover = [this]()
				{
					if (!isUsingController && lastMenuTransform) {
						MainMenuSound->Play();
						lastMenuTransform->scale = lastMenuOriginalScale * buttonScaleIncrease;
					}
				};
			LastMainMenu->onFinishHover = [this]()
				{
					if (!isUsingController && lastMenuTransform) {
						lastMenuTransform->scale = lastMenuOriginalScale;
					}
				};
		}
		LastMainMenu_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(LastMainMenuBtn->entityID);
		if (LastMainMenu_Spt) LastMainMenu_Spt->isVisible = false;
	}

	// Find fade-in screen entity
	FadeInScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FadeInMenu").get();
	if (FadeInScreen)
	{
		FadeInSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(FadeInScreen->entityID);
		if (FadeInSpriteRenderer)
		{
			FadeInSpriteRenderer->color.a = 0; // Start fully transparent
			FadeInSpriteRenderer->isVisible = true;
			fadeInElapsedTime = 0.0f;
			isFadingIn = false;
		}
	}

	MainMenu->onClick = [this]()
		{
			std::cout << "Button clicked QUIT!!!!!!\n";
			MainMenuSound->Play(1);
			isQuitButtonClicked = true;
			isFadingIn = true; // Start fade-in
			fadeInElapsedTime = 0.0f;
		};

	MainMenu->onHover = [this]()
		{
			if (!isUsingController && menuTransform) {
				MainMenuSound->Play();
				menuTransform->scale = menuOriginalScale * buttonScaleIncrease;
			}
		};

	MainMenu->onFinishHover = [this]() {
		if (!isUsingController && menuTransform) {
			menuTransform->scale = menuOriginalScale;
		}
		};

	Score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score").get();
	ScoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(Score->entityID);

	if (ScoreText) {
		ScoreText->text = "Score: " + std::to_string(ScoreLogic::scoreValue);
		ScoreText->isEnabled = true;
	}

	Entity* background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("EndBGM").get();
	backgroundSR = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(background->entityID);
	backgroundSR->texture = AssetManager::GetTextureByName(
		"DAY" + std::to_string(ScoreLogic::dayNumber));

	Star_1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_1").get();
	Star_2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_2").get();
	Star_3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_3").get();

	Star1 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_1->entityID);
	Star2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_2->entityID);
	Star3 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_3->entityID);

	Background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("EndBGM").get();
	BGMSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(Background->entityID);

	auto fpsTextEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("FPS_Text");
	if (fpsTextEntity) {
		FPSText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(fpsTextEntity->entityID);
		if (GameManager::GetGlobalVariable("ShowFPS").empty()) { FPSText->isEnabled = false; }
		else if (GameManager::GetGlobalVariable("ShowFPS") == "true") { FPSText->isEnabled = true; }
		else { FPSText->isEnabled = false; }
	}

	RestartButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restart").get();
	Restart = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(RestartButton->entityID);

	Restart_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(RestartButton->entityID);

	NextButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Next").get();
	Next = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(NextButton->entityID);

	isQuitButtonClicked = false;
	isrestartButtonClicked = false;
	isnextButtonClicked = false;

	auto endsound = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SFXEndScreen").get();
	if (endsound) {
		endSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(endsound->entityID);
	}

	isBGMSoundFadingIn = false;
	bgmFadeInTimer = 0.0f;
}

void EndScene::Start()
{
	Scene::Start();

	std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
	std::cout << "sceneName " << lastPlayedSceneName << std::endl;
	if (!lastPlayedSceneName.empty() && lastPlayedSceneName.find("Level") != std::string::npos)
	{
		int day = std::stoi(lastPlayedSceneName.substr(5, 1));
		std::string EndScenePath = "Resources/Sprites/EndScene/DAY" + std::to_string(day) + ".png";
		DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Background->entityID)->texture = *AssetManager::GetTexture(EndScenePath).get();
	}

	std::string dayTextureName = "DAY" + std::to_string(ScoreLogic::dayNumber);
	backgroundSR->texture = AssetManager::GetTextureByName(dayTextureName);


	// Setup controller-related variables
	isUsingController = false;
	controllerNavigationCooldown = 0.0f;
	currentButtonSelection = EndButtonSelection::RESTART;

	// Get transform components for scaling
	if (RestartButton) {
		restartTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(RestartButton->entityID);
		if (restartTransform) {
			restartOriginalScale = restartTransform->scale;
		}
	}

	if (MainMenuButton) {
		menuTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(MainMenuButton->entityID);
		if (menuTransform) {
			menuOriginalScale = menuTransform->scale;
		}
	}

	if (NextButton) {
		nextTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(NextButton->entityID);
		if (nextTransform) {
			nextOriginalScale = nextTransform->scale;
		}
	}

	auto LastMainMenuBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenu").get();
	if (LastMainMenuBtn) {
		lastMenuTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(LastMainMenuBtn->entityID);
		if (lastMenuTransform) {
			lastMenuOriginalScale = lastMenuTransform->scale;
		}
	}

	Restart->onClick = [this, lastPlayedSceneName]() {
		MainMenuSound->Play(1);
		isrestartButtonClicked = true;
		isFadingIn = true; // Start fade-in
		fadeInElapsedTime = 0.0f;
		};

	Restart->onHover = [this]()
		{
			if (!isUsingController && restartTransform) {
				MainMenuSound->Play();
				restartTransform->scale = restartOriginalScale * buttonScaleIncrease;
			}
		};

	Restart->onFinishHover = [this]() {
		if (!isUsingController && restartTransform) {
			restartTransform->scale = restartOriginalScale;
		}
		};

	Next->onClick = [this, lastPlayedSceneName]() {
		MainMenuSound->Play(1);
		isnextButtonClicked = true;
		isFadingIn = true; // Start fade-in
		fadeInElapsedTime = 0.0f;
		};

	Next->onHover = [this]()
		{
			if (!isUsingController && nextTransform) {
				MainMenuSound->Play();
				nextTransform->scale = nextOriginalScale * buttonScaleIncrease;
			}
		};

	Next->onFinishHover = [this]() {
		if (!isUsingController && nextTransform) {
			nextTransform->scale = nextOriginalScale;
		}
		};

	if (NextButton)
	{
		Next_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(NextButton->entityID);
	}

	// Tutorial Level
	if (lastPlayedSceneName == "Level0" || lastPlayedSceneName == "Level1_5" || lastPlayedSceneName == "Level2_5")
	{
		iStar_1 = iStar_2 = iStar_3 = 10;
	}
	else if (lastPlayedSceneName == "Level1")
	{
		iStar_1 = 50;
		iStar_2 = 100;
		iStar_3 = 150;
	}
	else if (lastPlayedSceneName == "Level2")
	{
		iStar_1 = 75;
		iStar_2 = 125;
		iStar_3 = 175;
	}
	else if (lastPlayedSceneName == "Level3")
	{
		iStar_1 = 100;
		iStar_2 = 150;
		iStar_3 = 200;
	}


	if (ScoreLogic::scoreValue < iStar_1)
	{
		Restart_Spt->isVisible = true;
		Next_Spt->isVisible = false;
		backgroundSR->texture = AssetManager::GetTextureByName("DAYLOSE");
		ScoreText->isEnabled = false;
		Star1->isVisible = false;
		Star2->isVisible = false;
		Star3->isVisible = false;

		if (lastPlayedSceneName == "Level3")
		{
			GameManager::GameCleared = false;
		}

		currentState = EndSceneState::LOSE;
		currentButtonSelection = EndButtonSelection::RESTART;
	}
	else
	{
		LevelSelectScreenLogic::LevelCompleted();

		LevelSelectScreenLogic::stageLevel = LevelSelectScreenLogic::currentStage;
		if (lastPlayedSceneName == "Level3")
		{
			GameManager::GameCleared = true;

			Next_Spt->isVisible = false;
			MainMenu_Spt->isVisible = false;
			LastMainMenu_Spt->isVisible = true;

			currentState = EndSceneState::COMPLETE;
			currentButtonSelection = EndButtonSelection::LASTMENU;
		}
		else
		{
			Next_Spt->isVisible = true;
			MainMenu_Spt->isVisible = true;
			LastMainMenu_Spt->isVisible = false;

			currentState = EndSceneState::WIN;
			currentButtonSelection = EndButtonSelection::NEXT;
		}
		Star1->isVisible = true;
		Star2->isVisible = true;
		Star3->isVisible = true;
		Restart_Spt->isVisible = false;
	}


	if (ScoreLogic::scoreValue >= iStar_1)
	{
		Star1->texture = AssetManager::GetTextureByName("star");
	}

	if (ScoreLogic::scoreValue >= iStar_2)
	{
		Star2->texture = AssetManager::GetTextureByName("star");
	}

	if (ScoreLogic::scoreValue >= iStar_3)
	{
		Star3->texture = AssetManager::GetTextureByName("star");
	}

	if (FPSText != nullptr) {
		FPSText->text = "FPS: " + std::to_string(static_cast<int>(DuckEngine::FPS()));
	}


}

void EndScene::DetermineCurrentState()
{
	// Determine the current state based on which buttons are visible
	if (GameManager::GameCleared) {
		currentState = EndSceneState::COMPLETE;
	}
	else if (Restart_Spt && Restart_Spt->isVisible) {
		currentState = EndSceneState::LOSE;
	}
	else if (Next_Spt && Next_Spt->isVisible) {
		currentState = EndSceneState::WIN;
	}
}

void EndScene::Update()
{
	if (GameManager::GameCleared) {
		currentButtonSelection = EndButtonSelection::LASTMENU;
	}

	if (ScoreLogic::scoreValue < iStar_1 && !isBGMSoundFadingIn)
	{
		std::cout << "Game Over" << std::endl;
		endSFX->Play(1); // Plays the lose SFX
		SoundSystem::SetSoundVolume(BGMSound->soundID[0], 0.0f);
		isBGMSoundFadingIn = true;
		bgmFadeInTimer = 0.0f;
	}
	else if (isBGMSoundFadingIn && BGMSound && currentState == EndSceneState::LOSE) {
		std::cout << "test " << bgmFadeInTimer << std::endl;
		bgmFadeInTimer += DuckEngine::DeltaTime();
		float t = bgmFadeInTimer / 5.f;
		if (t > 1.0f) t = 1.0f;

		float currentVolume = BGMSound->volume * t;
		SoundSystem::SetSoundVolume(BGMSound->soundID[0], currentVolume);
	}



	if (isFadingIn && FadeInSpriteRenderer && BGMSound)
	{
		for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
		{
			ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
			if (!button) continue;

			button->isEnabled = false;
		}

		if ((fadeInElapsedTime += DuckEngine::DeltaTime()) >= fadeInDuration)
		{
			// Ensure full fade-in effect
			FadeInSpriteRenderer->color.a = 255;
			isFadingIn = false;
			fadeInElapsedTime = 0.0f;

			// Ensure music volume is fully off and stop the sound
			SoundSystem::SetSoundVolume(BGMSound->soundID[0], 0.0f);
			SoundSystem::StopSounds(BGMSound->soundID[0]);

			std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
			if (isrestartButtonClicked)
			{
				GameManager::SetActiveScene(lastPlayedSceneName);
				std::cout << "current scene:" << lastPlayedSceneName << std::endl;
			}
			else if (isnextButtonClicked)
			{
				if (lastPlayedSceneName == "Level0")
				{
					GameManager::SetGlobalVariable("LastPlayedScene", "Level1");
					GameManager::SetActiveScene("Level1");
				}
				else if (lastPlayedSceneName == "Level1")
				{
					GameManager::SetGlobalVariable("LastPlayedScene", "Level1_5");
					GameManager::SetActiveScene("Level1_5");
				}

				else if (lastPlayedSceneName == "Level1_5")
				{
					GameManager::SetGlobalVariable("LastPlayedScene", "Level2");
					GameManager::SetActiveScene("Level2");
				}
				else if (lastPlayedSceneName == "Level2")
				{
					GameManager::SetGlobalVariable("LastPlayedScene", "Level2_5");
					GameManager::SetActiveScene("Level2_5");
				}
				else if (lastPlayedSceneName == "Level2_5")
				{
					GameManager::SetGlobalVariable("LastPlayedScene", "Level3");
					GameManager::SetActiveScene("Level3");
				}
			}
			else GameManager::SetActiveScene("MainMenu");

			return;
		}

		// Calculate fade-in progress (0.0 to 1.0)
		float fadeProgress = fadeInElapsedTime / fadeInDuration;

		// Update transparency
		FadeInSpriteRenderer->color.a = static_cast<unsigned char>(fadeProgress * 255);

		// Gradually decrease the background music volume
		SoundSystem::SetSoundVolume(BGMSound->soundID[0], BGMSound->volume * (1.0f - fadeProgress));
	}

	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	// Update gamepad navigation
	UpdateEndMenuSelection();
}

void EndScene::UpdateEndMenuSelection()
{
	// Handle different states separately based on which buttons are visible

	// Handle game cleared state (Only LastMenu button)
	if (currentState == EndSceneState::COMPLETE) {
		// Only one button (LastMenu), so simplified controls
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1)) {
			if (!isUsingController) {
				isUsingController = true;
				if (lastMenuTransform) {
					lastMenuTransform->scale = lastMenuOriginalScale * buttonScaleIncrease;
					if (MainMenuSound) MainMenuSound->Play();
				}
			}

			// Activate with A or Start button
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START)) {
				if (MainMenuSound) MainMenuSound->Play(1);
				isQuitButtonClicked = true;
				isFadingIn = true;
				fadeInElapsedTime = 0.0f;
			}
		}
		else if (isUsingController) {
			isUsingController = false;
			if (lastMenuTransform) {
				lastMenuTransform->scale = lastMenuOriginalScale;
			}
		}

		// Exit early since we don't need the rest of the navigation code
		return;
	}

	// Handle normal win state (Next and Menu buttons)
	else if (currentState == EndSceneState::WIN && Next_Spt && Next_Spt->isVisible) {
		// Check for gamepad connectivity
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
		{
			// Decrease cooldown timer for navigation
			if (controllerNavigationCooldown > 0)
			{
				controllerNavigationCooldown -= DuckEngine::DeltaTime();
			}

			// Get joystick/dpad input for horizontal navigation
			float horizontalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
			float rightHorizontalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_X);
			bool dpadLeft = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT);
			bool dpadRight = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);

			// Check for any controller input
			bool hasControllerInput = std::abs(horizontalInput) > 0.3f || std::abs(rightHorizontalInput) > 0.3f || dpadLeft || dpadRight ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START);

			// If this is the first controller input, select the next button
			if (hasControllerInput && !isUsingController)
			{
				isUsingController = true;
				currentButtonSelection = EndButtonSelection::NEXT;
				SelectButton(currentButtonSelection);
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			// Only process navigation if we're using controller
			if (isUsingController)
			{
				// Navigate left/right to toggle between Next and Menu
				if (controllerNavigationCooldown <= 0 && (horizontalInput < -0.3f || rightHorizontalInput < -0.3f || dpadLeft || horizontalInput > 0.3f || rightHorizontalInput > 0.3f || dpadRight))
				{
					// Toggle between NEXT and MENU
					EndButtonSelection newSelection = (currentButtonSelection == EndButtonSelection::NEXT) ?
						EndButtonSelection::MENU : EndButtonSelection::NEXT;

					SelectButton(newSelection);
					controllerNavigationCooldown = controllerNavigationDelay;
				}

				// Activate selected button with A button or Start button
				std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
				if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
					DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START))
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
	}

	// Handle lose state (Restart and Menu buttons)
	else if (currentState == EndSceneState::LOSE && Restart_Spt && Restart_Spt->isVisible) {
		// Check for gamepad connectivity
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
		{
			// Decrease cooldown timer for navigation
			if (controllerNavigationCooldown > 0)
			{
				controllerNavigationCooldown -= DuckEngine::DeltaTime();
			}

			// Get joystick/dpad input for horizontal navigation
			float horizontalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
			float rightHorizontalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_X);
			bool dpadLeft = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT);
			bool dpadRight = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);

			// Check for any controller input
			bool hasControllerInput = std::abs(horizontalInput) > 0.3f || std::abs(rightHorizontalInput) > 0.3f || dpadLeft || dpadRight ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START);

			// If this is the first controller input, select the restart button
			if (hasControllerInput && !isUsingController)
			{
				isUsingController = true;
				currentButtonSelection = EndButtonSelection::RESTART;
				SelectButton(currentButtonSelection);
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			// Only process navigation if we're using controller
			if (isUsingController)
			{
				// Navigate left/right to toggle between Restart and Menu
				if (controllerNavigationCooldown <= 0 && (horizontalInput < -0.3f || rightHorizontalInput < -0.3f || dpadLeft || horizontalInput > 0.3f || rightHorizontalInput > 0.3f || dpadRight))
				{
					// Toggle between RESTART and MENU
					EndButtonSelection newSelection = (currentButtonSelection == EndButtonSelection::RESTART) ?
						EndButtonSelection::MENU : EndButtonSelection::RESTART;

					SelectButton(newSelection);
					controllerNavigationCooldown = controllerNavigationDelay;
				}

				// Activate selected button with A button or Start button
				std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
				if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
					DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START))
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

void EndScene::SelectButton(EndButtonSelection selection)
{
	// Deselect all buttons first
	DeselectAllButtons();

	// Update current selection
	currentButtonSelection = selection;

	// Apply hover effect (increase scale) based on current selection
	switch (selection) {
	case EndButtonSelection::RESTART:
		if (restartTransform) {
			restartTransform->scale = restartOriginalScale * buttonScaleIncrease;
			if (MainMenuSound) MainMenuSound->Play();
		}
		break;

	case EndButtonSelection::MENU:
		if (menuTransform) {
			menuTransform->scale = menuOriginalScale * buttonScaleIncrease;
			if (MainMenuSound) MainMenuSound->Play();
		}
		break;

	case EndButtonSelection::NEXT:
		if (nextTransform) {
			nextTransform->scale = nextOriginalScale * buttonScaleIncrease;
			if (MainMenuSound) MainMenuSound->Play();
		}
		break;

	case EndButtonSelection::LASTMENU:
		if (lastMenuTransform) {
			lastMenuTransform->scale = lastMenuOriginalScale * buttonScaleIncrease;
			if (MainMenuSound) MainMenuSound->Play();
		}
		break;

	default:
		break;
	}
}

void EndScene::DeselectAllButtons()
{
	// Reset all button scales to original
	if (restartTransform) {
		restartTransform->scale = restartOriginalScale;
	}

	if (menuTransform) {
		menuTransform->scale = menuOriginalScale;
	}

	if (nextTransform) {
		nextTransform->scale = nextOriginalScale;
	}

	if (lastMenuTransform) {
		lastMenuTransform->scale = lastMenuOriginalScale;
	}
}

void EndScene::ActivateSelectedButton()
{
	switch (currentButtonSelection)
	{
	case EndButtonSelection::RESTART:
		std::cout << "Restart button activated with gamepad!" << std::endl;
		if (MainMenuSound) MainMenuSound->Play(1);
		isrestartButtonClicked = true;
		isFadingIn = true;
		fadeInElapsedTime = 0.0f;
		break;

	case EndButtonSelection::MENU:
		std::cout << "Menu button activated with gamepad!" << std::endl;
		if (MainMenuSound) MainMenuSound->Play(1);
		isQuitButtonClicked = true;
		isFadingIn = true;
		fadeInElapsedTime = 0.0f;
		break;

	case EndButtonSelection::NEXT:
		std::cout << "Next button activated with gamepad!" << std::endl;
		if (MainMenuSound) MainMenuSound->Play(1);
		isnextButtonClicked = true;
		isFadingIn = true;
		fadeInElapsedTime = 0.0f;
		break;

	case EndButtonSelection::LASTMENU:
		std::cout << "Final Menu button activated with gamepad!" << std::endl;
		if (MainMenuSound) MainMenuSound->Play(1);
		isQuitButtonClicked = true;
		isFadingIn = true;
		fadeInElapsedTime = 0.0f;
		break;

	default:
		break;
	}
}

void EndScene::PostUpdate()
{
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (FPSText) {
			FPSText->isEnabled = !FPSText->isEnabled;
			GameManager::SetGlobalVariable("ShowFPS", FPSText->isEnabled ? "true" : "false");
		}
	}
}

void EndScene::Exit()
{
	ScoreText->isEnabled = false;
	fadeInElapsedTime = 0.0f;
	isFadingIn = false;
	GameManager::GameCleared = false;
}

void EndScene::Unload()
{
	// base unload
	Scene::Unload();
}
