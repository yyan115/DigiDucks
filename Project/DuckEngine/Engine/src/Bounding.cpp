/******************************************************************************/
/*!
\file		Bounding.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 26 2024
\brief  	This file includes the definition of the Vector2D structure

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
Vec2 BoundingCollider::getCenterPos() const {
	return centerPos;
}

// Setters
void BoundingCollider::setCenterPos(const Vec2& pos) {
	centerPos = pos;
}

void BoundingCollider::setCenterPos(float x, float y) {
	centerPos = Vec2(x, y);
}


namespace {
    // Rotate along the z-axis
    Vec2 rotateVector(const Vec2& vec, float angle) {
        float s = sin(angle);
        float c = cos(angle);

        return Vec2(vec.x * c - vec.y * s, vec.x * s + vec.y * c);
    }
}

///// Box Collider /////
// Constructor
BoundingBox::BoundingBox(const Vec2& _center, const Vec2& _size, float _rotation):BoundingCollider() {
    setCenterPos(_center);
    size = _size;
    rotation = _rotation;

    // Get 4 corners of box using size and rotation
    topR = rotateVector(Vec2(_center.x + size.x, _center.y + size.y), rotation);
    topL = rotateVector(Vec2(_center.x - size.x, _center.y + size.y), rotation);
    btmR = rotateVector(Vec2(_center.x + size.x, _center.y - size.y), rotation);
    btmL = rotateVector(Vec2(_center.x - size.x, _center.y - size.y), rotation);
}

BoundingBox::BoundingBox(float _x, float _y, float sizeX, float sizeY, float _rotation) :BoundingCollider() {
    setCenterPos(_x, _y);
    size.x = sizeX;
    size.y = sizeY;
    rotation = _rotation;

    // Get 4 corners of box using size and rotation
    topR = rotateVector(Vec2(getCenterPos().x + size.x, getCenterPos().y + size.y), rotation);
    topL = rotateVector(Vec2(getCenterPos().x - size.x, getCenterPos().y + size.y), rotation);
    btmR = rotateVector(Vec2(getCenterPos().x + size.x, getCenterPos().y - size.y), rotation);
    btmL = rotateVector(Vec2(getCenterPos().x - size.x, getCenterPos().y - size.y), rotation);
}

// Setters
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

void BoundingBox::setSize(Vec2 _size) {
	size = _size;

    topR = rotateVector(Vec2(getCenterPos().x + size.x, getCenterPos().y + size.y), rotation);
    topL = rotateVector(Vec2(getCenterPos().x - size.x, getCenterPos().y + size.y), rotation);
    btmR = rotateVector(Vec2(getCenterPos().x + size.x, getCenterPos().y - size.y), rotation);
    btmL = rotateVector(Vec2(getCenterPos().x - size.x, getCenterPos().y - size.y), rotation);
}

void BoundingBox::rotate(float angle) {
	rotation += angle;
    // Keep angle within 360 degrees
    if (angle > 360.f)
        angle -= 360.f;

	topR = rotateVector(Vec2(getCenterPos().x + size.x, getCenterPos().y + size.y), rotation);
	topL = rotateVector(Vec2(getCenterPos().x - size.x, getCenterPos().y + size.y), rotation);
	btmR = rotateVector(Vec2(getCenterPos().x + size.x, getCenterPos().y - size.y), rotation);
	btmL = rotateVector(Vec2(getCenterPos().x - size.x, getCenterPos().y - size.y), rotation);
}

//// Circle Collider ////

// Getters
Vec2 BoundingCircle::getCenter() const {
	return getCenterPos();
}

float BoundingCircle::getRadius() const {
	return radius;
}

// Setters
void BoundingCircle::setCenter(Vec2 center) {
	setCenterPos(center);
}

void BoundingCircle::setRadius(float _radius) {
	radius = _radius;
}


// Collision Detection
namespace {

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
    void projectOnAxis(const Vec2& point,const  Vec2& axis, float& min, float& max ) {
        // Dot product of point and axis
        float projection = Vec2Dot(axis, point);

        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
    }
}


// Circle - Box
bool checkCollisionCB(BoundingCircle& circle, BoundingBox& box, float deltaTime, Vec2 cir_vel, Vec2 box_vel) {

    // Calculate next position of box
    Vec2 topRight = box.getTopR() + box_vel * deltaTime;
    Vec2 btmRight = box.getBtmR() + box_vel * deltaTime;
    Vec2 topLeft = box.getTopL() + box_vel * deltaTime;
    Vec2 btmLeft = box.getBtmL() + box_vel * deltaTime;

    // Calculate next position of circle
    Vec2 nextPos = circle.getCenter() + cir_vel * deltaTime;

    // Dynamic collision check
    if (checkCollisionCL(circle, nextPos, btmLeft, btmRight) ||
        checkCollisionCL(circle, nextPos, btmRight, topRight) ||
        checkCollisionCL(circle, nextPos, topRight, topLeft) ||
        checkCollisionCL(circle, nextPos, topLeft, btmLeft)) {

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
bool checkCollisionBC(BoundingBox& box, BoundingCircle& circle, float deltaTime, Vec2 box_vel, Vec2 cir_vel) {

    // Calculate next position of box
    Vec2 topRight = box.getTopR() + box_vel * deltaTime;
    Vec2 btmRight = box.getBtmR() + box_vel * deltaTime;
    Vec2 topLeft = box.getTopL() + box_vel * deltaTime;
    Vec2 btmLeft = box.getBtmL() + box_vel * deltaTime;

    // Calculate next position of circle
    Vec2 nextPos = circle.getCenter() + cir_vel * deltaTime;

    // Dynamic collision check
    if (checkCollisionCL(circle, nextPos, btmLeft, btmRight) ||
		checkCollisionCL(circle, nextPos, btmRight, topRight) ||
		checkCollisionCL(circle, nextPos, topRight, topLeft) ||
		checkCollisionCL(circle, nextPos, topLeft, btmLeft)) {
		return true;
	}

    return false;  // No collision
}

// Box - Box
bool checkCollisionBB(BoundingBox& box1, BoundingBox& box2, float deltaTime, Vec2 vel1, Vec2 vel2) {
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
bool checkCollisionCC(BoundingCircle& circle,BoundingCircle& circle2, float deltaTime, Vec2 vel1, Vec2 vel2) {
    //// Calculate Next Position
    //Vec2 nextPos1 = circle.getCenter() + vel1 * deltaTime;
    //Vec2 nextPos2 = circle2.getCenter() + vel2 * deltaTime;

    //// Get the distance between the two circles
    //Vec2 centerDiff = nextPos1 - nextPos2;
    //float combineRadii = circle.getRadius() + circle2.getRadius();

    //if(centerDiff.lengthSquared() <= combineRadii * combineRadii) {
    //    return true;  // Collision detected
    //}


    // Calculate relative velocity
	Vec2 relVel = vel1 - vel2;

	// Calculate distance between the two circles
    Vec2 centerDiff = circle.getCenter() - circle2.getCenter();

    float combineRadii = circle.getRadius() + circle2.getRadius();

    // Check static collisiohn
    if (centerDiff.lengthSquared() <= combineRadii * combineRadii) {
        return true;
    }
    else {  // Check dynamic Collision

        float a = relVel.lengthSquared(); // Coefficient of t^2
        float b = 2 * Vec2Dot(centerDiff, relVel); // Coefficient of t
        float c = centerDiff.lengthSquared() - (combineRadii * combineRadii); // Constant term

        // Quadratic formula : b^2 - 4ac
        float discriminant = b * b - 4 * a * c;

        if ((discriminant < 0) || (a == 0)) {
			return false;  // No collision
		}

        // Find 2 possible collision
		float t0 = (-b - sqrt(discriminant)) / (2 * a);
        float t1 = (-b + sqrt(discriminant)) / (2 * a);

        float t = t0 < t1 ? t0 : t1;

        if (t >= 0.f && t <= deltaTime) 
        {
            if (circle.onCollisionCallback) 
            {
                circle.onCollisionCallback(nullptr);  
            }
            if (circle2.onCollisionCallback) 
            {
                circle2.onCollisionCallback(nullptr);
            }
            return true;  // Collision detected
        }
    }

	return false;
}

// Cirlce - Line
bool checkCollisionCL(BoundingCircle& circle,const Vec2& nextPos, Vec2 lineStart, Vec2 lineEnd) {

    // Check closest Point to line from Next Position
    Vec2 closestPt = closestPointOnLineSegment(nextPos, lineStart, lineEnd);

    if ((closestPt - nextPos).lengthSquared() <= circle.getRadius() * circle.getRadius()) {
        return true;
    }

    return false; // No collision
}