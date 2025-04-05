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
			LastMainMenu->onHover = []()
				{
					MainMenuSound->Play();
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

	MainMenu->onHover = []()
		{
			MainMenuSound->Play();
		};

	Score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score").get();
	ScoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(Score->entityID);

	if (ScoreText) {
		ScoreText->text = "Score: " + std::to_string(ScoreLogic::scoreValue);
		ScoreText->isEnabled = true;
	}

	Entity* background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("EndBGM").get();
	backgroundSR = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(background->entityID);
	backgroundSR->texture = AssetManager::GetTextureByName("DAY" + ScoreLogic::dayNumber);

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

	LevelSelectScreenLogic::LevelCompleted();

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

	Restart->onClick = [this, lastPlayedSceneName]() {
		MainMenuSound->Play(1);
		isrestartButtonClicked = true;
		isFadingIn = true; // Start fade-in
		fadeInElapsedTime = 0.0f;
	};

	Restart->onHover = []()
		{
			MainMenuSound->Play();
		};

	Next->onClick = [this, lastPlayedSceneName]() {
		MainMenuSound->Play(1);
		isnextButtonClicked = true;
		isFadingIn = true; // Start fade-in
		fadeInElapsedTime = 0.0f;
	};

	Next->onHover = []()
		{
			MainMenuSound->Play();
		};
	
	Next_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(NextButton->entityID);



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
	}
	else
	{
		LevelSelectScreenLogic::stageLevel = LevelSelectScreenLogic::currentStage;
		if (lastPlayedSceneName == "Level3")
		{
			GameManager::GameCleared = true;

			Next_Spt->isVisible = false;
			MainMenu_Spt->isVisible = false;
			LastMainMenu_Spt->isVisible = true;
		}
		else
		{
			Next_Spt->isVisible = true;
			MainMenu_Spt->isVisible = true;
			LastMainMenu_Spt->isVisible = false;
		}
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

void EndScene::Update()
{
	if (GameManager::GameCleared) return;

	if (isFadingIn && FadeInSpriteRenderer && BGMSound)
	{
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
				if (lastPlayedSceneName == "Level0") GameManager::SetActiveScene("Level1");
				else if (lastPlayedSceneName == "Level1") GameManager::SetActiveScene("Level1_5");
				else if (lastPlayedSceneName == "Level1_5") GameManager::SetActiveScene("Level2");
				else if (lastPlayedSceneName == "Level2") GameManager::SetActiveScene("Level2_5");
				else if (lastPlayedSceneName == "Level2_5") GameManager::SetActiveScene("Level3");
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
	// Skip if we only have one button visible (game cleared)
	if (GameManager::GameCleared) return;

	// Skip if restart button is not visible
	if (!Restart_Spt || !Restart_Spt->isVisible) return;

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
		bool dpadLeft = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT);
		bool dpadRight = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);

		// Check for any controller input
		bool hasControllerInput = std::abs(horizontalInput) > 0.3f || dpadLeft || dpadRight ||
			DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
			DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START);

		// If this is the first controller input, select the restart button
		if (hasControllerInput && !isUsingController)
		{
			isUsingController = true;
			SelectButton(EndButtonSelection::RESTART);
			controllerNavigationCooldown = controllerNavigationDelay;
		}

		// Only process navigation if we're using controller
		if (isUsingController)
		{
			// Navigate left
			if (controllerNavigationCooldown <= 0 && (horizontalInput < -0.3f || dpadLeft))
			{
				int newSelection = static_cast<int>(currentButtonSelection) - 1;
				if (newSelection < 0)
				{
					newSelection = static_cast<int>(EndButtonSelection::COUNT) - 1;
				}

				SelectButton(static_cast<EndButtonSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			// Navigate right
			else if (controllerNavigationCooldown <= 0 && (horizontalInput > 0.3f || dpadRight))
			{
				int newSelection = (static_cast<int>(currentButtonSelection) + 1) % static_cast<int>(EndButtonSelection::COUNT);
				SelectButton(static_cast<EndButtonSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			// Activate selected button with A button or Start button
			std::string lastPlayedSceneName = GameManager::GetGlobalVariable("LastPlayedScene");
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
				DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_START))
			{
				ActivateSelectedButton(lastPlayedSceneName);
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

void EndScene::SelectButton(EndButtonSelection selection)
{
	// Deselect all buttons first
	DeselectAllButtons();

	// Update current selection
	currentButtonSelection = selection;

	// Apply hover effect (increase scale)
	if (selection == EndButtonSelection::RESTART && restartTransform)
	{
		// Scale up the restart button
		restartTransform->scale = restartOriginalScale * buttonScaleIncrease;
		// Play sound if available
		if (MainMenuSound) MainMenuSound->Play();
	}
	else if (selection == EndButtonSelection::MENU && menuTransform)
	{
		// Scale up the menu button
		menuTransform->scale = menuOriginalScale * buttonScaleIncrease;
		// Play sound if available
		if (MainMenuSound) MainMenuSound->Play();
	}
}

void EndScene::DeselectAllButtons()
{
	// Reset all button scales to original
	if (restartTransform)
	{
		restartTransform->scale = restartOriginalScale;
	}

	if (menuTransform)
	{
		menuTransform->scale = menuOriginalScale;
	}
}

void EndScene::ActivateSelectedButton(const std::string& lastPlayedSceneName)
{
	switch (currentButtonSelection)
	{
	case EndButtonSelection::RESTART:
		std::cout << "Restart button activated with gamepad!" << std::endl;
		if (MainMenuSound) MainMenuSound->Play(1);
		GameManager::SetActiveScene(lastPlayedSceneName);
		break;

	case EndButtonSelection::MENU:
		std::cout << "Menu button activated with gamepad!" << std::endl;
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