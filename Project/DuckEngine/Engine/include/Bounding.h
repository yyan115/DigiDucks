/******************************************************************************/
/*!
\file		Bounding.h
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 26 2024
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
#include <functional>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"

class DUCKENGINE_API BoundingCollider {
private:
	Vec2 centerPos{};

public:
	// Constructor
	BoundingCollider() : centerPos(0.f, 0.f) {}
	BoundingCollider(const Vec2& pos) : centerPos(pos) {}
	BoundingCollider(float x, float y) : centerPos(x, y) {}

	typedef std::function<void(Entity*)> OnCollisionCallback;

	OnCollisionCallback onCollisionCallback = nullptr;

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

	void SetCollisionCallback(OnCollisionCallback callback)
	{
		onCollisionCallback = callback;
	}
};


class DUCKENGINE_API BoundingBox : public BoundingCollider, public Component {
private:
	Vec2 size{};
	Vec2 topR{};
	Vec2 topL{};
	Vec2 btmR{};
	Vec2 btmL{};

public:
	float rotation{};

	// No default constructor
	BoundingBox() :BoundingCollider() {}

	// Constructor
	BoundingBox(const Vec2& _center, const Vec2& _size, float _rotation = 0.f);
	BoundingBox(float _x, float _y, float sizeX, float sizeY, float _rotation = 0.f);

	// Copy Constructor
	BoundingBox(BoundingBox& box) : 
		BoundingCollider(box.getCenter()), size(box.size), topR(box.topR), topL(box.topL), btmR(box.btmR), btmL(box.btmL), rotation(box.rotation) {}
	BoundingBox(const BoundingBox& box):
		BoundingCollider(box.getCenter()), size(box.size), topR(box.topR), topL(box.topL), btmR(box.btmR), btmL(box.btmL), rotation(box.rotation) {}

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
	Vec2 getCenter() const { return getCenterPos(); }

	/****************************************************************
	* @brief Get the size of the box
	* 
	* @return The size of the box
	* ***************************************************************/
	Vec2 getSize() const { return size; }
	
	/****************************************************************
	* @brief Get the top right corner of the box
	* 
	* @return The top right corner of the box
	* ***************************************************************/
	Vec2 getTopR() const { return topR; }

	/****************************************************************
	* @brief Get the top left corner of the box
	* 
	* @return The top left corner of the box
	* ***************************************************************/
	Vec2 getTopL() const { return topL; }

	/****************************************************************
	* @brief Get the bottom right corner of the box
	* 
	* @return The bottom right corner of the box
	* ***************************************************************/
	Vec2 getBtmR() const { return btmR; }

	/****************************************************************
	* @brief Get the bottom left corner of the box
	* 
	* @return The bottom left corner of the box
	* ***************************************************************/
	Vec2 getBtmL() const { return btmL; }
	
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

	/****************************************************************
	* @brief Set the top right corner of the box
	* 
	* @param _topR - The top right corner of the box
	* ***************************************************************/
	void rotate(float angle);
};


class DUCKENGINE_API BoundingCircle : public BoundingCollider, public Component {
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
DUCKENGINE_API bool checkCollisionCB(BoundingCircle& circle, BoundingBox& box, float deltaTime, Vec2 cir_vel = Vec2{ 0,0 }, Vec2 box_vel = Vec2{ 0,0 });

/****************************************************************
* @brief Check collision between a box and a circle
* 
* @param box - The box collider
* 
* @param circle - The circle collider
* 
* @param deltaTime - The time between frames
* 
* @param box_vel - The velocity of the box
* 
* @param cir_vel - The velocity of the circle
* 
* @return True if there is a collision, false otherwise
* ***************************************************************/
DUCKENGINE_API bool checkCollisionBC(BoundingBox& box, BoundingCircle& circle, float deltaTime, Vec2 box_vel = Vec2{ 0,0 }, Vec2 cir_vel = Vec2{ 0,0 });

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