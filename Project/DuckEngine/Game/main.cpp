#include "DuckEngine.h"

static DuckEngine engine;

#include <random>
#include <chrono>
#include <map>

/// <summary>
/// TEST CODE
/// </summary>
namespace  {

    // Global variables for timing
    std::chrono::time_point<std::chrono::steady_clock> lastSpawnTime = std::chrono::steady_clock::now();
    float spawnInterval = 0.3f; // 1 second

    // Random number generator for position, scale, rotation, and velocity
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> randomPosition(-500.0f, 500.0f);  // Position between -500 and 500
    std::uniform_real_distribution<float> randomScale(50.0f, 500.0f);      // Scale between 50 and 500
    std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);    // Rotation between 0 and 360 degrees
    std::uniform_real_distribution<float> randomVelocity(-1.0f, 1.0f);  // Velocity between -100 and 100

    // Map to store entity velocities (entityID -> (velocityX, velocityY))
    std::map<int, std::pair<float, float>> entityVelocities;

    void SpawnSquare(DuckEngine& engine) {
        // Generate random position, scale, rotation, and velocity
        Vec2 pos = Vec2(randomPosition(gen), randomPosition(gen));
        float scaleX = randomScale(gen);
        float scaleY = randomScale(gen);
        float rotation = randomRotation(gen);
        float velocityX = randomVelocity(gen);
        float velocityY = randomVelocity(gen);

        // Create a new square entity
        Entity& square = engine.DUCKENGINE_EntityManager.CreateEntity();

        // Add transform component with randomized values
        engine.DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square.EntityID, scaleX, scaleY, rotation, pos.x, pos.y);

        // Store the velocity in the map
        entityVelocities[square.EntityID] = std::make_pair(velocityX, velocityY);

        // Add sprite renderer component
        engine.DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square.EntityID, true);

        //// Print for debugging
        //std::cout << "Spawned square at position (" << pos.x << ", " << pos.y << "), scale (" << scaleX << ", " << scaleY
        //    << "), rotation " << rotation << ", velocity (" << velocityX << ", " << velocityY << ")" << std::endl;
    }

    void CheckAndSpawnSquare(DuckEngine& engine, float spawnInterval) {
        // Get the current time
        auto currentTime = std::chrono::steady_clock::now();

        // Calculate the time difference since the last spawn
        std::chrono::duration<float> elapsedTime = currentTime - lastSpawnTime;

        // Check if it's time to spawn a new square
        if (elapsedTime.count() >= spawnInterval) {
            // Spawn a new square
            SpawnSquare(engine);

            // Reset the timer
            lastSpawnTime = currentTime;
        }
    }

    void UpdateSquares(DuckEngine& engine, float deltaTime) {
        for (const auto& [entityId, velocity] : entityVelocities) {
            // Get the TransformComponent for the entity
            TransformComponent* transform = engine.DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityId);
            if (transform) {
                // Update position based on velocity and deltaTime
                //transform->position.x += velocity.first * deltaTime;   // velocity.first is velocityX
                //transform->position.y += velocity.second * deltaTime;  // velocity.second is velocityY

                // Debug print to check the updated position
                //std::cout << "Entity " << entityId << " moved to ("
                //    << transform->position.x << ", " << transform->position.y << ")" << std::endl;
            }
        }
    }
}

int main(void)
{
    //engine.Start();

    engine.Initialize();

    Entity& camera = engine.DUCKENGINE_EntityManager.CreateEntity();

    // POS 0, 0, ZOOM 1K, HEIGHT 1K, AR SET, LAYER DEFAULT 0
    engine.DUCKENGINE_ComponentManager.AddComponent<CameraComponent>(camera.EntityID,
        5.f, 10.f,
        1000.f,
        1000.f,
        1000.f / 1000.f
    );

    for (int i = 0; i < 8000; i++) 
    {
        SpawnSquare(engine);
    }

    //Entity& square = engine.DUCKENGINE_EntityManager.CreateEntity();
    //engine.DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square.EntityID, 100.0f, 100.0f, 60.f, 10.f, 10.f);
    //engine.DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square.EntityID, true);

    //Entity& square2 = engine.DUCKENGINE_EntityManager.CreateEntity();
    //engine.DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square2.EntityID, 500.0f, 500.0f, 0.f, 10.f, 200.f);
    //engine.DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square2.EntityID, true);

    //engine.DUCKENGINE_EntityManager.RemoveEntity(square.EntityID);
    //engine.DUCKENGINE_EntityManager.RemoveEntity(square2.EntityID);

    while (engine.Running()) 
    {
        //CheckAndSpawnSquare(engine, 1.0f);
        UpdateSquares(engine, 1.0F);
        

        engine.Update();
        engine.StartDraw();

        engine.Draw();
        
        
        engine.EndDraw();

    }

    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}