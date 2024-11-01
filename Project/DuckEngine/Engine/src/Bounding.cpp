/******************************************************************************/
/*!
\file		Bounding.cpp
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

#include "Bounding.h"
#include <math.h>
//#include <algorithm>
//#include <iostream>
//#include <vector>

// Getters
/****************************************************************
* @brief Get the center position of the collider
*
* @return The center position of the collider
* ***************************************************************/
Vec2 BoundingCollider::getCenterPos() const {
	return centerPos;
}

// Setters
/****************************************************************
* @brief Set the center position of the collider
*
* @param pos - The center position of the collider
* ***************************************************************/
void BoundingCollider::setCenterPos(const Vec2& pos) {
	centerPos = pos;
}

/****************************************************************
* @brief Set the center position of the collider
*
* @param x - The x position of the collider
*
* @param y - The y position of the collider
* ***************************************************************/
void BoundingCollider::setCenterPos(float x, float y) {
	centerPos = Vec2(x, y);
}


namespace {
    // Rotate along the z-axis
    /****************************************************************
    * @brief Rotates a 2D vector by a given angle
    *
    * This function rotates a 2D vector (`vec`) counterclockwise by a
    * specified angle (`angle`) using the standard 2D rotation matrix.
    * The angle is in Degrees.
    *
    * @param vec The vector to be rotated as a `Vec2`
    * @param angle The angle in radians to rotate the vector
    *
    * @return A new `Vec2` representing the rotated vector
    ****************************************************************/
    Vec2 rotateVector(const Vec2& vec, float angle) {
        float radian = static_cast<float>(angle * M_PI / 180.f);
        float s = sin(radian);
        float c = cos(radian);

        return Vec2(vec.x * c - vec.y * s, vec.x * s + vec.y * c);
    }
}

///// Box Collider /////
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
BoundingBox::BoundingBox(const Vec2& _center, const Vec2& _size, float _rotation):BoundingCollider() {
    setCenterPos(_center);
    size = _size;
    rotation = _rotation;

    // Get 4 corners of box using size and rotation
    topR = _center + rotateVector(Vec2(size.x, size.y), rotation);
    topL = _center + rotateVector(Vec2(-size.x, size.y), rotation);
    btmR = _center + rotateVector(Vec2(size.x, -size.y), rotation);
    btmL = _center + rotateVector(Vec2(-size.x, -size.y), rotation);
}

/****************************************************************
* @brief Constructor for BoundingBox with explicit coordinates for center and size
*
* @param _x The x-coordinate of the center of the bounding box
* @param _y The y-coordinate of the center of the bounding box
* @param sizeX The width of the bounding box
* @param sizeY The height of the bounding box
* @param _rotation The rotation of the bounding box in degrees (default is 0)
****************************************************************/
BoundingBox::BoundingBox(float _x, float _y, float sizeX, float sizeY, float _rotation) :BoundingCollider() {
    setCenterPos(_x, _y);
    size.x = sizeX;
    size.y = sizeY;
    rotation = _rotation;

    // Get 4 corners of box using size and rotation
    topR = getCenter() + rotateVector(Vec2(size.x, size.y), rotation);
    topL = getCenter() + rotateVector(Vec2(-size.x, size.y), rotation);
    btmR = getCenter() + rotateVector(Vec2(size.x, -size.y), rotation);
    btmL = getCenter() + rotateVector(Vec2(-size.x, -size.y), rotation);
}

// Setters
/****************************************************************
* @brief Set the center position of the bounding box
*
* @param pos The new center position of the bounding box as a Vec2
****************************************************************/
void BoundingBox::setCenter(Vec2 center) {
    Vec2 prevCenter = getCenterPos();
	setCenterPos(center);

    // get the difference between the new center and the previous center
    Vec2 diff = center - prevCenter;

    // Update the 4 corners of the box
    topR += diff;
    topL += diff;
    btmR += diff;
    btmL += diff;
}

/****************************************************************
* @brief Set the size of the bounding box
*
* @param _size The new size of the bounding box as a Vec2
****************************************************************/
void BoundingBox::setSize(Vec2 _size) {
	size = _size;

    topR = getCenter() + rotateVector(Vec2(size.x, size.y), rotation);
    topL = getCenter() + rotateVector(Vec2(-size.x, size.y), rotation);
    btmR = getCenter() + rotateVector(Vec2(size.x, -size.y), rotation);
    btmL = getCenter() + rotateVector(Vec2(-size.x, -size.y), rotation);
}

void BoundingBox::setRotation(float angle) {
    rotation = angle;
    // Keep angle within 360 degrees
    if (angle > 360.f)
        angle -= 360.f;

    topR = getCenter() + rotateVector(Vec2(size.x, size.y), rotation);
    topL = getCenter() + rotateVector(Vec2(-size.x, size.y), rotation);
    btmR = getCenter() + rotateVector(Vec2(size.x, -size.y), rotation);
    btmL = getCenter() + rotateVector(Vec2(-size.x, -size.y), rotation);
}

/****************************************************************
* @brief Rotate the bounding box by a given angle
*
* @param angle The rotation angle in degrees
****************************************************************/
void BoundingBox::rotate(float angle) {
	rotation += angle;
    // Keep angle within 360 degrees
    if (angle > 360.f)
        angle -= 360.f;

    topR = getCenter() + rotateVector(Vec2(size.x, size.y), rotation);
    topL = getCenter() + rotateVector(Vec2(-size.x, size.y), rotation);
    btmR = getCenter() + rotateVector(Vec2(size.x, -size.y), rotation);
    btmL = getCenter() + rotateVector(Vec2(-size.x, -size.y), rotation);
}

//// Circle Collider ////

// Getters
/****************************************************************
* @brief Get the center position of the bounding circle
*
* @return The center of the circle as a Vec2
****************************************************************/
Vec2 BoundingCircle::getCenter() const {
	return getCenterPos();
}

/****************************************************************
* @brief Get the radius of the bounding circle
*
* @return The radius of the circle as a float
****************************************************************/
float BoundingCircle::getRadius() const {
	return radius;
}

// Setters
/****************************************************************
* @brief Set the center position of the bounding circle
*
* @param center The new center position of the circle as a Vec2
****************************************************************/
void BoundingCircle::setCenter(Vec2 center) {
	setCenterPos(center);
}

/****************************************************************
* @brief Set the radius of the bounding circle
*
* @param radius The new radius of the circle as a float
****************************************************************/
void BoundingCircle::setRadius(float _radius) {
	radius = _radius;
}


// Collision Detection
namespace {
    /****************************************************************
    * @brief Finds the closest point on a line segment to a given point
    *
    * This function computes the closest point on a line segment defined by
    * `lineStart` and `lineEnd` to the specified point (`point`). It uses
    * vector projection to find the closest point and ensures that the
    * result is clamped between the endpoints of the segment.
    *
    * @param point The point to find the closest point to, as a `Vec2`
    * @param lineStart The start point of the line segment, as a `Vec2`
    * @param lineEnd The end point of the line segment, as a `Vec2`
    *
    * @return A `Vec2` representing the closest point on the line segment
    ****************************************************************/
    Vec2 closestPointOnLineSegment(const Vec2& point, const Vec2& lineStart, const Vec2& lineEnd) {
        Vec2 line = lineEnd - lineStart;
        float lineLengthSquared = line.lengthSquared();

        // Project point onto the line (parameterized t value)
        float t = Vec2Dot(point - lineStart, line) / lineLengthSquared;

        // Clamp t between 0 and 1 to make sure the closest point lies on the line segment
        t = std::max(0.0f, std::min(1.0f, t));

        // Return the closest point
        return lerp(lineStart, lineEnd, t);
    }

    // Project point onto axis
    /****************************************************************
    * @brief Projects a point onto a given axis and updates the min
    *        and max projection values
    *
    * This function calculates the dot product of the given point (`point`)
    * and the specified axis (`axis`). It then updates the `min` and `max`
    * values based on the projection result.
    *
    * @param point The point to be projected, as a `Vec2`
    * @param axis The axis onto which the point is projected, as a `Vec2`
    * @param min A reference to the minimum projection value, updated if needed
    * @param max A reference to the maximum projection value, updated if needed
    ****************************************************************/
    void projectOnAxis(const Vec2& point,const  Vec2& axis, float& min, float& max ) {
        // Dot product of point and axis
        float projection = Vec2Dot(axis, point);

        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
        return;
    }
}


// Circle - Box
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
bool checkCollisionCB(const BoundingCircle& circle, const BoundingBox& box, Vec2& interceptPt, const float& deltaTime, const Vec2& cir_vel, const Vec2& box_vel) {

    // Calculate next position of box
    Vec2 topRight = box.getTopR() + box_vel * deltaTime;
    Vec2 btmRight = box.getBtmR() + box_vel * deltaTime;
    Vec2 topLeft = box.getTopL() + box_vel * deltaTime;
    Vec2 btmLeft = box.getBtmL() + box_vel * deltaTime;

    // Calculate next position of circle
    Vec2 nextPos = circle.getCenter() + cir_vel * deltaTime;

    // Dynamic collision check    
    if (checkCollisionCL(circle, nextPos, btmLeft, btmRight, interceptPt) ||
        checkCollisionCL(circle, nextPos, btmRight, topRight, interceptPt) ||
        checkCollisionCL(circle, nextPos, topRight, topLeft, interceptPt) ||
        checkCollisionCL(circle, nextPos, topLeft, btmLeft, interceptPt)) {

        // Pass the colliding entities to the callbacks
        if (circle.onCollisionCallback)
        {
            circle.onCollisionCallback(nullptr);
        }
        if (box.onCollisionCallback)
        {
            box.onCollisionCallback(nullptr);
        }

        return true;
    }

    return false;  // No collision
}

// Box - Circle
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
bool checkCollisionBC(const BoundingBox& box,const BoundingCircle& circle, Vec2& interceptPt, const float& deltaTime, const Vec2& box_vel, const Vec2& cir_vel) {

    // Calculate next position of box
    Vec2 topRight = box.getTopR() + box_vel * deltaTime;
    Vec2 btmRight = box.getBtmR() + box_vel * deltaTime;
    Vec2 topLeft = box.getTopL() + box_vel * deltaTime;
    Vec2 btmLeft = box.getBtmL() + box_vel * deltaTime;

    // Calculate next position of circle
    Vec2 nextPos = circle.getCenter() + cir_vel * deltaTime;

    // Dynamic collision check
    if (checkCollisionCL(circle, nextPos, btmLeft, btmRight, interceptPt) ||
		checkCollisionCL(circle, nextPos, btmRight, topRight, interceptPt) ||
		checkCollisionCL(circle, nextPos, topRight, topLeft, interceptPt) ||
		checkCollisionCL(circle, nextPos, topLeft, btmLeft, interceptPt)) {

        // Pass the colliding entities to the callbacks
        if (circle.onCollisionCallback)
        {
            circle.onCollisionCallback(nullptr);
        }
        if (box.onCollisionCallback)
        {
            box.onCollisionCallback(nullptr);
        }

		return true;
	}

    return false;  // No collision
}

// Box - Box
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
bool checkCollisionBB(const BoundingBox& box1, const BoundingBox& box2, const float& deltaTime, const Vec2& vel1, const Vec2& vel2) {
    // Calculate Next Position
    Vec2 nextTL1 = box1.getTopL() + vel1 * deltaTime;
    Vec2 nextTR1 = box1.getTopR() + vel1 * deltaTime;
    Vec2 nextBR1 = box1.getBtmR() + vel1 * deltaTime;
    Vec2 nextBL1 = box1.getBtmL() + vel1 * deltaTime;

    Vec2 nextTL2 = box2.getTopL() + vel2 * deltaTime;
    Vec2 nextTR2 = box2.getTopR() + vel2 * deltaTime;
    Vec2 nextBR2 = box2.getBtmR() + vel2 * deltaTime;
    Vec2 nextBL2 = box2.getBtmL() + vel2 * deltaTime;

    std::vector<Vec2> axes;

    // Box 1
    // Top Axis - Top Right to Top Left
    axes.push_back((nextTL1 - nextTR1).normalized());
    // Right Axis - Btm Right to Top Right
    axes.push_back((nextTR1 - nextBR1).normalized());

    // Calculate Next Position
    if (box1.rotation != box2.rotation) {   // If angle not equal, calculate box2 axes
        // Top Axis - Top Right to Top Left
        axes.push_back((nextTL2 - nextTR2).normalized());
        // Right Axis - Btm Right to Top Right
        axes.push_back((nextTR2 - nextBR2).normalized());
    }

    // Check for overlap
    for (size_t i{}; i < axes.size(); ++i) {

        float min1 = std::numeric_limits<float>::max();
        float max1 = -std::numeric_limits<float>::max();
        float min2 = std::numeric_limits<float>::max();
        float max2 = -std::numeric_limits<float>::max();

        // Project box 1
        projectOnAxis(nextTL1, axes[i], min1, max1);
        projectOnAxis(nextTR1, axes[i], min1, max1);
        projectOnAxis(nextBR1, axes[i], min1, max1);
        projectOnAxis(nextBL1, axes[i], min1, max1);

        // Project box 2
        projectOnAxis(nextTL2, axes[i], min2, max2);
        projectOnAxis(nextTR2, axes[i], min2, max2);
        projectOnAxis(nextBR2, axes[i], min2, max2);
        projectOnAxis(nextBL2, axes[i], min2, max2);


        if (max1 < min2 || max2 < min1) {
            return false;  // No collision
        }
    }

    // No separating axis found, collision detected
    if (box1.onCollisionCallback)
    {
        box1.onCollisionCallback(nullptr);
    }
    if (box2.onCollisionCallback)
    {
        box2.onCollisionCallback(nullptr);
    }

    return true;
}

// Circle - Circle
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
bool checkCollisionCC(const BoundingCircle& circle, const BoundingCircle& circle2, const float& deltaTime, const Vec2& vel1, const Vec2& vel2) {
    //// Calculate Next Position
    Vec2 nextPos1 = circle.getCenter() + vel1 * deltaTime;
    Vec2 nextPos2 = circle2.getCenter() + vel2 * deltaTime;

    // Get the distance between the two circles
    Vec2 centerDiff = nextPos1 - nextPos2;
    float combineRadii = circle.getRadius() + circle2.getRadius();

    if(centerDiff.lengthSquared() <= combineRadii * combineRadii) {
        return true;  // Collision detected
    }


    //// Calculate relative velocity
    //Vec2 relVel = vel1 - vel2;

    //// Calculate distance between the two circles
    //Vec2 centerDiff = circle.getCenter() - circle2.getCenter();

    //float combineRadii = circle.getRadius() + circle2.getRadius();

    //// Check static collisiohn
    //if (centerDiff.lengthSquared() <= combineRadii * combineRadii) {
    //    return true;
    //}
    //else {  // Check dynamic Collision

    //    float a = relVel.lengthSquared(); // Coefficient of t^2
    //    float b = 2 * Vec2Dot(centerDiff, relVel); // Coefficient of t
    //    float c = centerDiff.lengthSquared() - (combineRadii * combineRadii); // Constant term

    //    // Quadratic formula : b^2 - 4ac
    //    float discriminant = b * b - 4 * a * c;

    //    if ((discriminant < 0) || (a == 0)) {
    //        return false;  // No collision
    //    }

    //    // Find 2 possible collision
    //    float t0 = (-b - sqrt(discriminant)) / (2 * a);
    //    float t1 = (-b + sqrt(discriminant)) / (2 * a);

    //    float t = t0 < t1 ? t0 : t1;

    //    if (t >= 0.f && t <= deltaTime)
    //    {
    //        if (circle.onCollisionCallback)
    //        {
    //            circle.onCollisionCallback(nullptr);
    //        }
    //        if (circle2.onCollisionCallback)
    //        {
    //            circle2.onCollisionCallback(nullptr);
    //        }
    //        return true;  // Collision detected
    //    }
    //}

    return false;
}

// Cirlce - Line
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
bool checkCollisionCL(const BoundingCircle& circle,const Vec2& nextPos, const Vec2& lineStart, const Vec2& lineEnd, Vec2& interceptPt) {

    // Check closest Point to line from Next Position
    Vec2 closestPt = closestPointOnLineSegment(nextPos, lineStart, lineEnd);

    if ((closestPt - nextPos).lengthSquared() <= circle.getRadius() * circle.getRadius()) {
        interceptPt = closestPt;
        return true;
    }

    return false; // No collision
}


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
void calculateNewVelocity(const Vec2& curPos, Vec2& curVel, const Vec2& interceptPt,const float& radius, const float& deltaTime) {
    // Get direction from current position to intersection point
    Vec2 dir = interceptPt - curPos;

    // Get the length of the direction vector
    float dist = dir.length();

    // Calculate distance from intercept with radius
    float distFmIntcpt = dist - radius;

    if (distFmIntcpt <= 0.0f) {
        curVel = Vec2(0.0f, 0.0f);
        return;
    }

    // Calculate the time taken to reach the intersection point
    float time = distFmIntcpt / (curVel.length() * deltaTime);

    // Calculate the new velocity
    curVel *= time;
}