#include "DuckEngine.h"

static DuckEngine engine;

#include <random>
#include <chrono>

// Global variables for timing
std::chrono::time_point<std::chrono::steady_clock> lastSpawnTime = std::chrono::steady_clock::now();
float spawnInterval = 0.3f; // 1 second

// Random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> randomPosition(-500.0f, 500.0f);  // Position between 0 and 1000
std::uniform_real_distribution<float> randomScale(50.0f, 500.0f);      // Scale between 5 and 100
std::uniform_real_distribution<float> randomRotation(0.0f, 360.0f);   // Rotation between 0 and 360 degrees

namespace TESTCODE {
    void SpawnSquare(DuckEngine& engine) {
        // Generate random position, scale, and rotation
        float posX = randomPosition(gen);
        float posY = randomPosition(gen);
        float scaleX = randomScale(gen);
        float scaleY = randomScale(gen);
        float rotation = randomRotation(gen);

        // Create a new square entity
        Entity& square = engine.DUCKENGINE_EntityManager.CreateEntity();

        // Add transform component with randomized values
        engine.DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square.EntityID, scaleX, scaleY, rotation, posX, posY);

        // Add sprite renderer component
        engine.DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square.EntityID, true);

        // Print out the randomized values for debugging purposes
        std::cout << "Spawned square at position (" << posX << ", " << posY << "), scale (" << scaleX << ", " << scaleY << "), rotation " << rotation << std::endl;
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
        TESTCODE::CheckAndSpawnSquare(engine, 1.0f);

        engine.Update();
        engine.Draw();
    }

    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}