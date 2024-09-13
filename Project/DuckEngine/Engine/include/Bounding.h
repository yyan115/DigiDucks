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

#pragma once
#include "Vector2.h"

class BoundingCollider {
	Vec2 centerPos{};

public:	

	// Constructor
	BoundingCollider() : centerPos(0.f, 0.f){}
	BoundingCollider(const Vec2& pos) : centerPos(pos){}
	BoundingCollider(float x, float y) : centerPos(x, y){}

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


class BoundingBox : public BoundingCollider {
private:
	Vec2 size{};

public:

	// No default constructor
	BoundingBox() = delete;

	// Constructor
	BoundingBox(const Vec2& center,const Vec2& size) : BoundingCollider(center), size(size) {};
	BoundingBox(float _x, float _y, float sizeX, float sizeY) : BoundingCollider(_x, _y), size(sizeX, sizeY) {}

	// Getters
	Vec2 getCenter() const;
	Vec2 getSize() const;

	// Setters
	void setCenter(Vec2 pos);
	void setSize(Vec2 _size);
};


class BoundingCircle : public BoundingCollider {
private:
	float radius{};

public:

	// No default constructor
	BoundingCircle() = delete;

	// Constructor
	BoundingCircle(const Vec2& center, float _radius) : BoundingCollider(center), radius(_radius) {};
	BoundingCircle(float x, float y, float _radius) : BoundingCollider(x, y), radius(_radius) {}

	// Getters

	/****************************************************************
	* @brief Get the center of the circle
	* 
	* @return The center of the circle
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
bool checkCollision(BoundingCircle& circle, Vec2&& circle_velo = Vec2(0.f, 0.f), BoundingBox& box,  Vec2&& box_velo = Vec2(0.f,0.f));

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
bool checkCollision(BoundingBox& box1, Vec2&& vel1 = Vec2(0.f, 0.f), BoundingBox& box2,  Vec2&& vel2 = Vec2(0.f, 0.f));

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
bool checkCollision(BoundingCircle& circle1, Vec2&& vel1 = Vec2(0.f, 0.f), BoundingCircle& circle2, Vec2&& vel2 = Vec2(0.f, 0.f));

