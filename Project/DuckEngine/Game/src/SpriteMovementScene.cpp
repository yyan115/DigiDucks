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
#include "DuckEngine_Sound.h"
#include "DuckEngine_Input.h"
#include "SpriteMovementScene.h"
#include "ImageLoader.h"
#include "ResourcePath.h"

Entity* player;
Entity* camera;

TransformComponent* playerTransform;
RigidbodyComponent* playerRb;
AnimatorComponent* playerAnimator;
SoundComponent* playerSound;

BoundingCircle* circle;

// Testing Obstacle
Entity* obstacle;
TransformComponent* obstacleTransform;
RigidbodyComponent* obstacleRb;
BoundingBox* box;

// 2nd Obstacle
Entity* obstacle2;
TransformComponent* obstacle2Transform;
RigidbodyComponent* obstacle2Rb;
BoundingCircle* box2;

// test UI
Entity* testUI;

// no texture sprite
Entity* noTextureEntity;

/************************************************************************
@brief Loads the necessary assets and entities for the SpriteMovementScene.
	   Initializes the player, obstacles, UI elements, and their components.
*************************************************************************/
void SpriteMovementScene::Load()
{
	DuckEngine::EnableLogging(true);

	DuckEngine::SetCameraHeight(20);

	// Load necessary assets (textures, sounds, etc.)
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_OLDMAN.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_BACKGROUND.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CRATE.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CHARACTERIDLE.c_str(), 19, 24);
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CHARACTERWALK.c_str(), 19, 24);
	DuckEngine::DUCKENGINE_AssetManager.LoadSound("TestSound", Resources::SOUND_THEME.c_str());
	DuckEngine::DUCKENGINE_AssetManager.LoadSound("TestSound2", Resources::SOUND_DUCKSOUND.c_str());

	PrefabManager::LoadPrefabsFromFile("../Resources/Prefab.json");
	LevelManager::LoadLevel("../Resources/Scenes/SpriteMovementScene.json");

	// instantiate prefabs
	player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
	playerRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(player->entityID);
	playerAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(player->entityID);
	circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);
	circle->SetCollisionCallback([](Entity* otherEntity) 
	{
		UNREFERENCED_PARAMETER(otherEntity);
		std::cout << "Player collided with another entity!" << std::endl;
	});
	std::shared_ptr<SoundComponent> playerSsound = std::make_shared<SoundComponent>("TestSound", false, false, 1.0f);
	DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SoundComponent>(player->entityID, *playerSsound);
	playerSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(player->entityID);

	obstacle = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Obstacle1");
	obstacleTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(obstacle->entityID);
	obstacleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(obstacle->entityID);
	box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(obstacle->entityID);

	obstacle2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Obstacle2");
	obstacle2Transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(obstacle2->entityID);
	obstacle2Rb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(obstacle2->entityID);
	box2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(obstacle2->entityID);


	// test UI
	testUI = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("", {400.0f, 400.0f}, {100.0f, 100.0f});
	auto testUITransfrom = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(testUI->entityID);
	testUITransfrom->relativeToCamera = false;

	auto testUISprite = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(testUI->entityID);
	testUISprite->layer = 2;
}

/************************************************************************
@brief Starts the SpriteMovementScene, initializing sounds or other
	   elements before the main update loop.
*************************************************************************/
void SpriteMovementScene::Start()
{
	//DuckEngine_Sound::PlaySounds("TestSound");
}

/************************************************************************
@brief Updates the logic of the SpriteMovementScene, including player input,
	   sprite animations, and rendering of objects.
*************************************************************************/
void SpriteMovementScene::Update()
{
	float moveSpeed = 10.0f;

	// Reset the player's velocity at the start of each frame
	playerRb->velocity = Vec2(0.f, 0.f);

	// Handle movement based on key input by setting velocity
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		playerRb->velocity.y = moveSpeed; // Move up
		playerAnimator->PlayAnimation("WalkAnimation");
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		playerRb->velocity.y = -moveSpeed; // Move down
		playerAnimator->PlayAnimation("WalkAnimation");
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		playerRb->velocity.x = -moveSpeed; // Move left
		playerAnimator->PlayAnimation("WalkAnimation");
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		playerRb->velocity.x = moveSpeed; // Move right
		playerAnimator->PlayAnimation("WalkAnimation");
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_Y)) {
		if (playerSound) {
			std::cout << "Sound play\n";
			playerSound->Play();
		}
	}

	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::GetScrollOffsetY() == DuckEngine_Input::SCROLL_DOWN)
	{
		std::cout << "Mouse scrolled down!\n";
	}

	if (DuckEngine_Input::GetScrollOffsetY() == DuckEngine_Input::SCROLL_UP)
	{
		std::cout << "Mouse scrolled up!\n";
	}

	if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_Y))
	{
		std::cout << "Y is released!\n";
	}

	if (playerRb->velocity.x == 0.0f && playerRb->velocity.y == 0.0f)
	{
		playerAnimator->PlayAnimation("IdleAnimation");
	}


	DuckEngine::DrawCircle(circle->getCenter(), circle->getRadius());

	DuckEngine::DrawRectangle(box->getBtmL(), box->getTopR(), box->rotation);

	DuckEngine::DrawCircle(box2->getCenter(), box2->getRadius());

	DuckEngine::DrawLine({ 7.f, 5.f }, {-10.f, 10.f}, 0.05f);

	DuckEngine::DrawPoint({ -3.f, -3.f }, 10.f);

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	DuckEngine::SetCameraPosition(-playerTransform->position.x, -playerTransform->position.y);

	DuckEngine::RenderText("TEST SCENE", { 20.f , DuckEngine::GetWindowHeight() - 200.f }, 1.f, { 255.f, 50.f, 100.f, 250.f });

	DuckEngine::RenderText("TEST TEXT", { DuckEngine::GetWindowWidth() - 300.f  , 250.f }, 1.f, { 0.f, 255.f, 150.f, 250.0f });
}

/************************************************************************
@brief Handles post-update logic after the main update loop.
*************************************************************************/
void SpriteMovementScene::PostUpdate()
{

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