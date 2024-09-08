/******************************************************************************/
/*!
\file		Vector2D.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 9 2024
\brief  	This file includes the definition of the Vector2D structure

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Vector2.h"
#include <math.h>

Vector2D& Vector2D::operator+=(const Vector2D& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vector2D& Vector2D::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2D& Vector2D::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

Vector2D& Vector2D::operator=(float val) {
	x = val;
	y = val;
	return *this;
}

Vector2D Vector2D::operator -() const {
	return Vector2D(-x, -y);
}

Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs) {
	return Vector2D(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs) {
	return Vector2D(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2D operator * (const Vector2D& lhs, float rhs) {
	return Vector2D(lhs.x * rhs, lhs.y * rhs);
}

Vector2D operator * (float lhs, const Vector2D& rhs) {
	return Vector2D(lhs * rhs.x, lhs * rhs.y);
}

Vector2D operator / (const Vector2D& lhs, float rhs) {
	return Vector2D(lhs.x / rhs, lhs.y / rhs);
}

// Private Functions
Vector2D Vector2D::normalized() const {
	// New vector to store the normalized vector
	Vector2D normalizedVec(*this);

	// Magnitude of the vector
	float magnitude = normalizedVec.length();

	// Normalize the vector
	if (magnitude != 0.f) {
		normalizedVec.x = x / magnitude;
		normalizedVec.y = y / magnitude;
	}
	else { // If the vector is a zero vector, return a zero vector
		normalizedVec.x = 0.f;
		normalizedVec.y = 0.f;
	}

	return normalizedVec;
}

float Vector2D::length() const {
	float length = sqrt(x * x + y * y);
	return length;
}

// Public Functions
float Vec2Dot(const Vector2D& lhs, const Vector2D& rhs) {
	float _x = lhs.x * rhs.x;
	float _y = lhs.y * rhs.y;
	return _x + _y;
}

float Vec2Cross(const Vector2D& lhs, const Vector2D& rhs) {
	float _x = lhs.x * rhs.y;
	float _y = lhs.y * rhs.x;
	return _x - _y;
}

float Vec2Dist(const Vector2D& lhs, const Vector2D& rhs) {
	Vector2D diff = lhs - rhs;
	return diff.length();
}

Vector2D lerp(const Vector2D& start, const Vector2D& end, float percent) {
	Vector2D result = start + (end - start) * percent;
	return result;
}