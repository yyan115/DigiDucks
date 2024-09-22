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


	DuckEngine::DUCKENGINE_AssetManager.LoadTexture("../Resources/oldman.png");
	DuckEngine::DUCKENGINE_AssetManager.LoadSound("TestSound", "../Resources/Sounds/magnetic.mp3");
	DuckEngine::DUCKENGINE_AssetManager.LoadSound("TestSound2", "../Resources/Sounds/twitchAlert.wav");


	obstacle = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("../Resources/oldman.png", { 5.0f, 0.0f }, { 2.0f, 2.0f });
	obstacleTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(obstacle->EntityID);
	obstacleRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(obstacle->EntityID);
	obstacleRb->isStatic = false;
	box = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(obstacle->EntityID, obstacleTransform->position, Vec2{1.f,2.f});
	

	player = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity("../Resources/oldman.png", { 0.0f, 0.0f }, { 1.0f, 1.0f });
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->EntityID);
	playerRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(player->EntityID);
	circle = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingCircle>(player->EntityID, playerTransform->position, 1.f);

	
}

void SpriteMovementScene::Start()
{
	DuckEngine_Sound::PlaySound("TestSound");

}

void SpriteMovementScene::Update()
{
	float moveSpeed = 10.0f;

	// Reset the player's velocity at the start of each frame
	playerRb->velocity = Vec2(0.f, 0.f);

	// Handle movement based on key input by setting velocity
	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_W))
	{
		playerRb->velocity.y = moveSpeed; // Move up
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_S))
	{
		playerRb->velocity.y = -moveSpeed; // Move down
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_A))
	{
		playerRb->velocity.x = -moveSpeed; // Move left
	}

	if (DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_D))
	{
		playerRb->velocity.x = moveSpeed; // Move right
	}

	DuckEngine::DrawCircle(circle->getCenter(), circle->getRadius()*2);
	DuckEngine::DrawRectangle(box->getMin(), box->getMax());
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