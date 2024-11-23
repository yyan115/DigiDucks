/******************************************************************************/
/*!
\file       GameScene.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 18 2024
\brief      Implementation of the GameScene class, which handles the game logic
			for the main game scene. This includes loading assets, initializing
			game objects, updating game logic, and cleaning up resources.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "GameScene.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "ResourcePath.h"
#include "RoamingLogic.h"
#include "MessagingSystem.h"
#include <random>
#include <chrono>
#include <map>


Entity* duck;
TransformComponent* duckTrfm;
RigidbodyComponent* duckRb;
AnimatorComponent* duckAnimator;
SoundComponent* duckSound;
BoundingCircle* duckCircleCollider;
BoundingBox* duckBoxCollider;
bool holdingObject = false;

// Object infront of player
Entity* frontObject;

////Test Messaging System
//InputEventManager inputEventManager;
//Player message;

void playerInteraction();

/// <summary>
/// Loads all necessary resources for the scene.
/// This function is called before the scene starts.
/// </summary>
void GameScene::Load() 
{
    DuckEngine::EnableLogging(false);

    DuckEngine::SetCameraHeight(20);

    DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CRATE.c_str());

	// instantiate prefabs
	duck = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
	duckTrfm = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(duck->entityID);
	duckRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(duck->entityID);
	duckAnimator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(duck->entityID);
	duckCircleCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(duck->entityID);
	duckBoxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(duck->entityID);
	//duckCircleCollider->SetCollisionCallback([](int otherEntityID)
	//	{
	//		std::cout << "Player collided with Entity ID: " << otherEntityID << std::endl;
	//	});
	duckBoxCollider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(duck->entityID);
	duckBoxCollider->SetCollisionCallback([](int otherEntityID)
		{
			frontObject = DuckEngine::DUCKENGINE_EntityManager.GetEntity(otherEntityID);
			if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_R))
			{
				playerInteraction();
				holdingObject = !holdingObject;
			}
		});
	
	duckSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(duck->entityID);


	//inputEventManager.addListener(&message);
}

/// <summary>
/// Starts the scene, initializing game objects and setting up the initial state.
/// This function is called when the scene begins running.
/// </summary>
void GameScene::Start() 
{
	DuckEngine::showDebugDraw = false;
}

/// <summary>
/// Updates the scene logic each frame, including processing input and updating game objects.
/// This function is called every frame.
/// </summary>
void GameScene::Update() 
{	
	float moveSpeed = 10.0f;

	// Reset the player's velocity at the start of each fixed update
	duckRb->velocity = Vec2(0.f, 0.f);

	// Store input state - don't directly modify velocity
	Vector2D inputDirection(0.0f, 0.0f);

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_W))
	{
		inputDirection.y += 1.0f;
		duckBoxCollider->setOffSet(0.f, 1.f);
		//playerAnimator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		inputDirection.y -= 1.0f;
		duckBoxCollider->setOffSet(0.f, -1.f);
		//playerAnimator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		inputDirection.x -= 1.0f;
		duckBoxCollider->setOffSet(-1.f, 0.f);
		//playerAnimator->PlayAnimation("WalkAnimation");
	}
	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		inputDirection.x += 1.0f;
		duckBoxCollider->setOffSet(1.f, 0.f);
		//playerAnimator->PlayAnimation("WalkAnimation");
	}

	// Normalize the input direction if it's not zero
	if (inputDirection.x != 0.0f || inputDirection.y != 0.0f)
	{
		float length = std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
		inputDirection.x /= length;
		inputDirection.y /= length;
	}

	// Set velocity based on normalized input
	duckRb->velocity = inputDirection * moveSpeed;

	//inputEventManager.notifyScrollEvent(static_cast<int>(DuckEngine_Input::GetScrollOffsetY()));

	if (duckRb->velocity.x == 0.0f && duckRb->velocity.y == 0.0f)
	{
		//playerAnimator->PlayAnimation("IdleAnimation");
	}



	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	DuckEngine::SetCameraPosition(duckTrfm->position.x, duckTrfm->position.y);

	// For each sound component, play the sound if it is set to play on start
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

}

/// <summary>
/// Performs any operations after the main update logic, such as cleanup or post-processing.
/// This function is called every frame, after the Update() method.
/// </summary>
void GameScene::PostUpdate() 
{
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT))
	{
		std::cout << "Left mouse button pressed!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_U)) {
		if (duckSound) {
			std::cout << "Sound stopped\n";
			duckSound->Stop();
		}
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_K))
	{
		std::cout << "K is pressed!\n";
		DuckEngine::showDebugDraw = !DuckEngine::showDebugDraw;
	}
}

/// <summary>
/// Exits the scene, performing any cleanup necessary before the scene is unloaded.
/// </summary>
void GameScene::Exit() 
{

}

/// <summary>
/// Unloads the scene and frees any resources that were loaded during the Load() phase.
/// </summary>
void GameScene::Unload() 
{
	// base unload
	Scene::Unload();

}

void playerInteraction()
{
	if (holdingObject) {
		if (frontObject->name == "Lettuce_Box") {
			std::cout << "Player interacted with Lettuce_Box\n";
		}
	}

}