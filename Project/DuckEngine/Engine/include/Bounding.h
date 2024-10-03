/******************************************************************************/
/*!
\file		Bounding.h
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 26 2024
\brief  	This file contains functions for detecting various types of 
            collisions in a 2D environment, including circle-box, 
            box-box, circle-circle, and circle-line segment collisions.

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
template class __declspec(dllexport) std::function<void(Entity*)>;

class DUCKENGINE_API BoundingCollider {
private:
	Vec2 centerPos{};

public:
	// Constructor
	/****************************************************************
	* @brief Default constructor for BoundingCollider
	*
	* Initializes the center position of the collider to (0, 0).
	****************************************************************/
	BoundingCollider() : centerPos(0.f, 0.f) {}

	/****************************************************************
	* @brief Constructor with center position
	*
	* @param pos The initial center position of the collider as a Vec2
	****************************************************************/
	BoundingCollider(const Vec2& pos) : centerPos(pos) {}
	/****************************************************************
	* @brief Constructor with x and y coordinates
	*
	* @param x The x-coordinate of the center position
	* @param y The y-coordinate of the center position
	****************************************************************/
	BoundingCollider(float x, float y) : centerPos(x, y) {}

	typedef std::function<void(Entity*)> OnCollisionCallback;

	OnCollisionCallback onCollisionCallback = nullptr;

	// Destructor
	/****************************************************************
	* @brief Virtual destructor for BoundingCollider
	*
	* Allows for proper cleanup when objects are deleted through a pointer
	* to the base class.
	****************************************************************/
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

	/****************************************************************
	* @brief Set the collision callback function
	*
	* @param callback The function to be called upon collision
	****************************************************************/
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
	/****************************************************************
	* @brief Constructor for BoundingBox
	*
	* Initializes the bounding box with a center position, size, and optional rotation.
	*
	* @param _center The center position of the bounding box as a Vec2
	* @param _size The size of the bounding box as a Vec2
	* @param _rotation The rotation of the bounding box in degrees (default is 0)
	****************************************************************/
	BoundingBox(const Vec2& _center, const Vec2& _size, float _rotation = 0.f);
	/****************************************************************
	* @brief Constructor for BoundingBox with explicit coordinates for center and size
	*
	* @param _x The x-coordinate of the center of the bounding box
	* @param _y The y-coordinate of the center of the bounding box
	* @param sizeX The width of the bounding box
	* @param sizeY The height of the bounding box
	* @param _rotation The rotation of the bounding box in degrees (default is 0)
	****************************************************************/
	BoundingBox(float _x, float _y, float sizeX, float sizeY, float _rotation = 0.f);

	// Copy Constructor
	/****************************************************************
	* @brief Copy constructor for BoundingBox
	*
	* Initializes a BoundingBox by copying the data from another bounding box.
	*
	* @param box The BoundingBox object to copy from
	****************************************************************/
	BoundingBox(BoundingBox& box) : 
		BoundingCollider(box.getCenter()), size(box.size), topR(box.topR), topL(box.topL), btmR(box.btmR), btmL(box.btmL), rotation(box.rotation) {}
	/****************************************************************
	* @brief Copy constructor for BoundingBox (const version)
	*
	* Initializes a BoundingBox by copying the data from another constant bounding box.
	*
	* @param box The constant BoundingBox object to copy from
	****************************************************************/
	BoundingBox(const BoundingBox& box):
		BoundingCollider(box.getCenter()), size(box.size), topR(box.topR), topL(box.topL), btmR(box.btmR), btmL(box.btmL), rotation(box.rotation) {}

	// Destructor
	/****************************************************************
	* @brief Destructor for BoundingBox
	*
	* Performs necessary cleanup of the BoundingBox object.
	****************************************************************/
	~BoundingBox() = default;

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoundingBox>(*this); 
	}

	// Getters
	/****************************************************************
	* @brief Get the center position of the bounding box
	*
	* @return The center position of the box as a Vec2
	****************************************************************/
	Vec2 getCenter() const { return getCenterPos(); }

	/****************************************************************
	* @brief Get the size of the bounding box
	*
	* @return The size of the box as a Vec2
	****************************************************************/
	Vec2 getSize() const { return size; }
	
	/****************************************************************
	* @brief Get the top right corner of the bounding box
	*
	* @return The top right corner of the box as a Vec2
	****************************************************************/
	Vec2 getTopR() const { return topR; }

	/****************************************************************
	* @brief Get the top left corner of the bounding box
	*
	* @return The top left corner of the box as a Vec2
	****************************************************************/
	Vec2 getTopL() const { return topL; }

	/****************************************************************
	* @brief Get the bottom right corner of the bounding box
	*
	* @return The bottom right corner of the box as a Vec2
	****************************************************************/
	Vec2 getBtmR() const { return btmR; }

	/****************************************************************
	* @brief Get the bottom left corner of the bounding box
	*
	* @return The bottom left corner of the box as a Vec2
	****************************************************************/
	Vec2 getBtmL() const { return btmL; }
	
	// Setters
	/****************************************************************
	* @brief Set the center position of the bounding box
	*
	* @param pos The new center position of the bounding box as a Vec2
	****************************************************************/
	void setCenter(Vec2 pos);

	/****************************************************************
	* @brief Set the size of the bounding box
	*
	* @param _size The new size of the bounding box as a Vec2
	****************************************************************/
	void setSize(Vec2 _size);

	/****************************************************************
	* @brief Rotate the bounding box by a given angle
	*
	* @param angle The rotation angle in degrees
	****************************************************************/
	void rotate(float angle);
};


class DUCKENGINE_API BoundingCircle : public BoundingCollider, public Component {
private:
	float radius{};

public:
	/****************************************************************
	* @brief Default constructor for BoundingCircle
	*
	* Initializes a bounding circle with a default radius of 2.0f and a
	* default center position set to (0, 0).
	****************************************************************/
	BoundingCircle() :BoundingCollider(), radius(2.f) {}

	// Constructor
	/****************************************************************
	* @brief Constructor for BoundingCircle
	*
	* Initializes a bounding circle with a given center position and radius.
	*
	* @param center The center position of the bounding circle as a Vec2
	* @param _radius The radius of the bounding circle
	****************************************************************/
	BoundingCircle(const Vec2& center, float _radius) : BoundingCollider(center), radius(_radius) {};
	/****************************************************************
	* @brief Constructor for BoundingCircle with explicit coordinates
	*
	* Initializes a bounding circle with a center defined by explicit
	* x and y coordinates and a radius.
	*
	* @param x The x-coordinate of the center of the circle
	* @param y The y-coordinate of the center of the circle
	* @param _radius The radius of the bounding circle
	****************************************************************/
	BoundingCircle(float x, float y, float _radius) : BoundingCollider(x, y), radius(_radius) {}

	// Copy Constructor
	/****************************************************************
	* @brief Copy constructor for BoundingCircle
	*
	* Initializes a BoundingCircle by copying the data from another BoundingCircle.
	*
	* @param circle The BoundingCircle object to copy from
	****************************************************************/
	BoundingCircle(BoundingCircle& circle) : BoundingCollider(circle.getCenterPos()), radius(circle.getRadius()) {}
	/****************************************************************
	* @brief Const copy constructor for BoundingCircle
	*
	* Initializes a BoundingCircle by copying the data from another constant BoundingCircle.
	*
	* @param circle The constant BoundingCircle object to copy from
	****************************************************************/
	BoundingCircle(const BoundingCircle& circle) : BoundingCollider(circle.getCenterPos()), radius(circle.getRadius()) {}

	// Destructor
	/****************************************************************
	* @brief Destructor for BoundingCircle
	*
	* Cleans up resources used by the BoundingCircle object.
	****************************************************************/
	~BoundingCircle() = default;

	/****************************************************************
	* @brief Clone method for BoundingCircle
	*
	* Creates a new instance of BoundingCircle, copying the current object.
	*
	* @return A shared pointer to the new BoundingCircle instance
	****************************************************************/
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoundingCircle>(*this);
	}

	// Getters
	/****************************************************************
	* @brief Get the center position of the bounding circle
	*
	* @return The center of the circle as a Vec2
	****************************************************************/
	Vec2 getCenter() const;

	/****************************************************************
	* @brief Get the radius of the bounding circle
	*
	* @return The radius of the circle as a float
	****************************************************************/
	float getRadius() const;

	// Setters
	/****************************************************************
	* @brief Set the center position of the bounding circle
	*
	* @param center The new center position of the circle as a Vec2
	****************************************************************/
	void setCenter(Vec2 center);

	/****************************************************************
	* @brief Set the radius of the bounding circle
	*
	* @param radius The new radius of the circle as a float
	****************************************************************/
	void setRadius(float radius);
};

// Check collision
/****************************************************************
* @brief Checks collision between a circle and a box
*
* This function determines if a dynamic collision occurs between
* a BoundingCircle and a BoundingBox, taking into account their
* velocities over time. If a collision is detected, collision callbacks
* are invoked.
*
* @param circle The BoundingCircle object
* @param box The BoundingBox object
* @param deltaTime The time step for calculating future positions
* @param cir_vel The velocity vector of the circle
* @param box_vel The velocity vector of the box
*
* @return True if a collision is detected, false otherwise
****************************************************************/
DUCKENGINE_API bool checkCollisionCB(BoundingCircle& circle, BoundingBox& box, float deltaTime, Vec2 cir_vel = Vec2{ 0,0 }, Vec2 box_vel = Vec2{ 0,0 });

/****************************************************************
* @brief Checks collision between a box and a circle
*
* This function checks for a dynamic collision between a
* BoundingBox and a BoundingCircle by computing their
* next positions based on their velocities.
*
* @param box The BoundingBox object
* @param circle The BoundingCircle object
* @param deltaTime The time step for calculating future positions
* @param box_vel The velocity vector of the box
* @param cir_vel The velocity vector of the circle
*
* @return True if a collision is detected, false otherwise
****************************************************************/
DUCKENGINE_API bool checkCollisionBC(BoundingBox& box, BoundingCircle& circle, float deltaTime, Vec2 box_vel = Vec2{ 0,0 }, Vec2 cir_vel = Vec2{ 0,0 });

/****************************************************************
* @brief Checks collision between two boxes
*
* This function uses the Separating Axis Theorem (SAT) to
* detect collisions between two BoundingBox objects based
* on their velocities and positions.
*
* @param box1 The first BoundingBox object
* @param box2 The second BoundingBox object
* @param deltaTime The time step for calculating future positions
* @param vel1 The velocity vector of the first box
* @param vel2 The velocity vector of the second box
*
* @return True if a collision is detected, false otherwise
****************************************************************/
DUCKENGINE_API bool checkCollisionBB(BoundingBox& box1, BoundingBox& box2, float deltaTime, Vec2 vel1 = Vec2(0.f, 0.f), Vec2 vel2 = Vec2(0.f, 0.f));

/****************************************************************
* @brief Checks collision between two circles
*
* This function checks both static and dynamic collisions between
* two BoundingCircle objects by calculating their relative velocity
* and distance. It uses the quadratic formula to solve for dynamic
* collisions.
*
* @param circle The first BoundingCircle object
* @param circle2 The second BoundingCircle object
* @param deltaTime The time step for calculating future positions
* @param vel1 The velocity vector of the first circle
* @param vel2 The velocity vector of the second circle
*
* @return True if a collision is detected, false otherwise
****************************************************************/
DUCKENGINE_API bool checkCollisionCC(BoundingCircle& circle1, BoundingCircle& circle2, float deltaTime, Vec2 vel1 = Vec2(0.f, 0.f), Vec2 vel2 = Vec2(0.f, 0.f));

/****************************************************************
* @brief Checks collision between a circle and a line segment
*
* This function checks whether a BoundingCircle collides with
* a line segment by finding the closest point on the line to the
* circle's next position and comparing it to the circle's radius.
*
* @param circle The `BoundingCircle` object
* @param nextPos The next position of the circle after applying velocity
* @param lineStart The start point of the line segment
* @param lineEnd The end point of the line segment
*
* @return True if a collision is detected, false otherwise
****************************************************************/
DUCKENGINE_API bool checkCollisionCL(BoundingCircle& circle,const Vec2& nextPos, Vec2 lineStart, Vec2 lineEnd);