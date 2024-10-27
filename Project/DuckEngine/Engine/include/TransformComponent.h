/******************************************************************************/
/*!
\file       TransformComponent.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the TransformComponent class, which represents the
			position, rotation, and scale of an entity in the game engine.
			This component also includes the option to set the transform
			relative to the camera.

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

class DUCKENGINE_API TransformComponent : public Component
{
  public:
	Vec2 position;
	float angle;
	Vec2 scale;
	bool relativeToCamera;

	/************************************************************************
	@brief The TransformComponent class holds the position, rotation (angle),
		   and scale of an entity in the game engine. It also includes a flag
		   to indicate whether the transform is relative to the camera.
	*************************************************************************/
	TransformComponent() : scale(1.0f, 1.0f), angle(0.0f), position(0.0f, 0.0f), relativeToCamera(true) {}
	
	/************************************************************************
	@brief Constructor for TransformComponent with specified position and scale.
	@param pos The initial position of the entity.
	@param scaleVec2 The initial scale of the entity.
	*************************************************************************/
	TransformComponent(Vec2 pos, Vec2 scaleVec2) : scale(scaleVec2.x, scaleVec2.y), angle(0), position(pos.x, pos.y), relativeToCamera(true) {};
	
	/************************************************************************
	@brief Constructor for TransformComponent with specified position, scale,
		   and rotation angle.
	@param scaleX The scale of the entity along the x-axis.
	@param scaleY The scale of the entity along the y-axis.
	@param angle The rotation angle of the entity.
	@param posX The x-coordinate of the entity's position.
	@param posY The y-coordinate of the entity's position.
	*************************************************************************/
	TransformComponent(float scaleX, float scaleY, float angle, float posX, float posY) : scale(scaleX, scaleY), angle(angle), position(posX, posY), relativeToCamera(true) {};
	
	/************************************************************************
	@brief Constructor for TransformComponent with references to position,
		   scale, and angle.
	@param scale Reference to the scale vector of the entity.
	@param angle The rotation angle of the entity.
	@param position Reference to the position vector of the entity.
	*************************************************************************/
	TransformComponent(Vec2& scale, float angle, Vec2& position) : scale(scale), angle(angle), position(position), relativeToCamera(true) {};
	
	/************************************************************************
	@brief Constructor for TransformComponent with rvalue references to
		   position, scale, and angle.
	@param scale Rvalue reference to the scale vector of the entity.
	@param angle The rotation angle of the entity.
	@param position Rvalue reference to the position vector of the entity.
	*************************************************************************/
	TransformComponent(Vec2&& scale, float angle, Vec2&& position) : scale(scale), angle(angle), position(position), relativeToCamera(true) {};
	
	/************************************************************************
	@brief Clones the current TransformComponent instance. This method is
		   used to create a deep copy of the component.
	@return A shared pointer to the newly cloned TransformComponent.
	*************************************************************************/
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<TransformComponent>(*this);
	}

	void SetPosition(const Vec2& newPos)
	{
		position = newPos;
	}


};