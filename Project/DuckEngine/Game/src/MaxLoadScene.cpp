#include "MaxLoadScene.h"
#include "DuckEngine.h"
#include <random>
#include <chrono>
#include <map>
#include "ImageLoader.h"

// Global variables for timing
std::chrono::time_point<std::chrono::steady_clock> lastSpawnTime = std::chrono::steady_clock::now();
float spawnInterval = 0.3f; // 1 second

// Random number generator for position, scale, rotation, and velocity
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> randomPosition(-500.0f, 500.0f);  // Position between -500 and 500
std::uniform_real_distribution<float> randomScale(10.0f, 100.0f);      // Scale between 50 and 500
std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);    // Rotation between 0 and 360 degrees
std::uniform_real_distribution<float> randomVelocity(-1.0f, 1.0f);  // Velocity between -100 and 100

// Map to store entity velocities (entityID -> (velocityX, velocityY))
std::map<int, std::pair<float, float>> entityVelocities;


void MaxLoadScene::Load()
{
    //Entity* camera = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

    //CameraComponent* cameraComponent = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<CameraComponent>(
    //    camera->EntityID,
    //    0.f, 0.f,        // Camera position (centered at the origin)
    //    1000.f,            // Zoom factor of 1.0 for 1:1 scale
    //    1000.0f,            // Camera height (adjust based on the size of your world)
    //    1.0f,            // Aspect ratio (if the window is square, otherwise adjust)
    //    0                // Layer (default)
    //);

    DuckEngine::SetCameraHeight(1000.f);

    for (int i = 0; i < 2500; i++)
    {
        SpawnSquare();
    }
}

void MaxLoadScene::Start()
{
}

void MaxLoadScene::Update()
{
    //CheckAndSpawnSquare(1.0f); // spawn square every 1 second
    UpdateSquares(1.0F);
}

void MaxLoadScene::PostUpdate()
{

}

void MaxLoadScene::Exit()
{

}

void MaxLoadScene::Unload()
{
    Scene::Unload();
}

void MaxLoadScene::SpawnSquare() {
    // Generate random position, scale, rotation, and velocity
    Vec2 pos = Vec2(randomPosition(gen), randomPosition(gen));
    float scaleX = randomScale(gen);
    float scaleY = randomScale(gen);
    float rotation = randomRotation(gen);
    float velocityX = randomVelocity(gen);
    float velocityY = randomVelocity(gen);

    // Create a new square entity
    Entity& square = DuckEngine::DUCKENGINE_EntityManager.CreateEntity();

    // Add transform component with randomized values
    DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square.EntityID);

    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(square.EntityID);

    transform->position = pos;
    transform->angle = rotation;
    transform->scale = { scaleX, scaleY };

    // Store the velocity in the map
    entityVelocities[square.EntityID] = std::make_pair(velocityX, velocityY);

    // Add sprite renderer component
    SpriteRendererComponent* sr = DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square.EntityID, true, 0, true, Color(0, 255.f, 255.f, 255.f));
    //sr->texture = ImageLoader::LoadTexture("../Resources/oldman.png");


    // Print for debugging
    //std::cout << "Spawned square at position (" << pos.x << ", " << pos.y << "), scale (" << scaleX << ", " << scaleY
    //    << "), rotation " << rotation << ", velocity (" << velocityX << ", " << velocityY << ")" << std::endl;
}

void MaxLoadScene::CheckAndSpawnSquare(float spawnInterval) {
    // Get the current time
    auto currentTime = std::chrono::steady_clock::now();

    // Calculate the time difference since the last spawn
    std::chrono::duration<float> elapsedTime = currentTime - lastSpawnTime;

    // Check if it's time to spawn a new square
    if (elapsedTime.count() >= spawnInterval) {
        // Spawn a new square
        SpawnSquare();

        // Reset the timer
        lastSpawnTime = currentTime;
    }
}

void MaxLoadScene::UpdateSquares(float deltaTime) {
    for (const auto& [entityId, velocity] : entityVelocities) {
        // Get the TransformComponent for the entity
        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
        if (transform) {
            // Update position based on velocity and deltaTime
            transform->position.x += velocity.first * deltaTime;   // velocity.first is velocityX
            transform->position.y += velocity.second * deltaTime;  // velocity.second is velocityY

            // Debug print to check the updated position
            //std::cout << "Entity " << entityId << " moved to ("
            //    << transform->position.x << ", " << transform->position.y << ")" << std::endl;
        }
    }
}