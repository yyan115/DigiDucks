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


// Function to check if a point (circle) intersects with the bounding box (rectangle) of an obstacle
bool point_intersects_box(const Vec2& point, const Vec2& box, BoundingCircle* player, BoundingBox* object) {
    // Check if the circle is colliding with the box by checking if the circle intersects the box boundary
    float distFromPlayer = Vec2Dist(box, point);
    float playerRadius_ObjSizeX = player->getRadius() + object->getSize().x;
    float playerRadius_ObjSizeY = player->getRadius() + object->getSize().y;

    if (distFromPlayer <= playerRadius_ObjSizeX || distFromPlayer <= playerRadius_ObjSizeY) {
        return true;
    }
    
    return false;
}

// Check if a line between two points (representing enemy movement) intersects with the box obstacle
bool line_intersects_obstacle(const Node& start, const Node& end,BoundingCircle* player, BoundingBox* box) {
    // We need to check if the circle (enemy's movement) crosses any of the box's boundaries.
    // For simplicity, check if either endpoint of the movement is colliding with the box
    return point_intersects_box(start.position,box->getCenter(),player, box) || point_intersects_box(end.position, box->getCenter(), player, box);
}

bool is_valid_move(const Vec2& neighbor,const Vec2& point, BoundingCircle* player, BoundingBox* obstacles , BoundingCircle* object) {
    // Check collision with the player (bounding circle)
    float colliderrad = player->getRadius() + object->getRadius();
    if (Vec2Dist(neighbor,point) < colliderrad){
        return false; // The enemy collides with the player
    }

    // Check collision with any obstacles (bounding boxes)
    
    if (point_intersects_box(neighbor, obstacles->getCenter(),player,obstacles)) {
        return false; // Path is blocked by an obstacle
    }

    return true; // No collisions, the move is valid
}

// Get neighboring nodes (step towards the player, adjusted for smooth movement)
std::vector<Node> get_neighbors(const Node& current, const Node& player, float step_size) {
    std::vector<Node> neighbors;
    // Move towards the player in small steps
    Vec2 direction = player.position - current.position;
    float distance_to_player = Vec2Dist(current.position,player.position);

    if (distance_to_player > step_size) {
        // Normalize direction and create the new position
        Vec2 step = direction * (step_size / distance_to_player);
        neighbors.push_back({ current.position + step, 0, 0, 0, nullptr });
    }
    else {
        // If within step size, just move to the player
        neighbors.push_back({ player.position, 0, 0, 0, nullptr });
    }

    return neighbors;
}

// Reconstruct the path from the goal to the start node
std::vector<Node> reconstruct_path(Node* goal) {
    std::vector<Node> path;
    Node* current = goal;
    while (current != nullptr) {
        path.push_back(*current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

float heuristic(const Node& current, const Node& goal) {
    return Vec2Dist(current.position, goal.position);
}

std::vector<Node> find_path(Node& start, Node& goal, float step_size, BoundingCircle* player, BoundingBox* box, BoundingCircle* enemy) {
    std::priority_queue<Node, std::vector<Node>, CompareNodes> open_list;  // Priority queue to choose the best node
    std::unordered_set<Node, NodeHash> closed_list;  // Set to store visited nodes, NodeHash is a custom hash function for Node

    start.g = 0;  // Starting cost is 0
    start.h = heuristic(start, goal);  // Heuristic value from start to goal
    start.f = start.g + start.h;  // Total cost (f = g + h)
    open_list.push(start);

    while (!open_list.empty()) {
        Node current = open_list.top();  // Get the node with the lowest f-value
        open_list.pop();

        // If we have reached the goal, reconstruct the path and return it
        if (current == goal) {
            return reconstruct_path(&current);  // Reconstruct and return the path
        }

        // Add the current node to the closed list (visited nodes)
        closed_list.insert(current);

        // Get neighbors (possible moves)
        for (auto& neighbor : get_neighbors(current, goal, step_size)) {
            // Check if the move is valid and if the neighbor is not in the closed list
            if (is_valid_move(neighbor.position, current.position, player, box, enemy) &&
                closed_list.find(neighbor) == closed_list.end()) {

                // Calculate g, h, and f for the neighbor
                neighbor.g = current.g + Vec2Dist(current.position, neighbor.position);
                neighbor.h = heuristic(neighbor, goal);
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.parent = &current;  // Set the parent node for path reconstruction

                // Push the neighbor onto the open list
                open_list.push(neighbor);
            }
        }
    }

    return {};  // No path found, return an empty vector
}

void workaround()
{

}


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
            float playerRadius_ObjSizeX = playerCircle->getRadius() + objBox->getSize().x;
            float playerRadius_ObjSizeY = playerCircle->getRadius() + objBox->getSize().y;
            if (distFromPlayer <= playerRadius_ObjSizeX || distFromPlayer <= playerRadius_ObjSizeY) {
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
            float playerRadius_ObjSizeX = playerCircle->getRadius() + objBox->getSize().x;
            float playerRadius_ObjSizeY = playerCircle->getRadius() + objBox->getSize().y;
			if (distFromPlayer <= playerRadius_ObjSizeX || distFromPlayer <= playerRadius_ObjSizeY) {
				objRb->isStatic = true;
				break;
			}
        }

        
        objRb->velocity = getSpeed(objTrf->position, playerTrf->position, state.objectSpeed);

        
        
        break;
    }
}

