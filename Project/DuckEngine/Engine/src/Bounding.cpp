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

Vec2& BoundingCollider::getCenterPos() {
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
        return lineStart + t * line;
    }
}


// Circle - Box
bool checkCollision(BoundingCircle& circle, Vec2& nextPos ,BoundingBox& box, float deltaTime, Vec2 circle_velo) {
    // Box corners
    Vec2 topLeft(box.getMin().x, box.getMax().y);
    Vec2 topRight = box.getMax();
    Vec2 btmLeft = box.getMin();
    Vec2 btmRight(box.getMax().x, box.getMin().y);

    // Closest points on each rectangle side to the circle's center
    Vec2 closestOnBottom = closestPointOnLineSegment(circle.getCenterPos(), btmLeft, btmRight);
    Vec2 closestOnRight = closestPointOnLineSegment(circle.getCenterPos(), btmRight, topRight);
    Vec2 closestOnTop = closestPointOnLineSegment(circle.getCenterPos(), topRight, topLeft);
    Vec2 closestOnLeft = closestPointOnLineSegment(circle.getCenterPos(), topLeft, btmLeft);

    // Check if any of the closest points is within the circle's radius
    if ((closestOnBottom - circle.getCenterPos()).length() <= circle.getRadius() ||
        (closestOnRight - circle.getCenterPos()).length() <= circle.getRadius() ||
        (closestOnTop - circle.getCenterPos()).length() <= circle.getRadius() ||
        (closestOnLeft - circle.getCenterPos()).length() <= circle.getRadius()) {
        return true;  // Collision detected
    }

    // Also check if the circle is completely within the box
    if (circle.getCenterPos().x >= box.getMin().x && circle.getCenterPos().x <= box.getMax().x &&
        circle.getCenterPos().y >= box.getMin().y && circle.getCenterPos().y <= box.getMax().y) {
        return true;  // Circle is inside the box
    }
    
    return false;  // No collision
}

// Box - Box
bool checkCollision(BoundingBox& box1, BoundingBox& box2, float deltaTime, Vec2 vel1, Vec2 vel2) {
    if (box1.getMax().x < box2.getMin().x || box1.getMax().y < box2.getMin().y ||
        box1.getMin().x > box2.getMax().x || box1.getMin().y > box2.getMax().y) {

        // No static collision, proceed with dynamic collision test

        // Calculate relative velocity
        Vec2 relVel = vel1 - vel2;

        // Initialize time variables
        float tFirst = 0.0f;
        float tLast = deltaTime;

        // Check collision along x-axis
        if (relVel.x != 0) {
            if (relVel.x < 0) {
                if (box1.getMin().x > box2.getMax().x) 
                    return false;
                if (box1.getMax().x < box2.getMin().x) 
                    tFirst = (box1.getMax().x - box2.getMin().x) / relVel.x;
                if (box1.getMin().x < box2.getMax().x) 
                    tLast = (box1.getMin().x - box2.getMax().x) / relVel.x;
            }
            else {
                if (box1.getMin().x > box2.getMax().x) 
                    tFirst = (box1.getMin().x - box2.getMax().x) / relVel.x;
                if (box1.getMax().x > box2.getMin().x) 
                    tLast = (box1.getMax().x - box2.getMin().x) / relVel.x;
                if (box1.getMax().x < box2.getMin().x) 
                    return false;
            }
        }
        else if (box1.getMax().x < box2.getMin().x || box1.getMin().x > box2.getMax().x) {
            return false;  // No collision along x-axis if velocities are parallel and outside bounds
        }

        // Check collision along y-axis
        if (relVel.y != 0) {
            if (relVel.y < 0) {
                if (box1.getMin().y > box2.getMax().y) 
                    return false;
                if (box1.getMax().y < box2.getMin().y) 
                    tFirst = (box1.getMax().y - box2.getMin().y) / relVel.y;
                if (box1.getMin().y < box2.getMax().y) 
                    tLast = (box1.getMin().y - box2.getMax().y) / relVel.y;
            }
            else {
                if (box1.getMin().y > box2.getMax().y) 
                    tFirst = (box1.getMin().y - box2.getMax().y) / relVel.y;
                if (box1.getMax().y > box2.getMin().y) 
                    tLast = (box1.getMax().y - box2.getMin().y) / relVel.y;
                if (box1.getMax().y < box2.getMin().y) 
                    return false;
            }
        }
        else if (box1.getMax().y < box2.getMin().y || box1.getMin().y > box2.getMax().y) {
            return false;  // No collision along y-axis if velocities are parallel and outside bounds
        }

        // If first collision time is greater than the last, no collision
        if (tFirst > tLast) 
            return false;

        return true;  // Collision detected during dynamic movement
    }

    return true;  // Collision detected for static case
}

// Circle - Circle
bool checkCollision(BoundingCircle& circle,BoundingCircle& circle2, float deltaTime, Vec2 vel1, Vec2 vel2) {
    // Calculate relative velocity
	Vec2 relVel = vel1 - vel2;

	// Calculate distance between the two circles
    Vec2 centerDiff = circle.getCenterPos() - circle2.getCenterPos();

    float combineRadii = circle.getRadius() + circle2.getRadius();

    // Check static collisiohn
    if (centerDiff.lengthSquared() <= combineRadii * combineRadii) {
        return true;
    }
    else {  // Check dynamic Collision

        float a = relVel.lengthSquared(); // Coefficient of t^2
        float b = -2 * Vec2Dot(centerDiff, relVel); // Coefficient of t
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