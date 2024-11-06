#include "RoamingLogic.h"

namespace {
    Vec2 getSpeed(Vec2& firstPos, Vec2& secondPos, float speed) {
        Vec2 dir = secondPos - firstPos;
        dir = dir.normalized();
        dir *= speed;
        return dir;
    }
}

// Define the global objectState variable here
std::unordered_map<int, ObjectDatas> objectState;

void RoamChar(int objectID, Vec2& firstPos, Vec2& secondPos) {
    // Retrieve or initialize the duck's specific state
    ObjectDatas& state = objectState[objectID];

    // Get components
    TransformComponent* objectTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectID);
    RigidbodyComponent* objectRigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(objectID);
	if (objectRigidbody->isStatic) {
		objectRigidbody->isStatic = false;
	}

	// Get player components
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
    TransformComponent* playerTransform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);

    // Check for missing components
    if (!objectTransform || !objectRigidbody || !playerTransform) {
        std::cerr << "Missing necessary components on ObjectPrefab or Player entity." << std::endl;
        return;
    }

    // FSM to handle duck movement
    switch (state.state) {
    case Idle:
        if (Vec2Dist(objectTransform->position, playerTransform->position) < state.objectRange) { state.state = Chasing; }

        if (state.reachedPos) {
            if (state.timer >= state.waitTime) {
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
        if (Vec2Dist(objectTransform->position, playerTransform->position) < state.objectRange) { state.state = Chasing; }

        if (state.firstToSecond) {
            if (Vec2Dist(objectTransform->position, secondPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
            }
            objectRigidbody->velocity = getSpeed(objectTransform->position, secondPos, state.objectSpeed);
        }
        else {
            if (Vec2Dist(objectTransform->position, firstPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
            }
            objectRigidbody->velocity = getSpeed(objectTransform->position, firstPos, state.objectSpeed);
        }
        break;

    case Chasing:
        if (Vec2Dist(objectTransform->position, playerTransform->position) > (state.objectRange * 1.5f)) { state.state = Moving; }

        objectRigidbody->velocity = getSpeed(objectTransform->position, playerTransform->position, state.objectSpeed);
        break;
    }
}

void RoamSelectedObject(std::string prefabName, Vec2 firstPos, Vec2 secondPos) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.prefabName == prefabName) {
            // Log each duck found to verify multiple instances are iterated
            std::cout << "Roaming " << prefabName << " entity: " << entity.entityID << std::endl;
            RoamChar(entity.entityID, firstPos, secondPos);
        }

    }
}
