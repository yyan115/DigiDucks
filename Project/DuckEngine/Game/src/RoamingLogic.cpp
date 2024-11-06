#include "RoamingLogic.h"
#include <unordered_map>
#include <iostream>

namespace {
    Vec2 getSpeed(Vec2& firstPos, Vec2& secondPos, float speed) {
        Vec2 dir = secondPos - firstPos;
        dir = dir.normalized();
        dir *= speed;
        return dir;
    }

    // Structure to hold individual state data for each duck
    struct DuckState {
        bool reachedPos = false;
        bool firstToSecond = true;
        ObjectState state = Moving;
        float timer = 0.f;
    };

    // Map to track the state for each duck entity by its entityID
    std::unordered_map<int, DuckState> duckStates;
}

void RoamChar(Entity& object, Vec2& firstPos, Vec2& secondPos) {
    // Retrieve or initialize the duck's specific state
    DuckState& state = duckStates[object.entityID];

    // Get components
    TransformComponent* objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(object.entityID);
    RigidbodyComponent* objectRigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(object.entityID);
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
    TransformComponent* playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);

    // Check for missing components
    if (!objectTransform || !objectRigidbody || !playerTransform) {
        std::cerr << "Missing necessary components on DuckPrefab or Player entity." << std::endl;
        return;
    }

    // FSM to handle duck movement
    switch (state.state) {
    case Idle:
        if (Vec2Dist(objectTransform->position, playerTransform->position) < objectRange) {
            state.state = Chasing;
            std::cout << "Duck started chasing." << std::endl;
        }

        if (state.reachedPos) {
            if (state.timer >= waitTime) {
                state.reachedPos = false;
                state.timer = 0.f;
                state.state = Moving;
            }
            else {
                state.timer += DuckEngine::DeltaTime();
            }
        }
        else {
            state.state = Moving;
        }
        break;

    case Moving:
        if (Vec2Dist(objectTransform->position, playerTransform->position) < objectRange) {
            state.state = Chasing;
        }

        if (state.firstToSecond) {
            if (Vec2Dist(objectTransform->position, secondPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
            }
            objectRigidbody->velocity = getSpeed(objectTransform->position, secondPos, objectSpeed);
        }
        else {
            if (Vec2Dist(objectTransform->position, firstPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
            }
            objectRigidbody->velocity = getSpeed(objectTransform->position, firstPos, objectSpeed);
        }
        break;

    case Chasing:
        if (Vec2Dist(objectTransform->position, playerTransform->position) > (objectRange * 1.5f)) {
            state.state = Moving;
        }
        objectRigidbody->velocity = getSpeed(objectTransform->position, playerTransform->position, objectSpeed);
        break;
    }
}

void RoamAllDucks(Vec2 firstPos, Vec2 secondPos) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.prefabName == "DuckPrefab") {
            // Log each duck found to verify multiple instances are iterated
            std::cout << "Roaming DuckPrefab entity: " << entity.entityID << std::endl;
            RoamChar(entity, firstPos, secondPos);
        }
    }
}
