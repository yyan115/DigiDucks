/******************************************************************************/
/*!
\file       SpriteMovementScene.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the SpriteMovementScene class, which handles the logic
			for a scene focused on sprite movement. This includes loading
			assets, setting up entities, and managing input to control player
			movement and animations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "SpriteMovementScene.h"
#include "ImageLoader.h"
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <set>
#include "GameManager.h"
#include "MovementLogic.h"

Entity* player;
Entity* camera;

TransformComponent* playerTransform;
RigidbodyComponent* playerRb;
AnimatorComponent* playerAnimator;
SoundComponent* playerSound;

BoundingCircle* circle;

// Test text
Entity* textBox;
TextComponent* textComponent;

// Test button
Entity* buttonBox;
//ButtonComponent buttonComponent;

// test UI
Entity* testUI;

// no texture sprite
Entity* noTextureEntity;

// Test Roaming Logic
Vec2 positon1 = { 10.f, 10.f };
Vec2 position2 = { -10.f, 10.f };

//Test Messaging System
InputEventManager inputEventManager;
Player message;

struct MyStruct {
	int x;
	int y;
};

bool operator<(const MyStruct& lhs, const MyStruct& rhs) {
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}


/************************************************************************
@brief Loads the necessary assets and entities for the SpriteMovementScene.
	   Initializes the player, obstacles, UI elements, and their components.
*************************************************************************/
void SpriteMovementScene::Load()
{
	DuckEngine::EnableLogging(false);

	DuckEngine::SetCameraHeight(20);

	// instantiate prefabs
	player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
	playerRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(player->entityID);
	playerAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(player->entityID);
	circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);
	circle->SetCollisionCallback([](int otherEntityID)
		{
			std::cout << "Player collided with Entity ID: " << otherEntityID << std::endl;

		});
	playerSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(player->entityID);




	//background entity
	//Entity* background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Background");

	// test UI
	//testUI = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", {400.0f, 400.0f}, {100.0f, 100.0f});
	//auto testUITransfrom = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(testUI->entityID);
	//testUITransfrom->relativeToCamera = false;

	//auto testUISprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(testUI->entityID);
	//testUISprite->layer = 2;

	//textBox = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", { 400.0f, 400.0f }, { 100.0f, 100.0f });
	//auto textComponent = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Obstacle1");
	//textComponent->text = "TEST TEXT COMPONENT";
	//textComponent->position = { 0.f, 0.f };
	//textComponent->fontSize = 1;
	//textComponent->color = { 255.f, 50.f, 100.f, 250.f };

	//buttonBox = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", { 400.0f, 400.0f }, { 100.0f, 100.0f });
	//auto buttonComponent = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<ButtonComponent>(buttonBox->entityID);
	//buttonComponent->minPos = {0.f, 0.f};
	//buttonComponent->maxPos = { 200.f, 100.f };
	//buttonComponent->onClick = []() { std::cout << "Button clicked!"; };

	//auto buttonSprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(buttonBox->entityID);
	//buttonSprite->color = { 0.0f, 255.0f, 255.0f, 255.0f };
	//buttonSprite->useColor = true;
	//buttonSprite->sprite = true;

	Entity* buttonEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("buttonBox");
	auto buttonTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(buttonEntity->entityID);
	auto buttonComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(buttonEntity->entityID);
	// Calculate width and height from minPos and maxPos
	float buttonWidth = buttonComponent->maxPos.x - buttonComponent->minPos.x;
	float buttonHeight = buttonComponent->maxPos.y - buttonComponent->minPos.y;

	buttonComponent->onClick = []() { std::cout << "Button clicked!\n"; };
	// Convert minPos and maxPos from top-left to OpenGL centered coordinates
	Vector2D centeredMinPos = {
		buttonComponent->minPos.x - DuckEngine::GetWindowWidth() / 2.0f,
		DuckEngine::GetWindowHeight() / 2.0f - buttonComponent->minPos.y
	};

	Vector2D centeredMaxPos = {
		buttonComponent->maxPos.x - DuckEngine::GetWindowWidth() / 2.0f,
		DuckEngine::GetWindowHeight() / 2.0f - buttonComponent->maxPos.y
	};

	// Set Transform scale
	buttonTransform->scale = { buttonWidth, buttonHeight };

	// Set Transform position as the center of the converted min and max positions
	buttonTransform->SetPosition((centeredMinPos + centeredMaxPos) / 2.0f);

	buttonTransform->relativeToCamera = false;

	//buttonTransform->position = buttonCenter;

	//// Given minPos and maxPos as the bounding coordinates
	//Vector2D minPos = buttonComponent->minPos;
	//Vector2D maxPos = buttonComponent->maxPos;

	//// Calculate the width (scale in x) and height (scale in y)
	//float width = maxPos.x - minPos.x;  // Scale along the x-axis
	//float height = maxPos.y - minPos.y;  // Scale along the y-axis

	//// Set the scale based on the calculated width and height
	//buttonTransform->scale = { width, height };


	//float buttonWidth = buttonComponent->maxPos.x - buttonComponent->minPos.x;
	//float buttonHeight = buttonComponent->maxPos.y - buttonComponent->minPos.y;
	//buttonTransform->scale = { buttonWidth, buttonHeight };
	//buttonTransform->position = {
	//(buttonComponent->minPos.x + buttonComponent->maxPos.x) / 2,
	//(buttonComponent->minPos.y + buttonComponent->maxPos.y) / 2
	//};
	//buttonTransform->relativeToCamera = false;

	inputEventManager.addListener(&message);
}

/************************************************************************
@brief Starts the SpriteMovementScene, initializing sounds or other
	   elements before the main update loop.
*************************************************************************/
void SpriteMovementScene::Start()
{
	Scene::Start();
	//DuckEngine_Sound::PlaySounds("TestSound");
	DuckEngine::showDebugDraw = false;
}

/************************************************************************
@brief Updates the logic of the SpriteMovementScene, including player input,
	   sprite animations, and rendering of objects.
*************************************************************************/
void SpriteMovementScene::Update()
{
	inputEventManager.notifyScrollEvent(static_cast<int>(DuckEngine_Input::GetScrollOffsetY()));

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	DuckEngine::SetCameraPosition(playerTransform->GetPosition().x, playerTransform->GetPosition().y);

	//DuckEngine::RenderText("TEST SCENE", { 20.f , DuckEngine::GetWindowHeight() - 200.f }, 1.f, { 255.f, 50.f, 100.f, 250.f });

	//DuckEngine::RenderText("TEST TEXT", { DuckEngine::GetWindowWidth() - 300.f  , 250.f }, 1.f, { 0.f, 255.f, 150.f, 250.0f });

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	// Test Roaming Logic
	// Between 2 Pos
	RoamSelectedPrefab("DuckPrefab", positon1, position2);
	// A direction for a time.
	//RoamSelectedPrefab("DuckPrefab", positon1, 3.f);

}

/************************************************************************
@brief Handles post-update logic after the main update loop.
*************************************************************************/
void SpriteMovementScene::PostUpdate()
{
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		auto movementLogic = GameLogicManager::GetLogicForEntity<MovementLogic>(player->entityID);
		movementLogic->TestTest();
		std::cout << "Left mouse button pressed!\n";
	}

	//if (DuckEngine_Input::GetScrollOffsetY() == DuckEngine_Input::SCROLL_DOWN)
	//{
	//	std::cout << "Mouse scrolled down!\n";
	//}

	//if (DuckEngine_Input::GetScrollOffsetY() == DuckEngine_Input::SCROLL_UP)
	//{
	//	std::cout << "Mouse scrolled up!\n";
	//}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (playerSound) {
			std::cout << "Sound stopped\n";
			playerSound->Stop();
		}
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_K))
	{
		std::cout << "K is pressed!\n";
		DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
	}
}

/************************************************************************
@brief Exits the SpriteMovementScene, handling any necessary cleanup.
*************************************************************************/
void SpriteMovementScene::Exit()
{
}

/************************************************************************
@brief Unloads the SpriteMovementScene, freeing any resources used.
*************************************************************************/
void SpriteMovementScene::Unload()
{
	// base unload
	Scene::Unload();

}