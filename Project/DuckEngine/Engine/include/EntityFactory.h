/******************************************************************************/
/*!
\file       EntityFactory.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the EntityFactory class responsible for creating and
			initializing entities in the game engine, with optional
			configurations such as position, scale, and texture.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <vector>
#include "Entity.h"
#include "Vector2.h"
#include "Component.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API EntityFactory
{
public:
	/************************************************************************
	@brief Creates a new entity with default values.
	@return A pointer to the newly created Entity.
	*************************************************************************/
	static Entity* CreateEntity();

	/************************************************************************
	@brief Creates a new entity with specified position and scale.
	@param position The initial position of the entity.
	@param scale The initial scale of the entity.
	@return A pointer to the newly created Entity.
	*************************************************************************/
	static Entity* CreateEntity(Vec2 position, Vec2 scale);

	/************************************************************************
	@brief Creates a new entity with specified texture, position, and scale.
	@param texturePath The path to the texture file for the entity.
	@param position The initial position of the entity.
	@param scale The initial scale of the entity.
	@return A pointer to the newly created Entity.
	*************************************************************************/
	static Entity* CreateEntity(const std::string& texturePath, Vec2 position, Vec2 scale);
};