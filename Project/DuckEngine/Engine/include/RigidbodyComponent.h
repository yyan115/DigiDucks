/******************************************************************************/
/*!
\file		RigidbodyComponent.h
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 28 2024
\brief  	This file holds the component of Rigidbody for physics 
			simulations in the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Vector2.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API RigidbodyComponent : public Component
{
public:
	Vec2 velocity;		//< Velocity of the entity
	Vec2 acceleration;	//< Acceleration of the entity
	float mass;			//< Mass of the entity
	bool isStatic;		//< Indicates if the entity is static (not affected by physics)

	// Constructor initializes the RigidbodyComponent with default values
	RigidbodyComponent() : velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), mass(1.0f), isStatic(false) {}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<RigidbodyComponent>(*this);
	}
};