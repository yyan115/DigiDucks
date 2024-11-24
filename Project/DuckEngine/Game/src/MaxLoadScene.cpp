/******************************************************************************/
/*!
\file       MaxLoadScene.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Implements the MaxLoadScene class, handling the logic for
            spawning 2500 square entities with randomized properties,
            and updating their positions each frame to simulate a
            heavy workload for testing purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


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
std::uniform_real_distribution<float> randomPosition(-10.0, 10.0f);  
std::uniform_real_distribution<float> randomScale(0.6f, 1.0f);    
std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);    
std::uniform_real_distribution<float> randomVelocity(-0.01f, 0.01f); 

/// <summary>
/// Loads the scene, including disabling logging for performance and spawning 2500 square objects.
/// </summary>
void MaxLoadScene::Load()
{
    // Disable logging for max load as it makes loading the scene very slow when you spawn 2.5k objects.
    // It doesn't actually impact FPS, just takes longer to load the scene with all the std::couts required for 2.5k objects.
    DuckEngine::EnableLogging(false);

    DuckEngine::SetCameraHeight(20);

    //for (int i = 0; i < 2500; i++)
    //{
    //    SpawnSquare();
    //}
}

/// <summary>
/// Starts the scene, used for initializing elements before the main loop.
/// </summary>
void MaxLoadScene::Start()
{
    Scene::Start();
}

/// <summary>
/// Updates the scene each frame by updating the positions of all spawned squares.
/// </summary>
void MaxLoadScene::Update()
{
    UpdateSquares(1.0F);
}

/// <summary>
/// Post update hook for performing any actions after the main update.
/// </summary>
void MaxLoadScene::PostUpdate()
{

}

/// <summary>
/// Exits the scene, performing any necessary cleanup.
/// </summary>
void MaxLoadScene::Exit()
{

}

/// <summary>
/// Unloads the scene and releases resources.
/// </summary>
void MaxLoadScene::Unload()
{
    Scene::Unload();
}

/// <summary>
/// Spawns a square entity with randomized position, scale, and rotation, and adds it to the scene.
/// </summary>
void MaxLoadScene::SpawnSquare() 
{
    //// Generate random position, scale, rotation, and velocity
    //Vec2 pos = Vec2(randomPosition(gen), randomPosition(gen));
    //float scaleX = randomScale(gen);
    //float scaleY = randomScale(gen);
    //float rotation = randomRotation(gen);

    ////// Add transform component with randomized values
    ////TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(square->entityID);
    ////
    ////transform->position = pos;
    ////transform->angle = rotation;
    ////transform->scale = { scaleX, scaleY };
    //// Create a new square entity
    //Entity* square = DuckEngine::DUCKENGINE_EntityFactory.CreateEntity(Resources::TEXTURE_CRATE.c_str(), pos, {1.0f, 1.0f});

    //// Add transform component with randomized values
    //TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(square->entityID);
    //
    //transform->SetPosition(pos);
    //transform->angle = rotation;
    //transform->scale = { scaleX, scaleY };
}

/// <summary>
/// Updates the positions of all squares in the scene based on random velocities and the deltaTime.
/// </summary>
/// <param name="deltaTime">The time elapsed since the last frame.</param>
void MaxLoadScene::UpdateSquares(float deltaTime) 
{
    for (const auto& [entityId, transformComponent] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TransformComponent>()) {
        // Get the TransformComponent for the entity
        TransformComponent* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
        if (transform) {
            // Update position based on velocity and deltaTime
            float velocityX = randomVelocity(gen);
            float velocityY = randomVelocity(gen);
            transform->GetPosition().x += velocityX * deltaTime;   // velocity.first is velocityX
            transform->GetPosition().y += velocityY * deltaTime;  // velocity.second is velocityY
        }
    }
}