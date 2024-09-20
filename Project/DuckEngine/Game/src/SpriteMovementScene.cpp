#include "DuckEngine.h"
#include "SpriteMovementScene.h"
#include "ImageLoader.h"
#include "Bounding.h"

Entity* player;
Entity* camera;

CameraComponent* cameraComponent;
TransformComponent* playerTransform;

BoundingBox* box;
BoundingCircle* circle;

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

	player = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
	SpriteRendererComponent* playerSprite = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(player->EntityID, true);
	playerTransform = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(player->EntityID);

	playerSprite->texture = ImageLoader::LoadTexture("../Resources/oldman.png");


	circle = new BoundingCircle(playerTransform->position, 1.f);

	box = new BoundingBox(playerTransform->position.x, playerTransform->position.y, 0.5f, 0.5f);
		
}

void SpriteMovementScene::Start()
{

}

void SpriteMovementScene::Update()
{
	float moveSpeed = 5.0f;
	// Handle movement based on key input
	Vec2 newPos = playerTransform->position;

	if (DuckEngine::IsKeyPressed(DuckEngine::KEY_W)) {
		newPos.y += moveSpeed * DuckEngine::DeltaTime();
		playerTransform->position.y += moveSpeed * DuckEngine::DeltaTime();
		circle->setCenterPos(newPos);
	}

	if (DuckEngine::IsKeyPressed(DuckEngine::KEY_S)) {
		newPos.y -= moveSpeed * DuckEngine::DeltaTime();
		playerTransform->position.y -= moveSpeed * DuckEngine::DeltaTime();
		circle->setCenterPos(newPos);
	}

	if (DuckEngine::IsKeyPressed(DuckEngine::KEY_A)) {
		newPos.x -= moveSpeed * DuckEngine::DeltaTime();
		playerTransform->position.x -= moveSpeed * DuckEngine::DeltaTime();
		circle->setCenterPos(newPos);
	}

	if (DuckEngine::IsKeyPressed(DuckEngine::KEY_D)) {
		newPos.x += moveSpeed * DuckEngine::DeltaTime();
		playerTransform->position.x += moveSpeed * DuckEngine::DeltaTime();
		circle->setCenterPos(newPos);
	}

	if (checkCollision(*circle, newPos, *box, DuckEngine::DeltaTime())) {
		std::cout << "Collision detected" << std::endl;
	}
	else {
		std::cout << "No collision detected" << std::endl;
	}

	DuckEngine::DrawCircle(circle->getCenterPos(), circle->getRadius());
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

}