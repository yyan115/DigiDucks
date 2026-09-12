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
#include "WindowManager.h"

namespace
{
	std::shared_ptr<Entity> CreatePauseUIEntity(
		const std::string& name,
		const Vec2& position,
		const Vec2& scale,
		const std::string& texturePath,
		int sortingOrder,
		bool visible = true)
	{
		auto entity = DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
		entity->name = name;
		entity->layerName = "UI";

		auto* transform = DuckEngine::DUCKENGINE_ComponentManager
			.AddComponent<TransformComponent>(entity->entityID);
		transform->angle = 0.0f;
		transform->scale = scale;
		transform->relativeToCamera = false;
		transform->SetPosition(position);

		auto* renderer = DuckEngine::DUCKENGINE_ComponentManager
			.AddComponent<SpriteRendererComponent>(
				entity->entityID, true, sortingOrder, false, Color(), visible);
		if (auto texture = AssetManager::GetTexture(texturePath))
		{
			renderer->texture = *texture;
			renderer->texturePath = texturePath;
		}

		if (auto* scene = DuckEngine::DUCKENGINE_SceneManager.GetActiveScene())
		{
			scene->AddEntityToLayer("UI", entity.get());
		}
		return entity;
	}

	void AttachPauseChild(
		const std::shared_ptr<Entity>& parent,
		const std::shared_ptr<Entity>& child)
	{
		if (!parent || !child)
		{
			return;
		}
		parent->childEntities.push_back(child);
		parent->childNames.push_back(child->name);
	}
}

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
						PauseMenuSound->Play(1);
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

	// Stars
	{
		star1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_1").get();
		if (star1)
		{
			star1Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(star1->entityID);
			if (star1Spt)
			{
				star1Spt->isVisible = false;
			}
		}
		star2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_2").get();
		if (star2)
		{
			star2Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(star2->entityID);
			if (star2Spt)
			{
				star2Spt->isVisible = false;
			}
		}
		star3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_3").get();
		if (star3)
		{
			star3Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(star3->entityID);
			if (star3Spt)
			{
				star3Spt->isVisible = false;
			}
		}
	}

	auto PauseMenuSFX = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("PauseMenuSFXManager").get();
	if (PauseMenuSFX)
	{
		PauseMenuSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(PauseMenuSFX->entityID);
	}

	CreateQuitGameControls();

	isPaused = false;
	pageNumb = 1;

	PauseGame(false);
}

void PauseMenuLogic::CreateQuitGameControls()
{
	// Make room for a fourth option without changing the serialized scenes.
	const struct ButtonPosition
	{
		SpriteRendererComponent* renderer;
		const char* entityName;
		float y;
	} positions[] = {
		{ gameResumeBtnSpt, "Resume_Btn", -0.40f },
		{ gameHTPBtnSpt, "HTP_Btn", -0.56f },
		{ gameExitBtnSpt, "Quit_Btn", -0.72f },
	};
	for (const auto& position : positions)
	{
		if (!position.renderer)
		{
			continue;
		}
		auto entity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(position.entityName);
		if (entity)
		{
			if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager
				.GetComponent<TransformComponent>(entity->entityID))
			{
				transform->SetPosition(Vec2(0.5f, position.y));
			}
		}
	}

	auto pauseRoot = gamePauseBg
		? DuckEngine::DUCKENGINE_EntityManager.GetEntity(gamePauseBg->entityID)
		: nullptr;
	if (!pauseRoot)
	{
		return;
	}

	const std::string quitTexturePath =
		"Resources/Sprites/Pause/pause_quitgame.png";
	const std::string quitHoverTexturePath =
		"Resources/Sprites/Pause/pause_quitgame_hover.png";
	gameQuitBtn_Normal = *AssetManager::GetTexture(quitTexturePath);
	gameQuitBtn_Hover = *AssetManager::GetTexture(quitHoverTexturePath);
	auto quitEntity = CreatePauseUIEntity(
		"Pause_Quit_Game_Btn", Vec2(0.5f, -0.88f),
		Vec2(0.18f, 0.13f), quitTexturePath, 101);
	gameQuitBtnSpt = DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(quitEntity->entityID);
	gameQuitButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(quitEntity->entityID);
	AttachPauseChild(pauseRoot, quitEntity);

	gameQuitButton->onClick = [this]()
	{
		if (!isPaused)
		{
			return;
		}
		if (gameExitBtnSound)
		{
			gameExitBtnSound->Resume();
			gameExitBtnSound->Play();
		}
		ShowQuitGameConfirmation(true);
	};
	gameQuitButton->onHover = [this]()
	{
		if (gameExitBtnSound)
		{
			gameExitBtnSound->Resume();
			gameExitBtnSound->Play(1);
		}
		gameQuitBtnSpt->texture = gameQuitBtn_Hover;
	};
	gameQuitButton->onFinishHover = [this]()
	{
		gameQuitBtnSpt->texture = gameQuitBtn_Normal;
	};

	const std::string confirmationTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/confirm_exit_game.png";
	auto confirmationRoot = CreatePauseUIEntity(
		"Quit_Game_Cfm_Bg", Vec2(0.5f, -0.5f), Vec2(1.0f, 1.0f),
		confirmationTexturePath, 105, false);
	gameQuitCfmBg = confirmationRoot.get();
	gameQuitCfmBgSpt = DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(confirmationRoot->entityID);
	AttachPauseChild(pauseRoot, confirmationRoot);

	const std::string yesTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_yes.png";
	const std::string yesHoverTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_yes_hover.png";
	const std::string noTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_no.png";
	const std::string noHoverTexturePath =
		"Resources/Sprites/Pause/Confirm Exit/exit_no_hover.png";
	gameQuitYesBtn_Normal = *AssetManager::GetTexture(yesTexturePath);
	gameQuitYesBtn_Hover = *AssetManager::GetTexture(yesHoverTexturePath);
	gameQuitNoBtn_Normal = *AssetManager::GetTexture(noTexturePath);
	gameQuitNoBtn_Hover = *AssetManager::GetTexture(noHoverTexturePath);

	auto yesEntity = CreatePauseUIEntity(
		"Quit_Game_Yes_Btn", Vec2(0.40f, -0.68f),
		Vec2(0.15f, 0.10f), yesTexturePath, 106);
	gameQuitYesBtnSpt = DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(yesEntity->entityID);
	gameQuitYesButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(yesEntity->entityID);
	AttachPauseChild(confirmationRoot, yesEntity);

	auto noEntity = CreatePauseUIEntity(
		"Quit_Game_No_Btn", Vec2(0.60f, -0.68f),
		Vec2(0.15f, 0.10f), noTexturePath, 106);
	gameQuitNoBtnSpt = DuckEngine::DUCKENGINE_ComponentManager
		.GetComponent<SpriteRendererComponent>(noEntity->entityID);
	gameQuitNoButton = DuckEngine::DUCKENGINE_ComponentManager
		.AddComponent<ButtonComponent>(noEntity->entityID);
	AttachPauseChild(confirmationRoot, noEntity);

	gameQuitYesButton->onClick = [this]()
	{
		if (gameExitBtnSound)
		{
			gameExitBtnSound->Resume();
			gameExitBtnSound->Play();
		}
		GameManager::Engine.CloseWindow();
	};
	gameQuitNoButton->onClick = [this]()
	{
		if (gameExitBtnSound)
		{
			gameExitBtnSound->Resume();
			gameExitBtnSound->Play();
		}
		ShowQuitGameConfirmation(false);
	};
	gameQuitYesButton->onHover = [this]()
	{
		quitGameConfirmationSelectsYes = true;
		gameQuitYesBtnSpt->texture = gameQuitYesBtn_Hover;
	};
	gameQuitYesButton->onFinishHover = [this]()
	{
		gameQuitYesBtnSpt->texture = gameQuitYesBtn_Normal;
	};
	gameQuitNoButton->onHover = [this]()
	{
		quitGameConfirmationSelectsYes = false;
		gameQuitNoBtnSpt->texture = gameQuitNoBtn_Hover;
	};
	gameQuitNoButton->onFinishHover = [this]()
	{
		gameQuitNoBtnSpt->texture = gameQuitNoBtn_Normal;
	};
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
	if (gameQuitCfmBgSpt && gameQuitCfmBgSpt->isVisible)
	{
		UpdateQuitGameConfirmationInput();
		return;
	}

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
		float rightVerticalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_RIGHT_Y);
		bool dpadUp = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_UP);
		bool dpadDown = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_DOWN);

		bool hasControllerInput = std::abs(verticalInput) > 0.3f || std::abs(rightVerticalInput) > 0.3f || dpadUp || dpadDown ||
			DuckEngine_Input::IsGamepadButtonReleased(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

		if (hasControllerInput && !isUsingController)
		{
			isUsingController = true;
			SelectButton(MenuSelection::RESUME);
			controllerNavigationCooldown = controllerNavigationDelay;
		}

		if (isUsingController)
		{
			if (controllerNavigationCooldown <= 0 && (verticalInput < -0.3f || rightVerticalInput < -0.3f || dpadUp))
			{
				int newSelection = static_cast<int>(currentSelection) - 1;
				if (newSelection < 0)
					newSelection = static_cast<int>(MenuSelection::COUNT) - 1;

				SelectButton(static_cast<MenuSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			else if (controllerNavigationCooldown <= 0 && (verticalInput > 0.3f || rightVerticalInput > 0.3f || dpadDown))
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

	case MenuSelection::QUIT_GAME:
		gameQuitBtnSpt->texture = gameQuitBtn_Hover;
		if (gameExitBtnSound)
		{
			gameExitBtnSound->Play();
		}
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
	if (gameQuitBtnSpt)
	{
		gameQuitBtnSpt->texture = gameQuitBtn_Normal;
	}
}

void PauseMenuLogic::ActivateSelectedButton()
{
	if (gamepadDown)
	{
		gamepadDown = false;
		return;
	}
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

	case MenuSelection::QUIT_GAME:
		if (gameExitBtnSound)
		{
			gameExitBtnSound->Resume();
			gameExitBtnSound->Play();
		}
		ShowQuitGameConfirmation(true);
		break;

	default:
		break;
	}
}

void PauseMenuLogic::ShowQuitGameConfirmation(bool state)
{
	if (!gameQuitCfmBgSpt)
	{
		return;
	}

	gameQuitCfmBgSpt->isVisible = state;
	if (gamePauseBgSpt2)
	{
		gamePauseBgSpt2->isVisible = !state;
	}
	DisableButtons(state);

	quitGameConfirmationSelectsYes = false;
	if (gameQuitYesBtnSpt)
	{
		gameQuitYesBtnSpt->texture = gameQuitYesBtn_Normal;
	}
	if (gameQuitNoBtnSpt)
	{
		gameQuitNoBtnSpt->texture = state && isUsingController
			? gameQuitNoBtn_Hover
			: gameQuitNoBtn_Normal;
	}

	if (!state && isPaused && isUsingController)
	{
		DeselectAllButtons();
		currentSelection = MenuSelection::QUIT_GAME;
		if (gameQuitBtnSpt)
		{
			gameQuitBtnSpt->texture = gameQuitBtn_Hover;
		}
	}
}

void PauseMenuLogic::SelectQuitGameConfirmation(bool selectYes)
{
	quitGameConfirmationSelectsYes = selectYes;
	gameQuitYesBtnSpt->texture = selectYes
		? gameQuitYesBtn_Hover
		: gameQuitYesBtn_Normal;
	gameQuitNoBtnSpt->texture = selectYes
		? gameQuitNoBtn_Normal
		: gameQuitNoBtn_Hover;

	if (gameExitBtnSound)
	{
		gameExitBtnSound->Resume();
		gameExitBtnSound->Play(1);
	}
}

void PauseMenuLogic::UpdateQuitGameConfirmationInput()
{
	if (DuckEngine_Input::IsGamepadButtonPressed(
			DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B))
	{
		ShowQuitGameConfirmation(false);
		return;
	}

	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
		DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_RIGHT))
	{
		if (isUsingController)
		{
			isUsingController = false;
			gameQuitYesBtnSpt->texture = gameQuitYesBtn_Normal;
			gameQuitNoBtnSpt->texture = gameQuitNoBtn_Normal;
		}
	}

	if (!DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
	{
		return;
	}

	if (controllerNavigationCooldown > 0.0f)
	{
		controllerNavigationCooldown -= DuckEngine::PauseDeltaTime();
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
		SelectQuitGameConfirmation(false);
		controllerNavigationCooldown = controllerNavigationDelay;
	}

	if (controllerNavigationCooldown <= 0.0f)
	{
		if (navigateLeft)
		{
			SelectQuitGameConfirmation(true);
			controllerNavigationCooldown = controllerNavigationDelay;
		}
		else if (navigateRight)
		{
			SelectQuitGameConfirmation(false);
			controllerNavigationCooldown = controllerNavigationDelay;
		}
	}

	if (activate)
	{
		if (quitGameConfirmationSelectsYes)
		{
			GameManager::Engine.CloseWindow();
		}
		else
		{
			ShowQuitGameConfirmation(false);
		}
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
	DuckEngine::PauseGame(state);
	DuckEngine::isGamePaused = state;
	WindowManager::SetCursorVisible(state);

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
	if (gameJournalSpt)
	{
		gameJournalSpt->isVisible = false;
	}
	ExitConfirm(false);
	ShowQuitGameConfirmation(false);
}

void PauseMenuLogic::RequestQuitConfirmation()
{
	if (!isPaused)
	{
		PauseGame(true);
		playPauseSound();
	}
	else
	{
		if (gameJournalSpt)
		{
			gameJournalSpt->isVisible = false;
		}
		ExitConfirm(false);
	}
	ShowQuitGameConfirmation(true);
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
		gameExitBtnSpt->isVisible = !state;
	}
	if (gameHTPButton)
	{
		gameHTPButton->isEnabled = !state;
	}
	if (gameQuitButton)
	{
		gameQuitButton->isEnabled = !state;
	}

	if (star1Spt)
	{
		star1Spt->isVisible = !state;
	}
	if (star2Spt)
	{
		star2Spt->isVisible = !state;
	}
	if (star3Spt)
	{
		star3Spt->isVisible = !state;
	}
}

void PauseMenuLogic::playPauseSound()
{
	if (PauseMenuSound)
	{
		if (isPaused) PauseMenuSound->Play();
		else PauseMenuSound->Play(1);
	}
}
