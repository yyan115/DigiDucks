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
	float angle;
	Vec2 scale;
	bool relativeToCamera;
	Vec2 previousPosition;
	Vec2 localPosition;
	Vec2 worldPosition;


	/************************************************************************
	@brief The TransformComponent class holds the position, rotation (angle),
		   and scale of an entity in the game engine. It also includes a flag
		   to indicate whether the transform is relative to the camera.
	*************************************************************************/
	TransformComponent()
		: angle(0.0f), scale(1.0f, 1.0f), relativeToCamera(true),
		previousPosition(0.0f, 0.0f), localPosition(0.0f, 0.0f),
		worldPosition(0.0f, 0.0f) {}
	
	/************************************************************************
	@brief Constructor for TransformComponent with specified position and scale.
	@param pos The initial position of the entity.
	@param scaleVec2 The initial scale of the entity.
	*************************************************************************/
	TransformComponent(Vec2 pos, Vec2 scaleVec2)
		: angle(0.0f), scale(scaleVec2.x, scaleVec2.y), relativeToCamera(true),
		previousPosition(pos.x, pos.y), localPosition(pos.x, pos.y),
		worldPosition(pos.x, pos.y) {
	};
	
	/************************************************************************
	@brief Constructor for TransformComponent with specified position, scale,
		   and rotation angle.
	@param scaleX The scale of the entity along the x-axis.
	@param scaleY The scale of the entity along the y-axis.
	@param angle The rotation angle of the entity.
	@param posX The x-coordinate of the entity's position.
	@param posY The y-coordinate of the entity's position.
	*************************************************************************/
	TransformComponent(float scaleX, float scaleY, float angle, float posX, float posY)
		: angle(angle), scale(scaleX, scaleY), relativeToCamera(true),
		previousPosition(posX, posY), localPosition(posX, posY),
		worldPosition(posX, posY) {};
	
	/************************************************************************
	@brief Constructor for TransformComponent with references to position,
		   scale, and angle.
	@param scale Reference to the scale vector of the entity.
	@param angle The rotation angle of the entity.
	@param position Reference to the position vector of the entity.
	*************************************************************************/
	TransformComponent(Vec2& scale, float angle, Vec2& position)
		: angle(angle), scale(scale), relativeToCamera(true),
		previousPosition(position), localPosition(position), worldPosition(position) {};
	
	/************************************************************************
	@brief Constructor for TransformComponent with rvalue references to
		   position, scale, and angle.
	@param scale Rvalue reference to the scale vector of the entity.
	@param angle The rotation angle of the entity.
	@param position Rvalue reference to the position vector of the entity.
	*************************************************************************/
	TransformComponent(Vec2&& scale, float angle, Vec2&& position)
		: angle(angle), scale(scale), relativeToCamera(true),
		previousPosition(position), localPosition(position), worldPosition(position) {};
	
	/************************************************************************
	@brief Clones the current TransformComponent instance. This method is
		   used to create a deep copy of the component.
	@return A shared pointer to the newly cloned TransformComponent.
	*************************************************************************/
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<TransformComponent>(*this);
	}

	void SetPosition(const Vec2& newPos);

	Vec2& GetPosition();
	Vec2& GetLocalPosition();

	void UpdateChildPositions(const Vec2& delta);

};
