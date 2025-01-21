/******************************************************************************/
/*!
\file		SpatialGridSystem.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Jan 21 2025
\brief  	Implementation of the SpatialGrid class for handling
            collision detection and response between bounding boxes and circles.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "SpatialGridSystem.h"

// Define the static grid
std::unordered_map<std::pair<int, int>, std::vector<int>, SpatialGrid::pair_hash> SpatialGrid::grid;

// Add a collider to the grid
void SpatialGrid::AddToCell(int entityId, const Vec2& position, float cellSize) {
    auto cell = GetCell(position, cellSize);
    grid[cell].push_back(entityId);
}

// Get nearby colliders
std::vector<int> SpatialGrid::GetNearbyEntities(const Vec2& position, float cellSize) {
    std::vector<int> nearbyEntities;
    auto cell = GetCell(position, cellSize);

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            auto neighbor = std::make_pair(cell.first + dx, cell.second + dy);
            if (grid.find(neighbor) != grid.end()) {
                nearbyEntities.insert(nearbyEntities.end(), grid[neighbor].begin(), grid[neighbor].end());
            }
        }
    }

    return nearbyEntities;
}


// Clear the grid (called only at the beginning of a frame or reset)
void SpatialGrid::Clear() {
	grid.clear();
}

// Calculate the cell for a position
std::pair<int, int> SpatialGrid::GetCell(const Vec2& position, float cellSize) {
    return { static_cast<int>(position.x / cellSize), static_cast<int>(position.y / cellSize) };
}