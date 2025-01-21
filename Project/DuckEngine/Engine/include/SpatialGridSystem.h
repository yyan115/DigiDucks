/******************************************************************************/
/*!
\file		SpatialGridSystem.h
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
#pragma once
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include "math.h"
#include "Vector2.h"

class SpatialGrid {
private:
    // Calculate the cell for a position
    static std::pair<int, int> GetCell(const Vec2& position, float cellSize);

    // Hash function for grid cells
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ (hash2 << 1);
        }
    };

public:
    float cellSize;

    // Unified grid shared by all colliders
    static std::unordered_map<std::pair<int, int>, std::vector<int>, pair_hash> grid;

    SpatialGrid(float cellSize) : cellSize(cellSize) {}

    // Add a collider to the grid
    static void AddToCell(int entityId, const Vec2& position, float cellSize);

    // Get nearby colliders
    static std::vector<int> GetNearbyEntities(const Vec2& position, float cellSize);

    // Clear the grid (called only at the beginning of a frame or reset)
    static void Clear();

};

