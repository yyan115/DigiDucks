/******************************************************************************/
/*!
\file		Bounding.h
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 13 2024
\brief  	This file includes BoundingCollider, BoundingCircle, and 
			BoundingBox classes

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#pragma once
#include "Vector2.h"
#include "Component.h"

class DUCKENGINE_API BoundingCollider {
private:
	Vec2 centerPos{};

public:
	// Constructor
	BoundingCollider() : centerPos(0.f, 0.f) {}
	BoundingCollider(const Vec2& pos) : centerPos(pos) {}
	BoundingCollider(float x, float y) : centerPos(x, y) {}

	// Destructor
	virtual ~BoundingCollider() = default;

	// Getters
	/****************************************************************
	* @brief Get the center position of the collider
	* 
	* @return The center position of the collider
	* ***************************************************************/
	Vec2 getCenterPos() const;

	// Setters
	/****************************************************************
	* @brief Set the center position of the collider
	* 
	* @param pos - The center position of the collider
	* ***************************************************************/
	void setCenterPos(const Vec2& pos);

	/****************************************************************
	* @brief Set the center position of the collider
	* 
	* @param x - The x position of the collider
	* 
	* @param y - The y position of the collider
	* ***************************************************************/
	void setCenterPos(float x, float y);


};


class DUCKENGINE_API BoundingBox : private BoundingCollider, public Component {
private:
	Vec2 size{};
	Vec2 max{};
	Vec2 min{};

public:
	// No default constructor
	BoundingBox() :BoundingCollider() {}

	// Constructor
	BoundingBox(const Vec2& _center, const Vec2& _size) : BoundingCollider(_center), size(_size), max(_center + _size), min(_center - _size) {}
	BoundingBox(float _x, float _y, float sizeX, float sizeY) : BoundingCollider(_x, _y), size(sizeX, sizeY), max(_x + sizeX, _y + sizeY), min(_x - sizeX, _y - sizeY) {}

	// Copy Constructor
	BoundingBox(BoundingBox& box) : BoundingCollider(box.getCenterPos()), size(box.getSize()), max(box.getMax()), min(box.getMin()) {}
	BoundingBox(const BoundingBox& box) : BoundingCollider(box.getCenterPos()), size(box.getSize()), max(box.getMax()), min(box.getMin()) {}

	// Destructor
	~BoundingBox() = default;

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoundingBox>(*this); 
	}

	// Getters
	/****************************************************************
	* @brief Get the center of the box
	* 
	* @return The center of the box
	* ***************************************************************/
	Vec2 getCenter() const;

	/****************************************************************
	* @brief Get the size of the box
	* 
	* @return The size of the box
	* ***************************************************************/
	Vec2 getSize() const;
	
	/****************************************************************
	* @brief Get the maximum point of the box
	* 
	* @return The maximum point of the box
	* ***************************************************************/
	Vec2 getMax() const;

	/****************************************************************
	* @brief Get the minimum point of the box
	* 
	* @return The minimum point of the box
	* ***************************************************************/
	Vec2 getMin() const;
	
	// Setters
	/****************************************************************
	* @brief Set the center of the box
	* 
	* @param pos - The center of the box
	* ***************************************************************/
	void setCenter(Vec2 pos);

	/****************************************************************
	* @brief Set the size of the box
	* 
	* @param _size - The size of the box
	* ***************************************************************/
	void setSize(Vec2 _size);
};


class DUCKENGINE_API BoundingCircle : private BoundingCollider, public Component {
private:
	float radius{};

public:
	// No default constructor
	BoundingCircle() :BoundingCollider(), radius(2.f) {}

	// Constructor
	BoundingCircle(const Vec2& center, float _radius) : BoundingCollider(center), radius(_radius) {};
	BoundingCircle(float x, float y, float _radius) : BoundingCollider(x, y), radius(_radius) {}

	// Copy Constructor
	BoundingCircle(BoundingCircle& circle) : BoundingCollider(circle.getCenterPos()), radius(circle.getRadius()) {}
	BoundingCircle(const BoundingCircle& circle) : BoundingCollider(circle.getCenterPos()), radius(circle.getRadius()) {}

	// Destructor
	~BoundingCircle() = default;

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoundingCircle>(*this);
	}

	// Getters
	/****************************************************************
	* @brief Get the center of the box
	*
	* @return The center of the box
	* ***************************************************************/
	Vec2 getCenter() const;

	/****************************************************************
	* @brief Get the radius of the circle
	* 
	* @return The radius of the circle
	* ***************************************************************/
	float getRadius() const;

	// Setters
	/****************************************************************
	* @brief Set the center of the circle
	* 
	* @param center - The center of the circle
	* ***************************************************************/
	void setCenter(Vec2 center);

	/****************************************************************
	* @brief Set the radius of the circle
	* 
	* @param radius - The radius of the circle
	* ***************************************************************/
	void setRadius(float radius);
};

// Check collision

/****************************************************************
* @brief Check collision between a circle and a box
*	
* @param circle - The circle collider
* 
* @param box - The box collider
* 
* @param circle_velo - The velocity of the circle
* 
* @param box_velo - The velocity of the box
* 
* @return True if there is a collision, false otherwise
* ***************************************************************/
DUCKENGINE_API bool checkCollisionCB(BoundingCircle& circle, Vec2& nextPos, BoundingBox& box);

/****************************************************************
* @brief Check collision between two boxes
* 
* @param box1 - The first box collider
* 
* @param box2 - The second box collider
* 
* @param box1_velo - The velocity of the first box
* 
* @param box2_velo - The velocity of the second box
* 
* @return True if there is a collision, false otherwise
* ***************************************************************/
DUCKENGINE_API bool checkCollisionBB(BoundingBox& box1, BoundingBox& box2, float deltaTime, Vec2 vel1 = Vec2(0.f, 0.f), Vec2 vel2 = Vec2(0.f, 0.f));

/****************************************************************
* @brief Check collision between two circles
* 
* @param circle1 - The first circle collider
* 
* @param circle2 - The second circle collider
* 
* @param vel1 - The velocity of the first circle
* 
* @param vel2 - The velocity of the second circle
*	
* @return True if there is a collision, false otherwise
* ***************************************************************/
DUCKENGINE_API bool checkCollisionCC(BoundingCircle& circle1, BoundingCircle& circle2, float deltaTime, Vec2 vel1 = Vec2(0.f, 0.f), Vec2 vel2 = Vec2(0.f, 0.f));

/****************************************************************
* @brief Check collision between a circle and a line
* 
* @param circle - The circle collider
* 
* @param nextPos - The next position of the circle
* 
* @param lineStart - The start point of the line
* 
* @param lineEnd - The end point of the line
* 
* @return True if there is a collision, false otherwise
* ***************************************************************/
DUCKENGINE_API bool checkCollisionCL(BoundingCircle& circle,const Vec2& nextPos, Vec2 lineStart, Vec2 lineEnd);