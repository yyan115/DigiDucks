/******************************************************************************/
/*!
\file		Vector2D.h
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
