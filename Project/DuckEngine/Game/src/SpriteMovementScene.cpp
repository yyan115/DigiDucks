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

void SpriteMovementScene::Load()
{
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
	LevelManager::LoadLevel("../Resources/SpriteRendererScene.json");

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
	TransformComponent* testUITransfrom = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(testUI->entityID);
	testUITransfrom->relativeToCamera = false;

	// test draw no texture
	//noTextureEntity = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity({ 0.0f, 0.0f }, { 2000.0f, 2000.0f });
	//DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(noTextureEntity->entityID, true);
}

void SpriteMovementScene::Start()
{
	//DuckEngine_Sound::PlaySounds("TestSound");
}

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

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_Y))
	{
		std::cout << "Y is pressed!\n";
	}

	if (DuckEngine_Input::IsKeyReleased(DuckEngine_Input::KEY_Y))
	{
		std::cout << "Y is released!\n";
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_P))
	{
		DuckEngine::ToggleEditor = !DuckEngine::ToggleEditor;
	}

	if (playerRb->velocity.x == 0.0f && playerRb->velocity.y == 0.0f)
	{
		playerAnimator->PlayAnimation("IdleAnimation");
	}


	DuckEngine::DrawCircle(circle->getCenter(), circle->getRadius());
	////DuckEngine::DrawRectangle(circle->getMin(), circle->getMax());

	////DuckEngine::DrawCircle(box->getCenter(), box->getRadius());
	DuckEngine::DrawRectangle(box->getBtmL(), box->getTopR());

	DuckEngine::DrawCircle(box2->getCenter(), box2->getRadius());
	////DuckEngine::DrawRectangle(box2->getBtmL(), box2->getTopR());

	DuckEngine::DrawLine({ 7.f, 5.f }, {-10.f, 10.f}, 0.05f);

	DuckEngine::DrawPoint({ -3.f, -3.f }, 10.f);

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);


	//// TEST DRAWING TO UI (LOOKS LIKE IT WORKS)
	//DuckEngine::DrawRectangle({ 0.f, 0.f }, { 460.f, 460.f }, 0.f, { 255.f, 0.f, 0.f, 255.f }, false);

	//// SET CAMERA TO MOVE ALONG TO PLAYER
	DuckEngine::SetCameraPosition(-playerTransform->position.x, -playerTransform->position.y);

	////std::cout << "player pos: " << playerTransform->position.x << ", " << playerTransform->position.y << "\n";

	DuckEngine::RenderText("TEST SCENE", { 20.f , DuckEngine::GetWindowHeight() - 70.f }, 1.f, { 255.f, 50.f, 100.f, 250.f });

	DuckEngine::RenderText("TEST TEXT", { DuckEngine::GetWindowWidth() - 300.f  , 250.f }, 1.f, { 0.f, 255.f, 150.f, 250.0f });
}

void SpriteMovementScene::PostUpdate()
{

}

void SpriteMovementScene::Exit()
{
}

void SpriteMovementScene::Unload()
{
	// base unload
	Scene::Unload();

}