/******************************************************************************/
/*!
\file		Bounding.h
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Sep 26 2024
\brief  	This file contains functions for detecting various types of 
            collisions in a 2D environment, including circle-box, 
            box-box, circle-circle, and circle-line segment collisions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <functional>
#include "Vector2.h"
#include "Component.h"
#include "Entity.h"
#include "SpatialGridSystem.h"

class BoundingCollider {
private:
	Vec2 centerPos{};
	Vec2 offSet{};

public:
	bool isKinematic = true;

	// Constructor
	/****************************************************************
	* @brief Default constructor for BoundingCollider
	*
	* Initializes the center position of the collider to (0, 0).
	****************************************************************/
	DUCKENGINE_API BoundingCollider() : centerPos(0.f, 0.f), offSet(0.f,0.f) {}

	/****************************************************************
	* @brief Constructor with center position
	*
	* @param pos The initial center position of the collider as a Vec2
	****************************************************************/
	DUCKENGINE_API BoundingCollider(const Vec2& pos, const Vec2& offSet_ = Vec2{ 0.f, 0.f }) : centerPos(pos), offSet(offSet_) {}
	/****************************************************************
	* @brief Constructor with x and y coordinates
	*
	* @param x The x-coordinate of the center position
	* @param y The y-coordinate of the center position
	****************************************************************/
	DUCKENGINE_API BoundingCollider(float x, float y, const Vec2& offSet_ = Vec2{ 0.f, 0.f }) : centerPos(x, y), offSet(offSet_) {}

	using OnCollisionCallback = std::function<void(int)>;
	OnCollisionCallback onCollisionCallback;

	// Destructor
	/****************************************************************
	* @brief Virtual destructor for BoundingCollider
	*
	* Allows for proper cleanup when objects are deleted through a pointer
	* to the base class.
	****************************************************************/
	DUCKENGINE_API virtual ~BoundingCollider() = default;

	// Getters
	/****************************************************************
	* @brief Get the center position of the collider
	* 
	* @return The center position of the collider
	* ***************************************************************/
	DUCKENGINE_API Vec2 getCenterPos() const;

	/****************************************************************
	* @brief Get the offset of the collider
	* 
	* @return The offset of the collider
	* ***************************************************************/
	DUCKENGINE_API Vec2 getOffSet() const;

	// Setters
	/****************************************************************
	* @brief Set the center position of the collider
	* 
	* @param pos - The center position of the collider
	* ***************************************************************/
	DUCKENGINE_API void setCenterPos(const Vec2& pos);

	/****************************************************************
	* @brief Set the center position of the collider
	* 
	* @param x - The x position of the collider
	* 
	* @param y - The y position of the collider
	* ***************************************************************/
	DUCKENGINE_API void setCenterPos(float x, float y);

	/****************************************************************
	* @brief Set the offset of the collider
	*	
	* @param offSet_ - The offset of the collider
	* ***************************************************************/
	DUCKENGINE_API void setOffSet(const Vec2& offSet_);

	/****************************************************************
	* @brief Set the offset of the collider
	*	
	* @param x - The x offset of the collider
	* 
	* @param y - The y offset of the collider
	* ***************************************************************/
	DUCKENGINE_API void setOffSet(float x, float y);

	/****************************************************************
	* @brief Set the collision callback function
	*
	* @param callback The function to be called upon collision
	****************************************************************/
	DUCKENGINE_API void SetCollisionCallback(OnCollisionCallback callback)
	{
		onCollisionCallback = callback;
	}
};


class BoundingBox : public BoundingCollider, public Component {
private:
	Vec2 size{};
	Vec2 topR{};
	Vec2 topL{};
	Vec2 btmR{};
	Vec2 btmL{};
	Vec2 initialSize{0.5f, 0.5f};

	/****************************************************************
	* @brief Set the Corners of the box
	****************************************************************/
	void setCorners();

public:
	float rotation{};

	// No default constructor
	DUCKENGINE_API BoundingBox() :BoundingCollider() {}

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
	DUCKENGINE_API BoundingBox(const Vec2& _center, const Vec2& _size, float _rotation = 0.f);
	/****************************************************************
	* @brief Constructor for BoundingBox with explicit coordinates for center and size
	*
	* @param _x The x-coordinate of the center of the bounding box
	* @param _y The y-coordinate of the center of the bounding box
	* @param sizeX The width of the bounding box
	* @param sizeY The height of the bounding box
	* @param _rotation The rotation of the bounding box in degrees (default is 0)
	****************************************************************/
	DUCKENGINE_API BoundingBox(float _x, float _y, float sizeX, float sizeY, float _rotation = 0.f);

	// Copy Constructor
	/****************************************************************
	* @brief Copy constructor for BoundingBox
	*
	* Initializes a BoundingBox by copying the data from another bounding box.
	*
	* @param box The BoundingBox object to copy from
	****************************************************************/
	DUCKENGINE_API BoundingBox(BoundingBox& box) :
		BoundingCollider(box.getCenter()), size(box.size), topR(box.topR), topL(box.topL), btmR(box.btmR), btmL(box.btmL), rotation(box.rotation) {}
	/****************************************************************
	* @brief Copy constructor for BoundingBox (const version)
	*
	* Initializes a BoundingBox by copying the data from another constant bounding box.
	*
	* @param box The constant BoundingBox object to copy from
	****************************************************************/
	DUCKENGINE_API BoundingBox(const BoundingBox& box):
		BoundingCollider(box.getCenter()), size(box.size), topR(box.topR), topL(box.topL), btmR(box.btmR), btmL(box.btmL), rotation(box.rotation) {}

	// Destructor
	/****************************************************************
	* @brief Destructor for BoundingBox
	*
	* Performs necessary cleanup of the BoundingBox object.
	****************************************************************/
	DUCKENGINE_API ~BoundingBox() = default;

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
	DUCKENGINE_API Vec2 getCenter() const { return getCenterPos(); }

	DUCKENGINE_API Vec2 getInitialSize() const { return initialSize; }

	/****************************************************************
	* @brief Get the size of the bounding box
	*
	* @return The size of the box as a Vec2
	****************************************************************/
	DUCKENGINE_API Vec2 getSize() const { return size; }
	
	/****************************************************************
	* @brief Get the top right corner of the bounding box
	*
	* @return The top right corner of the box as a Vec2
	****************************************************************/
	DUCKENGINE_API Vec2 getTopR() const { return topR; }

	/****************************************************************
	* @brief Get the top left corner of the bounding box
	*
	* @return The top left corner of the box as a Vec2
	****************************************************************/
	DUCKENGINE_API Vec2 getTopL() const { return topL; }

	/****************************************************************
	* @brief Get the bottom right corner of the bounding box
	*
	* @return The bottom right corner of the box as a Vec2
	****************************************************************/
	DUCKENGINE_API Vec2 getBtmR() const { return btmR; }

	/****************************************************************
	* @brief Get the bottom left corner of the bounding box
	*
	* @return The bottom left corner of the box as a Vec2
	****************************************************************/
	DUCKENGINE_API Vec2 getBtmL() const { return btmL; }
	
	// Setters
	/****************************************************************
	* @brief Set the center position of the bounding box
	*
	* @param pos The new center position of the bounding box as a Vec2
	****************************************************************/
	DUCKENGINE_API void setCenter(Vec2 pos);

	/****************************************************************
	* @brief Set the size of the bounding box
	*
	* @param _size The new size of the bounding box as a Vec2
	****************************************************************/
	DUCKENGINE_API void setSize(Vec2 _size);

	DUCKENGINE_API void setRotation(float angle);

	/****************************************************************
	* @brief Rotate the bounding box by a given angle
	*
	* @param angle The rotation angle in degrees
	****************************************************************/
	DUCKENGINE_API void rotate(float angle);

	bool showDebugCollider = false;
};


class BoundingCircle : public BoundingCollider, public Component {
private:
	float radius{};
	float initialRadius = 0.5f;

public:
	/****************************************************************
	* @brief Default constructor for BoundingCircle
	*
	* Initializes a bounding circle with a default radius of 2.0f and a
	* default center position set to (0, 0).
	****************************************************************/
	DUCKENGINE_API BoundingCircle() :BoundingCollider(), radius(2.f) {}

	// Constructor
	/****************************************************************
	* @brief Constructor for BoundingCircle
	*
	* Initializes a bounding circle with a given center position and radius.
	*
	* @param center The center position of the bounding circle as a Vec2
	* @param _radius The radius of the bounding circle
	****************************************************************/
	DUCKENGINE_API BoundingCircle(const Vec2& center, float _radius) : BoundingCollider(center), radius(_radius) {};
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
	DUCKENGINE_API BoundingCircle(float x, float y, float _radius) : BoundingCollider(x, y), radius(_radius) {}

	// Copy Constructor
	/****************************************************************
	* @brief Copy constructor for BoundingCircle
	*
	* Initializes a BoundingCircle by copying the data from another BoundingCircle.
	*
	* @param circle The BoundingCircle object to copy from
	****************************************************************/
	DUCKENGINE_API BoundingCircle(BoundingCircle& circle) : BoundingCollider(circle.getCenterPos()), radius(circle.getRadius()) {}
	/****************************************************************
	* @brief Const copy constructor for BoundingCircle
	*
	* Initializes a BoundingCircle by copying the data from another constant BoundingCircle.
	*
	* @param circle The constant BoundingCircle object to copy from
	****************************************************************/
	DUCKENGINE_API BoundingCircle(const BoundingCircle& circle) : BoundingCollider(circle.getCenterPos()), radius(circle.getRadius()) {}

	// Destructor
	/****************************************************************
	* @brief Destructor for BoundingCircle
	*
	* Cleans up resources used by the BoundingCircle object.
	****************************************************************/
	DUCKENGINE_API ~BoundingCircle() = default;

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
	DUCKENGINE_API Vec2 getCenter() const;

	DUCKENGINE_API float getInitialRadius() const { return initialRadius; }

	/****************************************************************
	* @brief Get the radius of the bounding circle
	*
	* @return The radius of the circle as a float
	****************************************************************/
	DUCKENGINE_API float getRadius() const;

	// Setters
	/****************************************************************
	* @brief Set the center position of the bounding circle
	*
	* @param center The new center position of the circle as a Vec2
	****************************************************************/
	DUCKENGINE_API void setCenter(Vec2 center);

	/****************************************************************
	* @brief Set the radius of the bounding circle
	*
	* @param radius The new radius of the circle as a float
	****************************************************************/
	DUCKENGINE_API void setRadius(float radius);

	bool showDebugCollider = false;
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
bool checkCollisionCB(const BoundingCircle& circle, const BoundingBox& box, Vec2& interceptPt, const float& deltaTime, const Vec2& cir_vel = Vec2{ 0,0 }, const Vec2& box_vel = Vec2{ 0,0 });

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
bool checkCollisionBC(const BoundingBox& box, const BoundingCircle& circle, Vec2& interceptPt, const float& deltaTime, const Vec2& box_vel = Vec2{ 0,0 }, const Vec2& cir_vel = Vec2{ 0,0 });

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
bool checkCollisionBB(const BoundingBox& box1, const BoundingBox& box2, const float& deltaTime, const Vec2& vel1 = Vec2(0.f, 0.f), const Vec2& vel2 = Vec2(0.f, 0.f));

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
bool checkCollisionCC(const BoundingCircle& circle1, const BoundingCircle& circle2, const float& deltaTime, const Vec2& vel1 = Vec2(0.f, 0.f), const Vec2& vel2 = Vec2(0.f, 0.f));

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
bool checkCollisionCL(const BoundingCircle& circle, const Vec2& nextPos, const Vec2& lineStart, const Vec2& lineEnd, Vec2& interceptPt);


// Calculate and adjust the new velocity with intersection point
/****************************************************************
* @brief Calculate the new velocity of a moving object
* 
* This function calculates the new velocity of a moving object
* after a collision with a stationary object, using the intersection
* point of the two objects.
* 
* @param curPos The current position of the moving object
* @param curVel The current velocity of the moving object
* @param interceptPt The intersection point of the two objects
* @param deltaTime The time step for calculating the new velocity
* ***************************************************************/
void calculateNewVelocity(const Vec2& curPos, Vec2& curVel, const Vec2& interceptPt, const float& radius, const float& deltaTime);