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

// Setters
void BoundingBox::setCenter(Vec2 center) {
	setCenterPos(center);
}

void BoundingBox::setSize(Vec2 _size) {
	size = _size;
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
// Circle - Box
bool checkCollision(BoundingCircle& circle, Vec2&& vel1, BoundingBox& box, Vec2&& vel2) {

	return false;
}

// Box - Box
bool checkCollision(BoundingBox& box1, Vec2&& vel1, BoundingBox& box2, Vec2&& vel2) {

	return false;
}

// Circle - Circle
bool checkCollision(BoundingCircle& circle, Vec2&& vel1, BoundingCircle& circle2, Vec2&& vel2) {

	return false;
}