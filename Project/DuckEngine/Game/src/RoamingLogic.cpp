/******************************************************************************/
/*!
\file		RoamingLogic.CPP
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Nov 6 2024
\brief		Definition of Roaming Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

/**
* @brief
* Function to roam any object with a specific prefab name between two positions
*
* @param prefabName - The name of the prefab to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamSelectedPrefab(std::string prefabName, Vec2 firstPos, Vec2 secondPos) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.prefabName == prefabName) {
            RoamTwoPos(entity.entityID, firstPos, secondPos);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object name between two positions
*
* @param objectName - The name of the object to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamSelectedObject(std::string objectName, Vec2 firstPos, Vec2 secondPos) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.name == objectName) {
            RoamTwoPos(entity.entityID, firstPos, secondPos);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object ID between two positions
*
* @param objectID - The ID of the object to roam
* @param firstPos - The first position to roam to
* @param secondPos - The second position to roam to
*/
void RoamTwoPos(int objectID, Vec2& firstPos, Vec2& secondPos) {
    // Retrieve or initialize the Object's specific state
    ObjectDatas& state = objectState[objectID];

    // Get components
    TransformComponent* objTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectID);
    RigidbodyComponent* objRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(objectID);
    BoundingBox* objBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(objectID);
    BoundingCircle* objCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(objectID);
    if (objRb->isStatic) { objRb->isStatic = false; }

	// Get player components
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
    TransformComponent* playerTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
	BoundingCircle* playerCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);

    // Check for missing components
    if (!objTrf || !objRb || !playerTrf) {
        std::cerr << "Missing necessary components on ObjectPrefab or Player entity." << std::endl;
        return;
    }

    // FSM to handle object movement
    switch (state.state) {
    case Idle:
		// Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

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
        // Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

        if (state.firstToSecond) {
            if (Vec2Dist(objTrf->position, secondPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
            }
            objRb->velocity = getSpeed(objTrf->position, secondPos, state.objectSpeed);
        }
        else {
            if (Vec2Dist(objTrf->position, firstPos) <= 0.1f) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
            }
            objRb->velocity = getSpeed(objTrf->position, firstPos, state.objectSpeed);
        }
        break;

    case Chasing:
        float distFromPlayer = Vec2Dist(objTrf->position, playerTrf->position);
        if (distFromPlayer > (state.objectRange * 1.5f)) { state.state = Idle; }

        // Check if object has a circle or box collider
        if (!objBox) {
            float playerRadius_ObjRadius = playerCircle->getRadius() + objCircle->getRadius();
            if (distFromPlayer <= playerRadius_ObjRadius) {
                objRb->isStatic = true;
                break;
            }
        }
        else {
            float playerRadius_ObjSize = playerCircle->getRadius() + objBox->getSize().length();
            if (distFromPlayer <= playerRadius_ObjSize) {
                objRb->isStatic = true;
                break;
            }
        }

        objRb->velocity = getSpeed(objTrf->position, playerTrf->position, state.objectSpeed);

        break;
    }
}

/*****************************************************************************************/

/**
* @brief
* Function to roam any object with a specific prefab name in a specific direction
* for a specific time
*
* @param prefabName - The name of the prefab to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamSelectedPrefab(std::string prefabName, Vec2 dir, float time) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.prefabName == prefabName) {
            RoamDir(entity.entityID, dir, time);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object name in a specific direction
* for a specific time
*
* @param objectName - The name of the object to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamSelectedObject(std::string objectName, Vec2 dir, float time) {
    auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();
    for (auto& entity : entities) {
        if (entity.name == objectName) {
            RoamDir(entity.entityID, dir, time);
        }

    }
}

/**
* @brief
* Function to roam any object with a specific object ID in a specific direction
* for a specific time
*
* @param objectID - The ID of the object to roam
* @param dir - The direction to roam in
* @param time - The time to roam in that direction
*/
void RoamDir(int objectID, Vec2& dir, float time) {
    // Retrieve or initialize the Object's specific state
    ObjectDatas& state = objectState[objectID];

    // Get components
    TransformComponent* objTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(objectID);
    RigidbodyComponent* objRb = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(objectID);
	BoundingBox* objBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(objectID);
	BoundingCircle* objCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(objectID);
    if (objRb->isStatic) { objRb->isStatic = false; }

    // Get player components
    Entity* player = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Player");
    TransformComponent* playerTrf = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(player->entityID);
    BoundingCircle* playerCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(player->entityID);

    // Check for missing components
    if (!objTrf || !objRb || !playerTrf) {
        std::cerr << "Missing necessary components on ObjectPrefab or Player entity." << std::endl;
        return;
    }
    
    // FSM to handle object movement
    switch (state.state) {
    case Idle:
        // Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

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
        // Check if player is within range
        if (Vec2Dist(objTrf->position, playerTrf->position) < state.objectRange) { state.state = Chasing; }

        if (state.firstToSecond) {
            if (state.timer >= time) {
                state.reachedPos = true;
                state.firstToSecond = false;
                state.state = Idle;
				state.timer = 0.f;
            }
			objRb->velocity = dir.normalized() * state.objectSpeed;
			state.timer += DuckEngine::DeltaTime();
        }
        else {
            if (state.timer >= time) {
                state.reachedPos = true;
                state.firstToSecond = true;
                state.state = Idle;
                state.timer = 0.f;
            }
            objRb->velocity = -dir.normalized() * state.objectSpeed;
            state.timer += DuckEngine::DeltaTime();
        }
        break;

    case Chasing:
        float distFromPlayer = Vec2Dist(objTrf->position, playerTrf->position);
        if (distFromPlayer > (state.objectRange * 1.5f)) { state.state = Idle; }

        // Check if object has a circle or box collider
        if (!objBox) {
			float playerRadius_ObjRadius = playerCircle->getRadius() + objCircle->getRadius();
            if (distFromPlayer <= playerRadius_ObjRadius) {
                objRb->isStatic = true;
                break;
            }
        }
        else {
            float playerRadius_ObjSize = playerCircle->getRadius() + objBox->getSize().length();
			if (distFromPlayer <= playerRadius_ObjSize) {
				objRb->isStatic = true;
				break;
			}
        }

        objRb->velocity = getSpeed(objTrf->position, playerTrf->position, state.objectSpeed);
        break;
    }
}

