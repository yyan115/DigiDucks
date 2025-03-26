#pragma once
#include "DuckEngine.h"
#include "GameLogicComponent.h"
#include "AssetManager.h"
#include <vector>

enum class LevelButtonSelection 
{
	LEVEL0 = 0,
	LEVEL1 = 1,
	LEVEL1_5 = 2,
	LEVEL2 = 3,
	LEVEL2_5 = 4,
	LEVEL3 = 5,
	LEVEL3_5 = 6,
	COUNT
};


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

	// For controller navigation
	LevelButtonSelection currentLevelSelection = LevelButtonSelection::LEVEL0;
	bool isUsingController = false;
	float controllerNavigationCooldown = 0.0f;
	const float controllerNavigationDelay = 0.2f;
	const float buttonScaleIncrease = 1.1f; // 10% increase

	// Store transforms and sprites
	std::vector<Entity*> levelButtonEntities;
	std::vector<TransformComponent*> levelButtonTransforms;
	std::vector<Vec2> originalScales;
	Entity* xButtonEntity = nullptr;
	TransformComponent* xButtonTransform = nullptr;
	Vec2 xButtonOriginalScale;
	SoundComponent* SFX = nullptr;

	void UpdateLevelMenuSelection();
	void SelectLevelButton(LevelButtonSelection selection);
	void DeselectAllLevelButtons();
	void ActivateSelectedLevelButton();
};