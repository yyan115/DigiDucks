/******************************************************************************/
/*!
\file		Bounding.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 13 2024
\brief  	This file includes the definition of the Vector2D structure

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Bounding.h"
#include <algorithm>
#include <math.h>
#include <iostream>

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



///// Box Collider /////

// Getters
Vec2 BoundingBox::getCenter() const {
	return getCenterPos();
}

Vec2 BoundingBox::getSize() const {
	return size;
}

Vec2 BoundingBox::getMax() const {
	return max;
}

Vec2 BoundingBox::getMin() const {
	return min;
}

// Setters
void BoundingBox::setCenter(Vec2 center) {
	setCenterPos(center);
    max = getCenterPos() + size;
    min = getCenterPos() - size;
}

void BoundingBox::setSize(Vec2 _size) {
	size = _size;
    max = getCenterPos() + size;
    min = getCenterPos() - size;
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
    // Helper function to compute the closest point on a line segment to a point
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
}


// Circle - Box
bool checkCollisionCB(BoundingCircle& circle, Vec2& nextPos ,BoundingBox& box) {

    // Check if the circle is completely within the box
    if (circle.getCenter().x >= box.getMin().x && circle.getCenter().x <= box.getMax().x &&
        circle.getCenter().y >= box.getMin().y && circle.getCenter().y <= box.getMax().y) {
        return true;  // Circle is inside the box
    }


    // Box corners
    Vec2 topLeft(box.getMin().x, box.getMax().y);
    Vec2 topRight = box.getMax();
    Vec2 btmLeft = box.getMin();
    Vec2 btmRight(box.getMax().x, box.getMin().y);

    // Static collision check
    if (checkCollisionCL(circle, circle.getCenter(), btmLeft, btmRight) ||
        checkCollisionCL(circle, circle.getCenter(), btmRight, topRight) ||
        checkCollisionCL(circle, circle.getCenter(), topRight, topLeft) ||
        checkCollisionCL(circle, circle.getCenter(), topLeft, btmLeft)) {
        return true;
    }

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
    // Calculate next positions after applying velocity
    Vec2 nextPos1Min = box1.getMin() + vel1 * deltaTime;
    Vec2 nextPos1Max = box1.getMax() + vel1 * deltaTime;
    Vec2 nextPos2Min = box2.getMin() + vel2 * deltaTime;
    Vec2 nextPos2Max = box2.getMax() + vel2 * deltaTime;

    // Static collision check based on the next positions
    if (nextPos1Max.x < nextPos2Min.x || nextPos1Max.y < nextPos2Min.y ||
        nextPos1Min.x > nextPos2Max.x || nextPos1Min.y > nextPos2Max.y) {

        // No static collision, proceed with dynamic collision test
        Vec2 relVel = vel1 - vel2;
        float tFirst = 0.0f;
        float tLast = deltaTime;

        // Check collision along x-axis
        if (relVel.x != 0) {
            if (relVel.x < 0) {
                if (nextPos1Min.x > nextPos2Max.x) return false;
                tFirst = std::max(tFirst, (nextPos1Max.x - nextPos2Min.x) / relVel.x);
                tLast = std::min(tLast, (nextPos1Min.x - nextPos2Max.x) / relVel.x);
            }
            else {
                if (nextPos1Max.x < nextPos2Min.x) return false;
                tFirst = std::max(tFirst, (nextPos1Min.x - nextPos2Max.x) / relVel.x);
                tLast = std::min(tLast, (nextPos1Max.x - nextPos2Min.x) / relVel.x);
            }
        }
        else if (nextPos1Max.x < nextPos2Min.x || nextPos1Min.x > nextPos2Max.x) {
            return false;  // No collision along x-axis if velocities are parallel and outside bounds
        }

        // Check collision along y-axis
        if (relVel.y != 0) {
            if (relVel.y < 0) {
                if (nextPos1Min.y > nextPos2Max.y) return false;
                tFirst = std::max(tFirst, (nextPos1Max.y - nextPos2Min.y) / relVel.y);
                tLast = std::min(tLast, (nextPos1Min.y - nextPos2Max.y) / relVel.y);
            }
            else {
                if (nextPos1Max.y < nextPos2Min.y) return false;
                tFirst = std::max(tFirst, (nextPos1Min.y - nextPos2Max.y) / relVel.y);
                tLast = std::min(tLast, (nextPos1Max.y - nextPos2Min.y) / relVel.y);
            }
        }
        else if (nextPos1Max.y < nextPos2Min.y || nextPos1Min.y > nextPos2Max.y) {
            return false;  // No collision along y-axis if velocities are parallel and outside bounds
        }

        // If the first time of collision is greater than the last, no collision
        if (tFirst > tLast) return false;

        return true;  // Collision detected during dynamic movement
    }

    return true;  // Collision detected for static case (if it happens now)
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

        if (t >= 0.f && t <= deltaTime) {
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