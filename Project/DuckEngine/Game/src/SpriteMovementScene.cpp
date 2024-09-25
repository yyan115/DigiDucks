#include "DuckEngine.h"
#include "DuckEngine_Sound.h"
#include "DuckEngine_Input.h"
#include "SpriteMovementScene.h"
#include "ImageLoader.h"
#include "Bounding.h"

Entity* player;
Entity* camera;

CameraComponent* cameraComponent;
TransformComponent* playerTransform;
RigidbodyComponent* playerRb;

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

void SpriteMovementScene::Load()
{
	//camera = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

	//cameraComponent = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<CameraComponent>(
	//	camera->EntityID,
	//	0.f, 0.f,        // Camera position (centered at the origin)
	//	1.0f,            // Zoom factor of 1.0 for 1:1 scale
	//	-10.0f,            // Camera height (adjust based on the size of your world)
	//	1.0f,            // Aspect ratio (if the window is square, otherwise adjust)
	//	0                // Layer (default)
	//);

	DuckEngine::SetCameraHeight(20.f);

	// Load necessary assets (textures, sounds, etc.)
	DuckEngine::DUCKENGINE_AssetManager.LoadTexture("../Resources/oldman.png");
	DuckEngine::DUCKENGINE_AssetManager.LoadSound("TestSound", "../Resources/Sounds/magnetic.mp3");
	DuckEngine::DUCKENGINE_AssetManager.LoadSound("TestSound2", "../Resources/Sounds/twitchAlert.wav");


	// setup prefabs
	std::shared_ptr<Prefab> playerPrefab = std::make_shared<Prefab>("Player", "../Resources/oldman.png", Vec2(1.0f, 1.0f));
	playerPrefab->AddComponent(std::make_shared<BoundingCircle>(Vec2(0.0f, 0.0f), 1.f));
	playerPrefab->AddComponent(std::make_shared<RigidbodyComponent>());

	std::shared_ptr<Prefab> obstaclePrefab = std::make_shared<Prefab>("Obstacle", "../Resources/oldman.png", Vec2(2.0f, 2.0f));
	obstaclePrefab->AddComponent(std::make_shared<BoundingBox>(Vec2(5.0f, 0.0f), Vec2{ 1.f, 1.5f }));
	obstaclePrefab->AddComponent(std::make_shared<RigidbodyComponent>());


	std::shared_ptr<Prefab> obstacle2Prefab = std::make_shared<Prefab>("Obstacle2", "../Resources/oldman.png", Vec2(2.0f, 2.0f));
	obstacle2Prefab->AddComponent(std::make_shared<BoundingCircle>(Vec2(-5.0f, 0.0f), 1.f));
	obstacle2Prefab->AddComponent(std::make_shared<RigidbodyComponent>());

	// load prefabs

	PrefabManager::LoadPrefab("Player", playerPrefab);
	PrefabManager::LoadPrefab("Obstacle", obstaclePrefab);
	PrefabManager::LoadPrefab("Obstacle2", obstacle2Prefab);
	
	// instantiate prefabs
	player = PrefabManager::InstantiatePrefab("Player", Vec2(0.0f, 0.0f));
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->EntityID);
	playerRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(player->EntityID);
	circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->EntityID);
	playerRb->velocity = Vec2(0.0f, 0.0f);
	playerRb->isStatic = false;

	obstacle = PrefabManager::InstantiatePrefab("Obstacle", { 5.0f, 0.0f });
	obstacleTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(obstacle->EntityID);
	obstacleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(obstacle->EntityID);
	box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(obstacle->EntityID);
	obstacleRb->isStatic = true;

	obstacle2 = PrefabManager::InstantiatePrefab("Obstacle2", { -3.0f, 0.0f });
	obstacle2Transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(obstacle2->EntityID);
	obstacle2Rb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(obstacle2->EntityID);
	box2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(obstacle2->EntityID);
	obstacle2Rb->isStatic = false;
}

void SpriteMovementScene::Start()
{
	//DuckEngine_Sound::PlaySound("TestSound");

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
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_S))
	{
		playerRb->velocity.y = -moveSpeed; // Move down
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_A))
	{
		playerRb->velocity.x = -moveSpeed; // Move left
	}

	if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_D))
	{
		playerRb->velocity.x = moveSpeed; // Move right
	}

	DuckEngine::DrawCircle(circle->getCenter(), circle->getRadius());
	//DuckEngine::DrawRectangle(circle->getMin(), circle->getMax());

	//DuckEngine::DrawCircle(box->getCenter(), box->getRadius());
	DuckEngine::DrawRectangle(box->getMin(), box->getMax());

	DuckEngine::DrawCircle(box2->getCenter(), box2->getRadius());
	//DuckEngine::DrawRectangle(box2->getMin(), box2->getMax());

	DuckEngine::DrawLine({ 7.f, 5.f }, {-10.f, 10.f}, 0.05f);

	DuckEngine::DrawPoint({ -3.f, -3.f }, 10.f);

	DuckEngine::SetBackgroundColor(255.f, 255.f, 255.f, 255.f);
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