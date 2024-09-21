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
    bool CheckMovingCircleToLineEdge(bool withinBothLines, BoundingCircle& circle, Vec2& nextPos, Vec2& lineStr, Vec2& lineEnd);

    // Circle - Line
    bool checkCircleLine(BoundingCircle& circle, Vec2& nextPos, Vec2 lineStr, Vec2 lineEnd, Vec2 relVel) {
        // Circle Velocity
        Vec2 velocity = (nextPos - circle.getCenterPos());
        // Velocity's Normal
        Vec2 v_N(velocity.y, -velocity.x);

        // Line Segment
        Vec2 lineNml(lineEnd.y - lineStr.y, -(lineEnd.x - lineStr.x));
        Vec2 n_lineNml = lineNml.normalized();

        // Get Normal.Bs
        float circle_dot_Normal = Vec2Dot(circle.getCenterPos(), n_lineNml);
        // Get Normal.P0
        float p0_dot_Normal = Vec2Dot(lineStr, n_lineNml);

        float distance = circle_dot_Normal - p0_dot_Normal;

        if (distance <= -circle.getRadius()) {

            // Make P0' and P1' -Radius
            Vec2 P0_Prime = (lineStr - (circle.getRadius() * n_lineNml));
            Vec2 P1_Prime = (lineEnd - (circle.getRadius() * n_lineNml));

            // Get Circle to P0', P1'
            Vec2 Circle_P0_Prime = (P0_Prime - circle.getCenterPos());
            Vec2 Circle_P1_Prime = (P1_Prime - circle.getCenterPos());

            //Magnitude of circle velocity vector
            float M_dot_p0_prime = Vec2Dot(v_N, Circle_P0_Prime);
            float M_dot_p1_prime = Vec2Dot(v_N, Circle_P1_Prime);

            if (M_dot_p0_prime * M_dot_p1_prime < 0) {
                // Check if Velocity is parellel to Line
                float v_dot_Normal = Vec2Dot(velocity, n_lineNml);
                if (v_dot_Normal != 0) {
                    // Get time of intercept
                    float interTime = (p0_dot_Normal - circle_dot_Normal - circle.getRadius()) / v_dot_Normal;

                    if (0 <= interTime && interTime <= 1) {
                        return true;    // Collision
                    }
                }
            }
            else {
                bool checkLineEdges = false;
                return CheckMovingCircleToLineEdge(checkLineEdges, circle, nextPos, lineStr, lineEnd);
            }

        }
        else if (distance >= circle.getRadius()) {

            // Make P0' and P1' +Radius
            Vec2 P0_Prime = (lineStr - (circle.getRadius() * n_lineNml));
            Vec2 P1_Prime = (lineEnd - (circle.getRadius() * n_lineNml));

            // Get Circle to p0' p1'
            Vec2 Circle_P0_Prime = (P0_Prime - circle.getCenterPos());
            Vec2 Circle_P1_Prime = (P1_Prime - circle.getCenterPos());

            //Magnitude of circle velocity vector
            float M_dot_p0_prime = Vec2Dot(v_N, Circle_P0_Prime);
            float M_dot_p1_prime = Vec2Dot(v_N, Circle_P1_Prime);

            if (M_dot_p0_prime * M_dot_p1_prime < 0) {
                // Check if Velocity is parellel to Line
                float v_dot_Normal = Vec2Dot(velocity, n_lineNml);
                if (v_dot_Normal != 0) {
                    // Get time of intercept
                    float interTime = (p0_dot_Normal - circle_dot_Normal + circle.getRadius()) / v_dot_Normal;

                    if (0 <= interTime && interTime <= 1) {
                        return true;	// Collision
                    }
                }
            }
            else {
                bool checkLineEdges = false;
                return CheckMovingCircleToLineEdge(checkLineEdges, circle, nextPos, lineStr, lineEnd);
            }
        }
        else {
            bool checkLineEdges = false;
            return CheckMovingCircleToLineEdge(checkLineEdges, circle, nextPos, lineStr, lineEnd);
        }

        return false;   // No Collision
    }

    // Circle - Line Edge
    bool CheckMovingCircleToLineEdge(bool withinBothLines, BoundingCircle& circle, Vec2& nextPos, Vec2& lineStr, Vec2& lineEnd) {
        // Circle Velocity
        Vec2 velocity = (nextPos - circle.getCenterPos());
        // Velocity's Normal
        Vec2 v_N(velocity.y, -velocity.x);

        // Normalize Velocity
        Vec2 N_velocity = velocity.normalized();

        // Magnitude of Velocity vector
        Vec2 M = v_N.normalized();

        Vec2 p0_p1(lineEnd - lineStr);
        Vec2 circle_p0(lineStr - circle.getCenterPos());
        Vec2 circle_p1(lineEnd - circle.getCenterPos());

        if (withinBothLines) {
            // Check if circle is closer to p0 or p1
            float dist_p0 = Vec2Dot(circle_p0, p0_p1);
            float dist_p1 = Vec2Dot(circle_p1, p0_p1);

            // Closer to P0
            if (dist_p0 > 0) {
                // Magnitude from P0
                float mag = Vec2Dot(circle_p0, N_velocity);
                if (mag > 0) {
                    // Get Distance from P0
                    float distance_0 = Vec2Dot(circle_p0, M);

                    // If distanec is more than circle radius, No collision.
                    if (abs(distance_0) > circle.getRadius()) {
                        return false;
                    }

                    float leng = sqrt(circle.getRadius() * circle.getRadius() - distance_0 * distance_0);

                    // Time of intercept.
                    float interTime = (mag - leng) / velocity.length();

                    if (interTime <= 1) {
                        return true;	// Got Collision
                    }
                }
            }
            // Closer to P1
            else if (dist_p1 < 0) {
                // Magnitude from P1
                float mag = Vec2Dot(circle_p1, N_velocity);
                if (mag > 0) {
                    // Get Distance from P0
                    float distance_1 = Vec2Dot(circle_p1, M);

                    // If distanec is more than circle radius, No collision.
                    if (abs(distance_1) > circle.getRadius()) {
                        return false;
                    }

                    float leng = sqrt(circle.getRadius() * circle.getRadius() - distance_1 * distance_1);

                    // Time of intercept.
                    float interTime = (mag - leng) / velocity.length();

                    if (interTime <= 1) {
                        return true;	// Got Collision
                    }
                }
            }
        }
        else {
            // Bool to check if on P0 or P1 side
            bool P0Side = false;	// False = P1 side, Ture = P0 side.

            // Distance between P0 and P1
            float dist0 = Vec2Dot(circle_p0, M);
            float dist1 = Vec2Dot(circle_p1, M);

            // Get copy of Absolute value of distance
            float abs_dist0 = abs(dist0);
            float abs_dist1 = abs(dist1);

            // If distance from both Points is more than radius, no collision.
            if ((abs_dist0 > circle.getRadius()) && (abs_dist1 > circle.getRadius())) {
                return 0;
            }
            // If Absolute distance is both less or equal to radius, find which point circle is closer to.
            else if ((abs_dist0 <= circle.getRadius()) && (abs_dist1 <= circle.getRadius())) {

                // Magnitude from both Points
                float m0 = Vec2Dot(circle_p0, velocity);
                float m1 = Vec2Dot(circle_p1, velocity);

                // Copy of Absolute value.
                float abs_m0 = abs(m0);
                float abs_m1 = abs(m1);

                // Find which Point is closer
                if (abs_m0 < abs_m1) {
                    // Closer to P0
                    P0Side = true;
                }
                else {
                    // Closer to P1
                    P0Side = false;
                }
            }
            else if (abs_dist0 <= circle.getRadius()) {
                P0Side = true;
            }
            else { // abs_dist1 <= Radius.
                P0Side = false;
            }


            if (P0Side) { // Closer to P0
                // Magnitude from P0
                float mag = Vec2Dot(circle_p0, N_velocity);
                if (mag < 0) {
                    return false;
                }
                else {
                    float leng = sqrt(circle.getRadius() * circle.getRadius() - dist0 * dist0);

                    // Time of intercept.
                    float interTime = (mag - leng) / velocity.length();

                    if (interTime <= 1) {
                        return true;	// Got Collision
                    }
                }
            }
            else { // Closer to P1
                // Magnitude from P1
                float mag = Vec2Dot(circle_p1, N_velocity);
                if (mag < 0) {
                    return false;
                }
                else {
                    float leng = sqrt(circle.getRadius() * circle.getRadius() - dist1 * dist1);

                    // Time of intercept.
                    float interTime = (mag - leng) / velocity.length();

                    if (interTime <= 1) {
                        return true;	// Got Collision
                    }
                }
            }
        }
        return false;   //no collision
    }

}


// Circle - Box
bool checkCollision(BoundingCircle& circle, Vec2& nextPos ,BoundingBox& box, float deltaTime, Vec2 circle_velo, Vec2 box_velo) {
    // Calculate relative velocity
    Vec2 relVel = circle_velo - box_velo;

    Vec2 topLeft(box.getMin().x, box.getMax().y);
    Vec2 topRight = box.getMax();
    Vec2 btmLeft = box.getMin();
    Vec2 btmRight(box.getMax().x, box.getMin().y);

    float dist = circle.getRadius() + std::max(box.getSize().x, box.getSize().y);

    // Distance between both center
    Vec2 centerDiff = box.getCenterPos() - circle.getCenterPos();

    if (centerDiff.lengthSquared() > dist * dist) {
        return false;
    }

    // Check collision with each line segment
    // Btm Line
    if (checkCircleLine(circle, nextPos, btmLeft, btmRight, relVel) &&
        checkCircleLine(circle, nextPos, btmRight, topRight, relVel) &&
        checkCircleLine(circle, nextPos, topRight, topLeft, relVel) &&
        checkCircleLine(circle, nextPos, topLeft, btmLeft, relVel)) {
        return true;
    }

    return false;
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