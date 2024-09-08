#include "DuckEngine.h"

static DuckEngine engine;

int main(void)
{
    //engine.Start();

    engine.Initialize();
    Entity& square = engine.DUCKENGINE_EntityManager.CreateEntity();
    engine.DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square.EntityID, 10.0f, 10.0f);
    engine.DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square.EntityID, true);

    Entity& square2 = engine.DUCKENGINE_EntityManager.CreateEntity();
    engine.DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(square2.EntityID, 5.0f, 5.0f);
    engine.DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(square2.EntityID, true);

    engine.DUCKENGINE_EntityManager.RemoveEntity(square.EntityID);
    engine.DUCKENGINE_EntityManager.RemoveEntity(square2.EntityID);

    while (engine.Running()) 
    {
        engine.Update();
        engine.Draw();

        
    }

    std::cout << "Exited!\n";

    engine.Exit();

    //cleanup();

    return 0;
}