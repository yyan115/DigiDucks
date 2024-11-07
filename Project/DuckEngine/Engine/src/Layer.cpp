/******************************************************************************
\file       Layer.cpp
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Implementation of the Layer class methods, allowing manipulation
            of entities within a layer, as well as control of layer order
            and visibility.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#include "Layer.h"
#include "Entity.h"

/**************************************************************************
@brief Adds an entity to the layer if it is not already present.
@param entity Pointer to the entity to be added to the layer.
**************************************************************************/
void Layer::AddEntity(Entity* entity)
{
    int entityID = entity->entityID;
    if (std::find(entityIDs.begin(), entityIDs.end(), entityID) == entityIDs.end())
    {
        entityIDs.push_back(entityID);
    }
}


/**************************************************************************
@brief Retrieves the list of entities contained within this layer.
@return A constant reference to a vector containing pointers to entities.
**************************************************************************/
const std::vector<int>& Layer::GetEntityIDs() const
{
    return entityIDs;
}


/**************************************************************************
@brief Retrieves the render order of the layer.
@return The integer order of the layer.
**************************************************************************/
int Layer::GetOrder() const
{
    return order;
}

/**************************************************************************
@brief Sets the render order of the layer.
@param _order Integer order value to set for the layer.
**************************************************************************/
void Layer::SetOrder(int _order)
{
    this->order = _order;
}

/**************************************************************************
@brief Checks if the layer is visible.
@return True if the layer is visible; otherwise, false.
**************************************************************************/
bool Layer::IsVisible() const
{
    return visible;
}

/**************************************************************************
@brief Sets the visibility of the layer.
@param _visible Boolean value to set the layer's visibility.
**************************************************************************/
void Layer::SetVisible(bool _visible)
{
    this->visible = _visible;
}

bool Layer::HasEntityByID(int entityID) const
{
    return std::find(entityIDs.begin(), entityIDs.end(), entityID) != entityIDs.end();
}

