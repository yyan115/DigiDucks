#pragma once
#include "DuckEngine.h"
#include "GameLogicComponent.h"
#include "AssetManager.h"
#include <vector>

class MainMenu;

class LevelSelectScreenLogic : public GameLogic
{
public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<LevelSelectScreenLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	// Static variables to track player's progress through levels
	static int currentStage; // Determines which levels are available
	static int stageLevel;   // Current level being played

	// Function to call when a level is completed
	static void LevelCompleted();

private:
	MainMenu* mainMenu = nullptr;

	Entity* levelSelectScreen = nullptr;
	Entity* mainMenuScreen = nullptr;

	SpriteRendererComponent* levelSelectScreenSpriteRenderer = nullptr;
	SpriteRendererComponent* mainMenuScreenSpriteRenderer = nullptr;

	// Store original textures for each button
	std::vector<Texture> originalTextures;

	// Helper function to update level button visuals based on currentStage
	void UpdateLevelButtonVisuals(
		SpriteRendererComponent* level0Sprite,
		SpriteRendererComponent* level1Sprite,
		SpriteRendererComponent* level1_5Sprite,
		SpriteRendererComponent* level2Sprite,
		SpriteRendererComponent* level2_5Sprite,
		SpriteRendererComponent* level3Sprite,
		SpriteRendererComponent* level3_5Sprite
	);
};