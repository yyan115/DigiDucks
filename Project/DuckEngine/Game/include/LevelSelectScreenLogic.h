#pragma once
#include "DuckEngine.h"
#include "GameLogicComponent.h"

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

private:
	MainMenu* mainMenu = nullptr;
	
	Entity* levelSelectScreen = nullptr;
	Entity* mainMenuScreen = nullptr;

	SpriteRendererComponent* levelSelectScreenSpriteRenderer = nullptr;
	SpriteRendererComponent* mainMenuScreenSpriteRenderer = nullptr;



};