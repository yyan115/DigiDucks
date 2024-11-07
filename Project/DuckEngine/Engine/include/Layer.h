/******************************************************************************
\file       Layer.h
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Declaration of the Layer class, which represents a layer within a
            scene, containing entities, controlling their render order, and
            visibility.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once

#include <vector>
#include "Entity.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class Layer
{
public:
    /**************************************************************************
    @brief Adds an entity to the layer if it is not already present.
    @param entity Pointer to the entity to be added to the layer.
    **************************************************************************/
    DUCKENGINE_API void AddEntity(Entity* entity);

    /**************************************************************************
    @brief Retrieves the list of entities contained within this layer.
    @return A constant reference to a vector containing pointers to entities.
    **************************************************************************/
    DUCKENGINE_API const std::vector<int>& GetEntityIDs() const;

    /**************************************************************************
    @brief Retrieves the render order of the layer.
    @return The integer order of the layer.
    **************************************************************************/
    DUCKENGINE_API int GetOrder() const;

    /**************************************************************************
    @brief Sets the render order of the layer.
    @param _order Integer order value to set for the layer.
    **************************************************************************/
    DUCKENGINE_API void SetOrder(int _order);

    /**************************************************************************
    @brief Checks if the layer is visible.
    @return True if the layer is visible; otherwise, false.
    **************************************************************************/
    DUCKENGINE_API bool IsVisible() const;

    /**************************************************************************
    @brief Sets the visibility of the layer.
    @param _visible Boolean value to set the layer's visibility.
    **************************************************************************/
    DUCKENGINE_API void SetVisible(bool _visible);

    DUCKENGINE_API bool HasEntityByID(int entityID) const;

private:
    std::vector<int> entityIDs; // Entities in this layer
    int order = 1;                     // Order of the layer
    bool visible = true;                  // Visibility of the layer
    std::string name;
};
