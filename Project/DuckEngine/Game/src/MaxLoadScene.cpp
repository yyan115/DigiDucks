#include "MaxLoadScene.h"
#include "DuckEngine.h"
#include <random>
#include <chrono>
#include <map>
#include "ImageLoader.h"
#include "ResourcePath.h"

// Global variables for timing
std::chrono::time_point<std::chrono::steady_clock> lastSpawnTime = std::chrono::steady_clock::now();
float spawnInterval = 0.3f; // 1 second

// Random number generator for position, scale, rotation, and velocity
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> randomPosition(-10.0, 10.0f);  
std::uniform_real_distribution<float> randomScale(0.6f, 1.0f);    
std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);    
std::uniform_real_distribution<float> randomVelocity(-0.01f, 0.01f); 

void MaxLoadScene::Load()
{
    DuckEngine::SetCameraHeight(20);

    DuckEngine::DUCKENGINE_AssetManager.LoadTexture(Resources::TEXTURE_CRATE.c_str());


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

void MaxLoadScene::SpawnSquare() 
{
    // Generate random position, scale, rotation, and velocity
    Vec2 pos = Vec2(randomPosition(gen), randomPosition(gen));
    float scaleX = randomScale(gen);
    float scaleY = randomScale(gen);
    float rotation = randomRotation(gen);

    // Create a new square entity
    Entity* square = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity(Resources::TEXTURE_CRATE.c_str(), pos, {1.0f, 1.0f});

    // Add transform component with randomized values
    TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(square->entityID);
    
    transform->position = pos;
    transform->angle = rotation;
    transform->scale = { scaleX, scaleY };
}

void MaxLoadScene::UpdateSquares(float deltaTime) 
{
    for (const auto& [entityId, transformComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TransformComponent>()) {
        // Get the TransformComponent for the entity
        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
        if (transform) {
            // Update position based on velocity and deltaTime
            float velocityX = randomVelocity(gen);
            float velocityY = randomVelocity(gen);
            transform->position.x += velocityX * deltaTime;   // velocity.first is velocityX
            transform->position.y += velocityY * deltaTime;  // velocity.second is velocityY
        }
    }
}