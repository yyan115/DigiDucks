#include "CameraSystem.h"

void CameraSystem::Start()
{

}

void CameraSystem::Update()
{
	for (const auto& [entityId, camera] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<CameraComponent>())
	{
		CameraComponent* camera = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<CameraComponent>(entityId);
		//TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
		//if (spriteRenderer->sprite && transform)
		//{
		//	std::cout << "SpriteRenderer: " << transform->x << " " << transform->y << " \n";
		//}

		if (camera) {
			// Example of updating camera code
			// 
			// 
			//if (Input::IsKeyPressed(KEY_W)) {
			//	cameraEntity->position.y += 1.0f;  // Move camera up
			//}

			//// Update the camera height, zoom, etc.
			//cameraEntity->cameraHeight = /* some logic for camera height */;
			//cameraEntity->zoom = /* some logic for zoom */;

			//// Update the camera aspect ratio (use framebuffer size)
			//int screenWidth, screenHeight;
			//glfwGetFramebufferSize(pWindow, &screenWidth, &screenHeight);
			//cameraEntity->windowAspectRatio = static_cast<float>(screenWidth) / screenHeight;
		}
	}
}