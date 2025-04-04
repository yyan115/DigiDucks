#include "ExitConfirmLogic.h"
#include "GameManager.h"

void ExitConfirmLogic::Start()
{
	auto gamePauseBg = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Pause_Menu").get();
	if (gamePauseBg)
	{
		pauseMenuLogic = GameLogicManager::GetLogicForEntity<PauseMenuLogic>(gamePauseBg->entityID);
	}
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
		gameExitYesBtnTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(gameExitYesBtn->entityID);
		if (gameExitYesBtnTransform)
		{
			gameExitYesBtnOriginalScale = gameExitYesBtnTransform->scale;
		}
		gameExitYesBtn_Normal = AssetManager::GetTextureByName("exit_yes");
		gameExitYesBtn_Hover = AssetManager::GetTextureByName("exit_yes_hover");
		gameExitYesButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitYesBtn->entityID);
		gameExitYesBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitYesBtn->entityID);
		if (gameExitYesButton)
		{
			gameExitYesButton->onClick = [this]() {
				gameExitYesBtnSound->Resume();
				gameExitYesBtnSound->Play();
				DuckEngine::PauseGame(false);
				// need to transition
				GameManager::SetActiveScene("MainMenu"); };
			gameExitYesButton->onHover = [this]() {
				gameExitYesBtnSound->Resume();
				gameExitYesBtnSound->Play(1);
				gameExitYesBtnSpt->texture = gameExitYesBtn_Hover;
				if (gameExitYesBtnTransform) {
					gameExitYesBtnTransform->scale = gameExitYesBtnOriginalScale * buttonScaleIncrease;
				} };
			gameExitYesButton->onFinishHover = [this]() {
				gameExitYesBtnSpt->texture = gameExitYesBtn_Normal;
				if (gameExitYesBtnTransform) {
					gameExitYesBtnTransform->scale = gameExitYesBtnOriginalScale;
				} };
		}
	}
	auto gameExitNoBtn = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Exit_No_Btn").get();
	if (gameExitNoBtn)
	{
		gameExitNoBtnSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameExitNoBtn->entityID);
		gameExitNoBtnTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(gameExitNoBtn->entityID);
		if (gameExitNoBtnTransform)
		{
			gameExitNoBtnOriginalScale = gameExitNoBtnTransform->scale;
		}
		gameExitNoBtn_Normal = AssetManager::GetTextureByName("exit_no");
		gameExitNoBtn_Hover = AssetManager::GetTextureByName("exit_no_hover");
		gameExitNoButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameExitNoBtn->entityID);
		gameExitNoBtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameExitNoBtn->entityID);
		if (gameExitNoButton)
		{
			gameExitNoButton->onClick = [this]() {
				gameExitNoBtnSound->Resume();
				gameExitNoBtnSound->Play();
				if (pauseMenuLogic) {
					// Hide the exit confirmation first
					if (gameExitCfmBg && gameExitCfmBgSpt)
					{
						gameExitCfmBgSpt->isVisible = false;
					}

					pauseMenuLogic->ExitConfirm(false);
					pauseMenuLogic->DisableButtons(false);
				}
				};
			gameExitNoButton->onHover = [this]() {
				gameExitNoBtnSound->Resume();
				gameExitNoBtnSound->Play(1);
				gameExitNoBtnSpt->texture = gameExitNoBtn_Hover;
				if (gameExitNoBtnTransform) {
					gameExitNoBtnTransform->scale = gameExitNoBtnOriginalScale * buttonScaleIncrease;
				} };
			gameExitNoButton->onFinishHover = [this]() {
				gameExitNoBtnSpt->texture = gameExitNoBtn_Normal;
				if (gameExitNoBtnTransform) {
					gameExitNoBtnTransform->scale = gameExitNoBtnOriginalScale;
				} };
		}
	}

	// Set default selection for gamepad (YES as default)
	currentSelection = MenuSelection::YES;
	isUsingController = false;
}

void ExitConfirmLogic::Update()
{
	// Only update gamepad controls if the exit confirmation is visible
	if (gameExitCfmBg && gameExitCfmBgSpt && gameExitCfmBgSpt->isVisible)
	{
		UpdateMenuSelection();
	}
}

void ExitConfirmLogic::UpdateMenuSelection()
{
	if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
	{
		if (controllerNavigationCooldown > 0)
		{
			controllerNavigationCooldown -= DuckEngine::PauseDeltaTime();
		}

		float horizontalInput = DuckEngine_Input::GetGamepadAxisValue(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_AXIS_LEFT_X);
		bool dpadLeft = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_LEFT);
		bool dpadRight = DuckEngine_Input::IsGamepadButtonDown(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_DPAD_RIGHT);

		bool hasControllerInput = std::abs(horizontalInput) > 0.3f || dpadLeft || dpadRight ||
			DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

		if (hasControllerInput && !isUsingController)
		{
			isUsingController = true;
			SelectButton(MenuSelection::YES); // Default to YES
			controllerNavigationCooldown = controllerNavigationDelay;
		}

		if (isUsingController)
		{
			if (controllerNavigationCooldown <= 0 && (horizontalInput < -0.3f || dpadLeft))
			{
				SelectButton(MenuSelection::YES);
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			else if (controllerNavigationCooldown <= 0 && (horizontalInput > 0.3f || dpadRight))
			{
				SelectButton(MenuSelection::NO);
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
			{
				ActivateSelectedButton();
			}

			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B))
			{
				// B button cancels the exit confirmation (same as No)
				if (pauseMenuLogic) {
					std::cout << "B button pressed - closing exit confirmation" << std::endl;

					// Hide the exit confirmation first
					if (gameExitCfmBg && gameExitCfmBgSpt)
					{
						gameExitCfmBgSpt->isVisible = false;
					}

					// Add a small delay to prevent immediate button processing
					pauseMenuLogic->gamepadDown = true;

					pauseMenuLogic->ExitConfirm(false);
					pauseMenuLogic->DisableButtons(false); // Ensure pause menu buttons are enabled
				}
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

void ExitConfirmLogic::SelectButton(MenuSelection selection)
{
	// Deselect all buttons first
	DeselectAllButtons();

	// Update current selection
	currentSelection = selection;

	// Apply hover effect to the selected button
	switch (selection)
	{
	case MenuSelection::YES:
		if (gameExitYesBtnSpt)
		{
			gameExitYesBtnSpt->texture = gameExitYesBtn_Hover;
			if (gameExitYesBtnTransform)
			{
				gameExitYesBtnTransform->scale = gameExitYesBtnOriginalScale * buttonScaleIncrease;
			}
			if (gameExitYesBtnSound)
			{
				gameExitYesBtnSound->Resume();
				gameExitYesBtnSound->Play(1);
			}
		}
		break;

	case MenuSelection::NO:
		if (gameExitNoBtnSpt)
		{
			gameExitNoBtnSpt->texture = gameExitNoBtn_Hover;
			if (gameExitNoBtnTransform)
			{
				gameExitNoBtnTransform->scale = gameExitNoBtnOriginalScale * buttonScaleIncrease;
			}
			if (gameExitNoBtnSound)
			{
				gameExitNoBtnSound->Resume();
				gameExitNoBtnSound->Play(1);
			}
		}
		break;

	default:
		break;
	}
}

void ExitConfirmLogic::DeselectAllButtons()
{
	if (gameExitYesBtnSpt)
	{
		gameExitYesBtnSpt->texture = gameExitYesBtn_Normal;
	}

	if (gameExitYesBtnTransform)
	{
		gameExitYesBtnTransform->scale = gameExitYesBtnOriginalScale;
	}

	if (gameExitNoBtnSpt)
	{
		gameExitNoBtnSpt->texture = gameExitNoBtn_Normal;
	}

	if (gameExitNoBtnTransform)
	{
		gameExitNoBtnTransform->scale = gameExitNoBtnOriginalScale;
	}
}

void ExitConfirmLogic::ActivateSelectedButton()
{
	switch (currentSelection)
	{
	case MenuSelection::YES:
		if (gameExitYesBtnSound)
		{
			gameExitYesBtnSound->Resume();
			gameExitYesBtnSound->Play();
		}
		// Transition to main menu
		DuckEngine::PauseGame(false);
		GameManager::GamepadDown = true;
		GameManager::SetActiveScene("MainMenu");
		break;

	case MenuSelection::NO:
		if (gameExitNoBtnSound)
		{
			gameExitNoBtnSound->Resume();
			gameExitNoBtnSound->Play();
		}
		if (pauseMenuLogic)
		{
			std::cout << "Closing exit confirmation, returning to pause menu" << std::endl;

			// Hide the exit confirmation first
			if (gameExitCfmBg && gameExitCfmBgSpt)
			{
				gameExitCfmBgSpt->isVisible = false;
			}

			pauseMenuLogic->gamepadDown = true;

			// Now tell the pause menu logic to handle the rest
			pauseMenuLogic->ExitConfirm(false);
			pauseMenuLogic->DisableButtons(false); // Ensure buttons are enabled
		}
		break;

	default:
		break;
	}
}

void ExitConfirmLogic::FixedUpdate()
{
}

void ExitConfirmLogic::ShowExitConfirm(bool state)
{
	if (gameExitCfmBg && gameExitCfmBgSpt)
	{
		gameExitCfmBgSpt->isVisible = state;

		// Reset selection to YES when showing the exit confirmation
		if (state)
		{
			if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
			{
				isUsingController = true;
				SelectButton(MenuSelection::YES);
			}
		}
	}
}