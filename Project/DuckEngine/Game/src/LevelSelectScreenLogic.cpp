/******************************************************************************/
/*!
\file       LevelSelectScreenLogic.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Implements the LevelSelectScreenLogic class. Handles dynamic level
			button behavior in the level selection screen, including input
			handling, level lock/unlock visuals, and transition logic.

			Features include:
			- Controller navigation with joystick/D-pad
			- Button scaling animation on hover
			- Progress-based unlocks using SaveLoadManager
			- Support for both mouse and gamepad users

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "LevelSelectScreenLogic.h"
#include "MainMenu.h"
#include "DuckEngine_Input.h"
#include "SaveLoadManager.h"

int LevelSelectScreenLogic::currentStage = -1;
int LevelSelectScreenLogic::stageLevel = 0;


//LevelSelectScreen
void LevelSelectScreenLogic::Start()
{
	if (currentStage < SaveLoadManager::currentLevel)
	{
		currentStage = SaveLoadManager::currentLevel;
	}

	levelSelectScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();
	mainMenuScreen = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MainMenuScreen").get();

	Entity* XButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("LevelSelectXButton").get();
	ButtonComponent* XButton = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(XButtonEntity->entityID);
	SoundComponent* XButtonSFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(XButtonEntity->entityID);

	// Get all level button entities
	Entity* level0ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level0Button").get();
	ButtonComponent* level0Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level0ButtonEntity->entityID);
	SpriteRendererComponent* level0Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level0ButtonEntity->entityID);
	SoundComponent* level0SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level0ButtonEntity->entityID);

	Entity* level1ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level1Button").get();
	ButtonComponent* level1Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level1ButtonEntity->entityID);
	SpriteRendererComponent* level1Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level1ButtonEntity->entityID);
	SoundComponent* level1SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level1ButtonEntity->entityID);

	Entity* level1_5ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level1.5Button").get();
	ButtonComponent* level1_5Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level1_5ButtonEntity->entityID);
	SpriteRendererComponent* level1_5Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level1_5ButtonEntity->entityID);
	SoundComponent* level1_5SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level1_5ButtonEntity->entityID);

	Entity* level2ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level2Button").get();
	ButtonComponent* level2Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level2ButtonEntity->entityID);
	SpriteRendererComponent* level2Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level2ButtonEntity->entityID);
	SoundComponent* level2SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level2ButtonEntity->entityID);

	Entity* level2_5ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level2.5Button").get();
	ButtonComponent* level2_5Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level2_5ButtonEntity->entityID);
	SpriteRendererComponent* level2_5Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level2_5ButtonEntity->entityID);
	SoundComponent* level2_5SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level2_5ButtonEntity->entityID);

	Entity* level3ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level3Button").get();
	ButtonComponent* level3Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level3ButtonEntity->entityID);
	SpriteRendererComponent* level3Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level3ButtonEntity->entityID);
	SoundComponent* level3SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level3ButtonEntity->entityID);

	Entity* level3_5ButtonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Level3.5Button").get();
	ButtonComponent* level3_5Button = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(level3_5ButtonEntity->entityID);
	SpriteRendererComponent* level3_5Sprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(level3_5ButtonEntity->entityID);
	SoundComponent* level3_5SFX = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(level3_5ButtonEntity->entityID);

	levelSelectScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(levelSelectScreen->entityID);
	mainMenuScreenSpriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(mainMenuScreen->entityID);

	mainMenu = DuckEngine::DUCKENGINE_SceneManager.GetScene<MainMenu>("MainMenu").get();

	// X Button to return to main menu
	XButton->onClick = [this, XButtonSFX]()
		{
			XButtonSFX->Play();
			levelSelectScreenSpriteRenderer->isVisible = false;
			mainMenuScreenSpriteRenderer->isVisible = true;
		};

	// Level 0 (Tutorial) - Always available
	level0Button->onClick = [this, XButtonSFX]()
		{
			std::cout << "Level 0 button clicked!" << std::endl;
			XButtonSFX->Play();
			stageLevel = 0;
			mainMenu->OnPlayButtonClicked("Level0");

			// Disable all buttons when start button is clicked
			for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
			{
				ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
				if (!button) continue;

				button->isEnabled = false;
			}
		};
	level0Button->onHover = [this, level0SFX]()
		{
			if (!isUsingController)
			{
				level0SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL0);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level0Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL0);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	// Level 1 - Available if currentStage >= 0
	level1Button->onClick = [this, XButtonSFX]()
		{
			if (currentStage >= 0)
			{
				std::cout << "Level 1 button clicked!" << std::endl;
				XButtonSFX->Play();
				stageLevel = 1;
				mainMenu->OnPlayButtonClicked("Level1");

				// Disable all buttons when start button is clicked
				for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
				{
					ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
					if (!button) continue;

					button->isEnabled = false;
				}
			}
		};
	level1Button->onHover = [this, level1SFX]()
		{
			if (!isUsingController)
			{
				level1SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL1);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level1Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL1);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	// Level 1.5 - Available if currentStage >= 1
	level1_5Button->onClick = [this, XButtonSFX]()
		{
			if (currentStage >= 1)
			{
				std::cout << "Level 1.5 button clicked!" << std::endl;
				XButtonSFX->Play();
				stageLevel = 2;
				mainMenu->OnPlayButtonClicked("Level1_5");

				// Disable all buttons when start button is clicked
				for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
				{
					ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
					if (!button) continue;

					button->isEnabled = false;
				}
			}
		};
	level1_5Button->onHover = [this, level1_5SFX]()
		{
			if (!isUsingController)
			{
				level1_5SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL1_5);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level1_5Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL1_5);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	// Level 2 - Available if currentStage >= 2
	level2Button->onClick = [this, XButtonSFX]()
		{
			if (currentStage >= 2)
			{
				std::cout << "Level 2 button clicked!" << std::endl;
				XButtonSFX->Play();
				stageLevel = 3;
				mainMenu->OnPlayButtonClicked("Level2");

				// Disable all buttons when start button is clicked
				for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
				{
					ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
					if (!button) continue;

					button->isEnabled = false;
				}
			}
		};
	level2Button->onHover = [this, level2SFX]()
		{
			if (!isUsingController)
			{
				level2SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL2);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level2Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL2);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	// Level 2.5 - Available if currentStage >= 3
	level2_5Button->onClick = [this, XButtonSFX]()
		{
			if (currentStage >= 3)
			{
				std::cout << "Level 2.5 button clicked!" << std::endl;
				XButtonSFX->Play();
				stageLevel = 4;
				mainMenu->OnPlayButtonClicked("Level2_5");

				// Disable all buttons when start button is clicked
				for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
				{
					ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
					if (!button) continue;

					button->isEnabled = false;
				}
			}
		};
	level2_5Button->onHover = [this, level2_5SFX]()
		{
			if (!isUsingController)
			{
				level2_5SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL2_5);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level2_5Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL2_5);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	// Level 3 - Available if currentStage >= 4
	level3Button->onClick = [this, XButtonSFX]()
		{
			if (currentStage >= 4)
			{
				std::cout << "Level 3 button clicked!" << std::endl;
				XButtonSFX->Play();
				stageLevel = 5;
				mainMenu->OnPlayButtonClicked("Level3");

				// Disable all buttons when start button is clicked
				for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
				{
					ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
					if (!button) continue;

					button->isEnabled = false;
				}
			}
		};
	level3Button->onHover = [this, level3SFX]()
		{
			if (!isUsingController)
			{
				level3SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL3);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level3Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL3);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	// Level 3.5 - Available if currentStage >= 5
	level3_5Button->onClick = [this, XButtonSFX]()
		{
			if (currentStage >= 5)
			{
				std::cout << "Level 3.5 button clicked!" << std::endl;
				XButtonSFX->Play();
				stageLevel = 6;
				mainMenu->OnPlayButtonClicked("Level3_5");

				// Disable all buttons when start button is clicked
				for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<ButtonComponent>())
				{
					ButtonComponent* button = static_cast<ButtonComponent*>(component.get());
					if (!button) continue;

					button->isEnabled = false;
				}
			}
		};
	level3_5Button->onHover = [this, level3_5SFX]()
		{
			if (!isUsingController)
			{
				level3_5SFX->Play();
				int index = static_cast<int>(LevelButtonSelection::LEVEL3_5);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
				}
			}
		};
	level3_5Button->onFinishHover = [this]()
		{
			if (!isUsingController)
			{
				int index = static_cast<int>(LevelButtonSelection::LEVEL3_5);
				if (index < levelButtonTransforms.size())
				{
					levelButtonTransforms[index]->scale = originalScales[index];
				}
			}
		};

	UpdateLevelButtonVisuals(level0Sprite, level1Sprite, level1_5Sprite, level2Sprite,
		level2_5Sprite, level3Sprite, level3_5Sprite);

	levelButtonEntities.clear();
	levelButtonTransforms.clear();
	originalScales.clear();

	// Store buttons in the order we want to navigate
	levelButtonEntities.push_back(level0ButtonEntity);
	levelButtonEntities.push_back(level1ButtonEntity);
	levelButtonEntities.push_back(level1_5ButtonEntity);
	levelButtonEntities.push_back(level2ButtonEntity);
	levelButtonEntities.push_back(level2_5ButtonEntity);
	levelButtonEntities.push_back(level3ButtonEntity);
	levelButtonEntities.push_back(level3_5ButtonEntity);

	// Get transform components for each button and store original scales
	for (auto entity : levelButtonEntities)
	{
		if (entity)
		{
			TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID);
			if (transform)
			{
				levelButtonTransforms.push_back(transform);
				originalScales.push_back(transform->scale);
			}
		}
	}

	// Store X button separately
	xButtonEntity = XButtonEntity;
	if (xButtonEntity)
	{
		xButtonTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(xButtonEntity->entityID);
		if (xButtonTransform)
		{
			xButtonOriginalScale = xButtonTransform->scale;
		}
	}
}

void LevelSelectScreenLogic::Update()
{
	if (levelSelectScreenSpriteRenderer && levelSelectScreenSpriteRenderer->isVisible)
	{
		if (DuckEngine_Input::IsGamepadConnected(DuckEngine_Input::GAMEPAD_1))
		{
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_B))
			{
				DeselectAllLevelButtons();
				levelSelectScreenSpriteRenderer->isVisible = false;
				mainMenuScreenSpriteRenderer->isVisible = true;
				isUsingController = false;
			}
		}

		UpdateLevelMenuSelection();
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_0) && DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_SHIFT))
	{
		currentStage = 100;
		Start();
	}
}

void LevelSelectScreenLogic::FixedUpdate()
{
}

void LevelSelectScreenLogic::UpdateLevelButtonVisuals(
	SpriteRendererComponent* level0Sprite,
	SpriteRendererComponent* level1Sprite,
	SpriteRendererComponent* level1_5Sprite,
	SpriteRendererComponent* level2Sprite,
	SpriteRendererComponent* level2_5Sprite,
	SpriteRendererComponent* level3Sprite,
	SpriteRendererComponent* level3_5Sprite)
{
	std::string lockedTexture = "lvlselectbutton_locked";
	std::string unlockedTexture = "lvlselectbutton";

	std::string level0LockedTexture = "lvlselectbutton_locked_knife";
	std::string level0UnlockedTexture = "lvlselectbutton_knife";

	std::string level2LockedTexture = "lvlselectbutton_locked_pan";
	std::string level2UnlockedTexture = "lvlselectbutton_pan";

	std::string level4LockedTexture = "lvlselectbutton_locked_soup";
	std::string level4UnlockedTexture = "lvlselectbutton_soup";


	level0Sprite->texture = AssetManager::GetTextureByName(level0UnlockedTexture);

	level1Sprite->texture = AssetManager::GetTextureByName((currentStage >= 0) ? unlockedTexture : lockedTexture);
	level1_5Sprite->texture = AssetManager::GetTextureByName((currentStage >= 1) ? level2UnlockedTexture : level2LockedTexture);
	level2Sprite->texture = AssetManager::GetTextureByName((currentStage >= 2) ? unlockedTexture : lockedTexture);
	level2_5Sprite->texture = AssetManager::GetTextureByName((currentStage >= 3) ? level4UnlockedTexture : level4LockedTexture);
	level3Sprite->texture = AssetManager::GetTextureByName((currentStage >= 4) ? unlockedTexture : lockedTexture);
	level3_5Sprite->texture = AssetManager::GetTextureByName((currentStage >= 5) ? unlockedTexture : lockedTexture);
}


void LevelSelectScreenLogic::LevelCompleted()
{
	currentStage++;

	if (currentStage > 5)
	{
		currentStage = 5;
	}

	if (currentStage > SaveLoadManager::currentLevel) 
	{
		SaveLoadManager::currentLevel = currentStage;
		SaveLoadManager::SaveGame();
		std::cout << "Game progress saved: Level " << currentStage << std::endl;
	}

}

void LevelSelectScreenLogic::UpdateLevelMenuSelection()
{
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
			DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A);

		// If this is the first controller input, select the first level button
		if (hasControllerInput && !isUsingController)
		{
			isUsingController = true;
			SelectLevelButton(LevelButtonSelection::LEVEL0);
			controllerNavigationCooldown = controllerNavigationDelay;
		}

		// Only process navigation if we're using controller
		if (isUsingController)
		{
			// Navigate left
			if (controllerNavigationCooldown <= 0 && (horizontalInput < -0.3f || dpadLeft))
			{
				int newSelection = static_cast<int>(currentLevelSelection) - 1;
				if (newSelection < 0)
				{
					newSelection = static_cast<int>(LevelButtonSelection::COUNT) - 1;
				}

				SelectLevelButton(static_cast<LevelButtonSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}
			// Navigate right
			else if (controllerNavigationCooldown <= 0 && (horizontalInput > 0.3f || dpadRight))
			{
				int newSelection = (static_cast<int>(currentLevelSelection) + 1) % static_cast<int>(LevelButtonSelection::COUNT);
				SelectLevelButton(static_cast<LevelButtonSelection>(newSelection));
				controllerNavigationCooldown = controllerNavigationDelay;
			}

			// Activate selected button with X button (A on Xbox)
			if (DuckEngine_Input::IsGamepadButtonPressed(DuckEngine_Input::GAMEPAD_1, DuckEngine_Input::GAMEPAD_BUTTON_A))
			{
				ActivateSelectedLevelButton();
			}
		}
	}
	else
	{
		// Reset controller usage flag when no gamepad is connected
		if (isUsingController)
		{
			DeselectAllLevelButtons();
			isUsingController = false;
		}
	}

	// Switch back to mouse mode if mouse movement is detected
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT) ||
		DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_RIGHT))
	{
		if (isUsingController)
		{
			DeselectAllLevelButtons();
			isUsingController = false;
		}
	}
}

void LevelSelectScreenLogic::SelectLevelButton(LevelButtonSelection selection)
{
	// Deselect all buttons first
	DeselectAllLevelButtons();

	// Update current selection
	currentLevelSelection = selection;

	// Apply hover effect (increase scale by 10%)

	int index = static_cast<int>(selection);
	if (index >= 0 && index < levelButtonTransforms.size())
	{
		levelButtonTransforms[index]->scale = originalScales[index] * buttonScaleIncrease;
	}
}

void LevelSelectScreenLogic::DeselectAllLevelButtons()
{
	// Reset all button scales
	for (size_t i = 0; i < levelButtonTransforms.size(); i++)
	{
		if (i < originalScales.size() && levelButtonTransforms[i])
		{
			levelButtonTransforms[i]->scale = originalScales[i];
		}
	}

	if (xButtonTransform)
	{
		xButtonTransform->scale = xButtonOriginalScale;
	}
}

void LevelSelectScreenLogic::ActivateSelectedLevelButton()
{
	switch (currentLevelSelection)
	{
	case LevelButtonSelection::LEVEL0:
		std::cout << "Level 0 button activated!" << std::endl;
		if (SFX) SFX->Play();
		stageLevel = 0;
		mainMenu->OnPlayButtonClicked("Level0");
		break;

	case LevelButtonSelection::LEVEL1:
		if (currentStage >= 0)
		{
			std::cout << "Level 1 button activated!" << std::endl;
			if (SFX) SFX->Play();
			stageLevel = 1;
			mainMenu->OnPlayButtonClicked("Level1");
		}
		break;

	case LevelButtonSelection::LEVEL1_5:
		if (currentStage >= 1)
		{
			std::cout << "Level 1.5 button activated!" << std::endl;
			if (SFX) SFX->Play();
			stageLevel = 2;
			mainMenu->OnPlayButtonClicked("Level1_5");
		}
		break;

	case LevelButtonSelection::LEVEL2:
		if (currentStage >= 2)
		{
			std::cout << "Level 2 button activated!" << std::endl;
			if (SFX) SFX->Play();
			stageLevel = 3;
			mainMenu->OnPlayButtonClicked("Level2");
		}
		break;

	case LevelButtonSelection::LEVEL2_5:
		if (currentStage >= 3)
		{
			std::cout << "Level 2.5 button activated!" << std::endl;
			if (SFX) SFX->Play();
			stageLevel = 4;
			mainMenu->OnPlayButtonClicked("Level2_5");
		}
		break;

	case LevelButtonSelection::LEVEL3:
		if (currentStage >= 4)
		{
			std::cout << "Level 3 button activated!" << std::endl;
			if (SFX) SFX->Play();
			stageLevel = 5;
			mainMenu->OnPlayButtonClicked("Level3");
		}
		break;

	case LevelButtonSelection::LEVEL3_5:
		if (currentStage >= 5)
		{
			std::cout << "Level 3.5 button activated!" << std::endl;
			if (SFX) SFX->Play();
			stageLevel = 6;
			mainMenu->OnPlayButtonClicked("Level3_5");
		}
		break;

	default:
		break;
	}
}