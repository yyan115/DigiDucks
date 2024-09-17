#include "DuckEngine.h"
#include "SpriteMovementScene.h"
#include "ImageLoader.h"

Entity* player;
Entity* camera;

CameraComponent* cameraComponent;
TransformComponent* playerTransform;

void SpriteMovementScene::Load()
{
	camera = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

	cameraComponent = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<CameraComponent>(
		camera->EntityID,
		0.f, 0.f,        // Camera position (centered at the origin)
		1.0f,            // Zoom factor of 1.0 for 1:1 scale
		-10.0f,            // Camera height (adjust based on the size of your world)
		1.0f,            // Aspect ratio (if the window is square, otherwise adjust)
		0                // Layer (default)
	);

	player = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
	SpriteRendererComponent* playerSprite = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(player->EntityID, true);
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(player->EntityID);

	playerSprite->texture = ImageLoader::LoadTexture("../Resources/oldman.png");


}

void SpriteMovementScene::Start()
{

}

void SpriteMovementScene::Update()
{
	float moveSpeed = 5.0f;
	if (DuckEngine::WKeyDown)
	{
		playerTransform->position.y += moveSpeed * DuckEngine::DeltaTime();
	}

	if (DuckEngine::SKeyDown)
	{
		playerTransform->position.y -= moveSpeed * DuckEngine::DeltaTime();
	}

	if (DuckEngine::AKeyDown)
	{
		playerTransform->position.x -= moveSpeed * DuckEngine::DeltaTime();
	}

	if (DuckEngine::DKeyDown)
	{
		playerTransform->position.x += moveSpeed * DuckEngine::DeltaTime();
	}

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

}