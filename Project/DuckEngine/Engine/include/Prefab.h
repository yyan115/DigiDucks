/******************************************************************************/
/*!
\file       Prefab.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the Prefab class, which represents a reusable template
			for creating entities in the game engine. A prefab contains
			information such as texture, position, scale, and a collection
			of components, and it can be instantiated to create new entities.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"
#include "Serialization.h"

/************************************************************************
@brief The Prefab class represents a reusable template for creating entities.
	   It contains the entity's name, texture, position, scale, and components,
	   and provides methods to add components and instantiate new entities.
*************************************************************************/
class Prefab
{
public:
	std::string name;
	std::string texturePath;
	Vec2 position;
	Vec2 scale;
	std::vector<std::shared_ptr<Component>> components;
	nlohmann::json componentsData;

	/************************************************************************
	@brief Constructor for the Prefab class. Initializes the prefab with a
		   name, optional texture path, and optional scale.
	@param name The name of the prefab.
	@param texturePath The path to the texture for the entity (default is empty).
	@param scale The scale of the entity (default is {1.0f, 1.0f}).
	*************************************************************************/
	Prefab(const std::string& name, const std::string& texturePath = "", Vec2 scale = { 1.0f, 1.0f })
		: name(name), texturePath(texturePath), scale(scale) {}

	/************************************************************************
	@brief Adds a component to the prefab. The component is stored and
		   will be included when the prefab is instantiated.
	@param component A shared pointer to the component to be added.
	*************************************************************************/
	void AddComponent(const std::shared_ptr<Component>& component);

	/************************************************************************
	@brief Instantiates the prefab to create a new entity at a specified position.
		   All components and data from the prefab are applied to the new entity.
	@param newPosition The position where the new entity will be placed.
	@return A pointer to the newly instantiated entity.
	*************************************************************************/
	Entity* Instantiate(Vec2 newPosition);
};