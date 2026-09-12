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

namespace
{
	std::shared_ptr<Entity> CreateUIEntity(
		Scene& scene,
		const std::string& name,
		const Vec2& position,
		const Vec2& scale)
	{
		auto entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
		entity->name = name;
		entity->layerName = "UI";

		auto* transform = DuckEngine::DUCKENGINE_ComponentManager
			.AddComponent<TransformComponent>(entity->entityID);
		transform->angle = 0.0f;
		transform->scale = scale;
		transform->relativeToCamera = true;
		transform->SetPosition(position);

		scene.AddEntityToLayer("UI", entity.get());
		return entity;
	}

	std::shared_ptr<Entity> CreateSolidUIEntity(
		Scene& scene,
		const std::string& name,
		const Vec2& position,
		const Vec2& scale,
		const Color& color,
		int sortingOrder,
		bool visible = true)
	{
		auto entity = CreateUIEntity(scene, name, position, scale);
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(
			entity->entityID, true, sortingOrder, true, color, visible);
		return entity;
	}

	std::shared_ptr<Entity> CreateTextUIEntity(
		Scene& scene,
		const std::string& name,
		const std::string& text,
		const Vec2& position,
		float fontSize,
		const Color& color,
		int sortingOrder)
	{
		auto entity = CreateUIEntity(scene, name, position, Vec2(1.0f, 1.0f));
		DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TextComponent>(
			entity->entityID,
			"Kenney High Square",
			text,
			Vec2(),
			fontSize,
			color,
			true,
			sortingOrder);
		return entity;
	}

	void AttachChild(
		const std::shared_ptr<Entity>& parent,
		const std::shared_ptr<Entity>& child)
	{
		parent->childEntities.push_back(child);
		parent->childNames.push_back(child->name);
	}
}

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

	CreateComplianceMenus();

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
				QuitSound->Play(1);
				ShowQuitConfirmation(true);
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

	quitNormalTexture = *AssetManager::GetTexture(
		"Resources/Sprites/MainMenu/quit.png");
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

	creditsButton->onClick = [this]()
		{
			if (!isFadingOut)
			{
				StartSound->Play(1);
				ShowCredits(true);
			}
		};
	creditsButton->onHover = [this]()
		{
			if (!isUsingController)
			{
				StartSound->Play();
				creditsButtonRenderer->color = Color(255.0f, 224.0f, 153.0f, 255.0f);
				creditsButtonText->color = Color(82.0f, 38.0f, 24.0f, 255.0f);
			}
		};
	creditsButton->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				creditsButtonRenderer->color = Color(82.0f, 38.0f, 24.0f, 230.0f);
				creditsButtonText->color = Color(255.0f, 241.0f, 220.0f, 255.0f);
			}
		};

	creditsCloseButton->onClick = [this]()
		{
			StartSound->Play(1);
			ShowCredits(false);
		};

	quitConfirmYesButton->onClick = [this]()
		{
			QuitSound->Play(1);
			GameManager::Engine.CloseWindow();
		};
	quitConfirmNoButton->onClick = [this]()
		{
			QuitSound->Play(1);
			ShowQuitConfirmation(false);
		};
	quitConfirmYesButton->onHover = [this]()
		{
			if (!isUsingController)
			{
				quitConfirmYesRenderer->texture = quitConfirmYesHoverTexture;
			}
		};
	quitConfirmYesButton->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				quitConfirmYesRenderer->texture = quitConfirmYesTexture;
			}
		};
	quitConfirmNoButton->onHover = [this]()
		{
			if (!isUsingController)
			{
				quitConfirmNoRenderer->texture = quitConfirmNoHoverTexture;
			}
		};
	quitConfirmNoButton->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				quitConfirmNoRenderer->texture = quitConfirmNoTexture;
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

void MainMenu::CreateComplianceMenus()
{
	const Color cream(255.0f, 241.0f, 220.0f, 255.0f);
	const Color brown(82.0f, 38.0f, 24.0f, 255.0f);
	const Color gold(220.0f, 157.0f, 75.0f, 255.0f);

	auto creditsButtonEntity = CreateSolidUIEntity(
		*this, "Credits", Vec2(-14.0f, 8.5f), Vec2(5.5f, 1.3f),
		Color(82.0f, 38.0f, 24.0f, 230.0f), 20);
	CreditsButton = creditsButtonEntity.get();
	creditsButtonRenderer = DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(CreditsButton->entityID);
	creditsButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(CreditsButton->entityID);
	auto creditsButtonLabel = CreateTextUIEntity(
		*this, "CreditsLabel", "CREDITS", Vec2(-16.3f, 8.15f),
		0.55f, cream, 21);
	creditsButtonText = DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<TextComponent>(creditsButtonLabel->entityID);
	AttachChild(creditsButtonEntity, creditsButtonLabel);
	AttachChild(
		DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen"),
		creditsButtonEntity);

	auto creditsRoot = CreateSolidUIEntity(
		*this, "CreditsScreen", Vec2(0.0f, 0.0f), Vec2(36.0f, 20.0f),
		Color(35.0f, 18.0f, 12.0f, 245.0f), 100, false);
	creditsScreen = creditsRoot.get();
	auto creditsPanel = CreateSolidUIEntity(
		*this, "CreditsPanel", Vec2(0.0f, 0.0f), Vec2(34.0f, 18.2f),
		cream, 101);
	AttachChild(creditsRoot, creditsPanel);

	const struct CreditLine
	{
		const char* name;
		const char* text;
		Vec2 position;
		float size;
		Color color;
	} creditLines[] = {
		{ "CreditsTitle", "CREDITS", Vec2(-3.2f, 8.0f), 1.2f, brown },
		{ "CreditsGame", "QUACK KITCHEN - TEAM DIGIDUCKS", Vec2(-7.5f, 6.4f), 0.55f, gold },
		{ "CreditsTeamHeading", "PROJECT TEAM", Vec2(-15.0f, 4.8f), 0.52f, brown },
		{ "CreditsTeam1", "Lucas Yee Jun Jie", Vec2(-15.0f, 3.6f), 0.38f, brown },
		{ "CreditsTeam2", "Ernest Ho Yong Heng", Vec2(-15.0f, 2.6f), 0.38f, brown },
		{ "CreditsTeam3", "Muhammad Zikry Bin Zakaria", Vec2(-15.0f, 1.6f), 0.38f, brown },
		{ "CreditsTeam4", "Yan Yu", Vec2(-15.0f, 0.6f), 0.38f, brown },
		{ "CreditsTeam5", "Tan Yan Kai", Vec2(-15.0f, -0.4f), 0.38f, brown },
		{ "CreditsTeam6", "Jovan Chua", Vec2(-15.0f, -1.4f), 0.38f, brown },
		{ "CreditsTeam7", "Lee Ying Jie", Vec2(-15.0f, -2.4f), 0.38f, brown },
		{ "CreditsToolsHeading", "TOOLS, SOFTWARE AND LIBRARIES", Vec2(1.0f, 4.8f), 0.45f, brown },
		{ "CreditsTools1", "DuckEngine - custom game engine", Vec2(1.0f, 3.6f), 0.34f, brown },
		{ "CreditsTools2", "FMOD Studio by Firelight Technologies Pty Ltd", Vec2(1.0f, 2.6f), 0.30f, brown },
		{ "CreditsTools3", "GLFW, GLEW, FreeType, GLM", Vec2(1.0f, 1.6f), 0.34f, brown },
		{ "CreditsTools4", "stb_image, Dear ImGui, nlohmann/json", Vec2(1.0f, 0.6f), 0.31f, brown },
		{ "CreditsTools5", "Kenney fonts (CC0); Inter font (OFL 1.1)", Vec2(1.0f, -0.4f), 0.30f, brown },
		{ "CreditsTools6", "Sound Ideas General HD SFX Collection", Vec2(1.0f, -1.4f), 0.31f, brown },
		{ "CreditsTools7", "Cartoon Express SFX Library; HVD Fonts - Mikado", Vec2(1.0f, -2.4f), 0.28f, brown },
		{ "CreditsSchool", "Created at DigiPen Institute of Technology Singapore", Vec2(-10.2f, -5.4f), 0.40f, brown },
		{ "CreditsCopyright1", "All content Copyright 2026 DigiPen Institute of Technology Singapore.", Vec2(-12.8f, -6.6f), 0.34f, brown },
		{ "CreditsCopyright2", "All Rights Reserved.", Vec2(-3.9f, -7.6f), 0.34f, brown },
	};

	for (const auto& line : creditLines)
	{
		AttachChild(
			creditsRoot,
			CreateTextUIEntity(
				*this, line.name, line.text, line.position, line.size,
				line.color, 102));
	}

	auto creditsCloseEntity = CreateUIEntity(
		*this, "CreditsClose", Vec2(15.5f, 8.3f), Vec2(2.0f, 2.0f));
	auto* creditsCloseRenderer = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<SpriteRendererComponent>(
			creditsCloseEntity->entityID, true, 103, true, cream, true);
	const std::string closeTexturePath = "Resources/Sprites/HowToPlay/xbutton.png";
	if (auto texture = AssetManager::GetTexture(closeTexturePath))
	{
		creditsCloseRenderer->texture = *texture;
		creditsCloseRenderer->texturePath = closeTexturePath;
	}
	creditsCloseButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(creditsCloseEntity->entityID);
	AttachChild(creditsRoot, creditsCloseEntity);

	auto quitRoot = CreateSolidUIEntity(
		*this, "QuitConfirmScreen", Vec2(0.0f, 0.0f), Vec2(36.0f, 20.0f),
		Color(35.0f, 18.0f, 12.0f, 220.0f), 110, false);
	quitConfirmScreen = quitRoot.get();
	auto quitPanel = CreateSolidUIEntity(
		*this, "QuitConfirmPanel", Vec2(0.0f, 0.0f), Vec2(18.0f, 8.0f),
		cream, 111);
	AttachChild(quitRoot, quitPanel);
	AttachChild(
		quitRoot,
		CreateTextUIEntity(
			*this, "QuitConfirmTitle", "QUIT QUACK KITCHEN?",
			Vec2(-6.6f, 1.7f), 0.72f, brown, 112));
	AttachChild(
		quitRoot,
		CreateTextUIEntity(
			*this, "QuitConfirmHint", "Unsaved progress may be lost.",
			Vec2(-5.2f, 0.2f), 0.36f, brown, 112));

	const std::string yesTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_yes.png";
	const std::string yesHoverTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_yes_hover.png";
	const std::string noTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_no.png";
	const std::string noHoverTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_no_hover.png";
	quitConfirmYesTexture = *AssetManager::GetTexture(yesTexturePath);
	quitConfirmYesHoverTexture = *AssetManager::GetTexture(yesHoverTexturePath);
	quitConfirmNoTexture = *AssetManager::GetTexture(noTexturePath);
	quitConfirmNoHoverTexture = *AssetManager::GetTexture(noHoverTexturePath);

	auto quitYesEntity = CreateUIEntity(
		*this, "QuitConfirmYes", Vec2(-3.2f, -2.0f), Vec2(5.0f, 1.75f));
	quitConfirmYesRenderer = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<SpriteRendererComponent>(
			quitYesEntity->entityID, true, 112, true, cream, true);
	quitConfirmYesRenderer->texture = quitConfirmYesTexture;
	quitConfirmYesRenderer->texturePath = yesTexturePath;
	quitConfirmYesButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(quitYesEntity->entityID);
	AttachChild(quitRoot, quitYesEntity);

	auto quitNoEntity = CreateUIEntity(
		*this, "QuitConfirmNo", Vec2(3.2f, -2.0f), Vec2(5.0f, 1.75f));
	quitConfirmNoRenderer = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<SpriteRendererComponent>(
			quitNoEntity->entityID, true, 112, true, cream, true);
	quitConfirmNoRenderer->texture = quitConfirmNoTexture;
	quitConfirmNoRenderer->texturePath = noTexturePath;
	quitConfirmNoButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(quitNoEntity->entityID);
	AttachChild(quitRoot, quitNoEntity);
}

void MainMenu::ShowCredits(bool show)
{
	DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(creditsScreen->entityID)->isVisible = show;
	DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = !show;

	if (!show && isUsingController)
	{
		SelectButton(MenuSelection::CREDITS);
	}
}

void MainMenu::ShowQuitConfirmation(bool show)
{
	DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(quitConfirmScreen->entityID)->isVisible = show;
	DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = !show;

	quitConfirmationSelectsYes = false;
	quitConfirmYesRenderer->texture = quitConfirmYesTexture;
	quitConfirmNoRenderer->texture = show && isUsingController
		? quitConfirmNoHoverTexture
		: quitConfirmNoTexture;

	if (!show && isUsingController)
	{
		SelectButton(MenuSelection::QUIT);
	}
}

void MainMenu::SelectQuitConfirmation(bool selectYes)
{
	quitConfirmationSelectsYes = selectYes;
	quitConfirmYesRenderer->texture = selectYes
		? quitConfirmYesHoverTexture
		: quitConfirmYesTexture;
	quitConfirmNoRenderer->texture = selectYes
		? quitConfirmNoTexture
		: quitConfirmNoHoverTexture;
	StartSound->Play();
}

void MainMenu::UpdateQuitConfirmationInput()
{
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE) ||
		DuckEngine_Input::IsGamepadButtonPressed(
			DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B))
	{
		ShowQuitConfirmation(false);
		return;
	}

	if (!DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
	{
		return;
	}

	if (controllerNavigationCooldown > 0.0f)
	{
		controllerNavigationCooldown -= DuckEngine::DeltaTime();
	}

	const float leftInput = DuckEngine_Input::GetGamepadAxisValue(
		DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
	const float rightInput = DuckEngine_Input::GetGamepadAxisValue(
		DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_X);
	const bool navigateLeft = leftInput < -0.3f || rightInput < -0.3f ||
		DuckEngine_Input::IsGamepadButtonDown(
			DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT);
	const bool navigateRight = leftInput > 0.3f || rightInput > 0.3f ||
		DuckEngine_Input::IsGamepadButtonDown(
			DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);
	const bool activate = DuckEngine_Input::IsGamepadButtonReleased(
		DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

	if ((navigateLeft || navigateRight || activate) && !isUsingController)
	{
		isUsingController = true;
		SelectQuitConfirmation(false);
		controllerNavigationCooldown = controllerNavigationDelay;
	}

	if (controllerNavigationCooldown <= 0.0f)
	{
		if (navigateLeft)
		{
			SelectQuitConfirmation(true);
			controllerNavigationCooldown = controllerNavigationDelay;
		}
		else if (navigateRight)
		{
			SelectQuitConfirmation(false);
			controllerNavigationCooldown = controllerNavigationDelay;
		}
	}

	if (activate)
	{
		if (quitConfirmationSelectsYes)
		{
			GameManager::Engine.CloseWindow();
		}
		else
		{
			ShowQuitConfirmation(false);
		}
	}
}

void MainMenu::UpdateMenuSelection()
{
	if ((DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
		 DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_RIGHT)) &&
		isUsingController)
	{
		DeselectAllButtons();
		quitConfirmYesRenderer->texture = quitConfirmYesTexture;
		quitConfirmNoRenderer->texture = quitConfirmNoTexture;
		isUsingController = false;
	}

	// First, check if any submenus are open - don't allow main menu navigation if they are
	bool submenusOpen = false;

	if (levelSelectScreen && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible) {
		submenusOpen = true;
	}

	if (HTPScreen && DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible) {
		submenusOpen = true;
	}

	const bool creditsOpen = creditsScreen &&
		DuckEngine::DUCKENGINE_ComponentManager
			.GetComponent<SpriteRendererComponent>(creditsScreen->entityID)->isVisible;
	const bool quitConfirmationOpen = quitConfirmScreen &&
		DuckEngine::DUCKENGINE_ComponentManager
			.GetComponent<SpriteRendererComponent>(quitConfirmScreen->entityID)->isVisible;
	submenusOpen = submenusOpen || creditsOpen || quitConfirmationOpen;

	if (quitConfirmationOpen)
	{
		UpdateQuitConfirmationInput();
		return;
	}

	if (creditsOpen)
	{
		if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ESCAPE) ||
			DuckEngine_Input::IsGamepadButtonPressed(
				DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A) ||
			DuckEngine_Input::IsGamepadButtonPressed(
				DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B))
		{
			ShowCredits(false);
		}
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
		float verticalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_Y);
		float rightVerticalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_Y);
		bool dpadUp = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP);
		bool dpadDown = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN);

		// Check for any controller input
		bool hasControllerInput = std::abs(verticalInput) > 0.3f || std::abs(rightVerticalInput) > 0.3f || dpadUp || dpadDown ||
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
			if (controllerNavigationCooldown <= 0 && (verticalInput < -0.3f || rightVerticalInput < -0.3f || dpadUp))
			{
				int newSelection = static_cast<int>(currentSelection) - 1;
				if (newSelection < 0)
					newSelection = static_cast<int>(MenuSelection::COUNT) - 1;

				SelectButton(static_cast<MenuSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			// Navigate down
			else if (controllerNavigationCooldown <= 0 && (verticalInput > 0.3f || rightVerticalInput > 0.3f || dpadDown))
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

	case MenuSelection::CREDITS:
		creditsButtonRenderer->color = Color(255.0f, 224.0f, 153.0f, 255.0f);
		creditsButtonText->color = Color(82.0f, 38.0f, 24.0f, 255.0f);
		StartSound->Play();
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
	creditsButtonRenderer->color = Color(82.0f, 38.0f, 24.0f, 230.0f);
	creditsButtonText->color = Color(255.0f, 241.0f, 220.0f, 255.0f);
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
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID)->isVisible = true;
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
		}
		break;

	case MenuSelection::HOW_TO_PLAY:
		if (!isFadingOut)
		{
			HtpSound->Play(1);
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(HTPScreen->entityID)->isVisible = true;
			DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID)->isVisible = false;
		}
		break;

	case MenuSelection::CREDITS:
		if (!isFadingOut)
		{
			StartSound->Play(1);
			ShowCredits(true);
		}
		break;

	case MenuSelection::QUIT:
		if (!isFadingOut)
		{
			QuitSound->Play(1);
			ShowQuitConfirmation(true);
		}
		break;

	default:
		break;
	}
}
