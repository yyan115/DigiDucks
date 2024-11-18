/******************************************************************************/
/*!
\file		Vector2D.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Sep 9 2024
\brief  	This file includes the definition of the Vector2D structure

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Vector2.h"
#include <math.h>

// Copy Constructor
/****************************************************************
* @brief Copy constructor for Vector2D.
*
* @param rhs The vector to copy from.
****************************************************************/
Vector2D::Vector2D(const Vector2D& rhs) {
	x = rhs.x;
	y = rhs.y;
}

/****************************************************************
* @brief Move constructor for Vector2D.
*
* @param rhs The vector to move.
****************************************************************/
Vector2D::Vector2D(Vector2D&& rhs) noexcept {
	x = rhs.x;
	y = rhs.y;
}

/****************************************************************
* @brief Assignment operator for copying a Vector2D.
*
* @param rhs The vector to assign from.
* @return The updated vector after assignment.
****************************************************************/
Vector2D& Vector2D::operator=(const Vector2D& rhs) {
	x = rhs.x;
	y = rhs.y;
	return *this;
}

// Assignment Operator
/****************************************************************
* @brief Adds a vector to the current vector.
*
* @param rhs The vector to add.
* @return The updated vector after addition.
****************************************************************/
Vector2D& Vector2D::operator+=(const Vector2D& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

/****************************************************************
* @brief Subtracts a vector from the current vector.
*
* @param rhs The vector to subtract.
* @return The updated vector after subtraction.
****************************************************************/
Vector2D& Vector2D::operator-=(const Vector2D& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

/****************************************************************
* @brief Multiplies the current vector by a scalar.
*
* @param scalar The scalar value to multiply by.
* @return The updated vector after multiplication.
****************************************************************/
Vector2D& Vector2D::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

/****************************************************************
* @brief Divides the current vector by a scalar.
*
* @param scalar The scalar value to divide by.
* @return The updated vector after division.
****************************************************************/
Vector2D& Vector2D::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}

/****************************************************************
* @brief Assigns both x and y coordinates to the same value.
*
* @param val The value to assign.
* @return The updated vector after assignment.
****************************************************************/
Vector2D& Vector2D::operator=(float val) {
	x = val;
	y = val;
	return *this;
}

// Negation Operator
/****************************************************************
* @brief Negates the vector (reverses direction).
*
* @return A new negated vector.
****************************************************************/
Vector2D Vector2D::operator -() const {
	return Vector2D(-x, -y);
}

// Operators Overloading
/****************************************************************
* @brief Overloaded operator + to add two vectors
*
* @param lhs - The left hand side vector
*
* @param rhs - The right hand side vector
*
* @return The sum of the two vectors
****************************************************************/
Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs) {
	return Vector2D(lhs.x + rhs.x, lhs.y + rhs.y);
}

/****************************************************************
* @brief Overloaded operator - to subtract two vectors
*
* @param lhs - The left hand side vector
*
* @param rhs - The right hand side vector
*
* @return The difference of the two vectors
****************************************************************/
Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs) {
	return Vector2D(lhs.x - rhs.x, lhs.y - rhs.y);
}

/****************************************************************
* @brief Overloaded operator * to multiply a vector by a scalar
*
* @param lhs - The left hand side vector
*
* @param rhs - The scalar
*
* @return The product of the vector and the scalar
****************************************************************/
Vector2D operator * (const Vector2D& lhs, float rhs) {
	return Vector2D(lhs.x * rhs, lhs.y * rhs);
}

/****************************************************************
* @brief Overloaded operator * to multiply a scalar by a vector
*
* @param lhs - The scalar
*
* @param rhs - The right hand side vector
*
* @return The product of the scalar and the vector
****************************************************************/
Vector2D operator * (float lhs, const Vector2D& rhs) {
	return Vector2D(lhs * rhs.x, lhs * rhs.y);
}

/****************************************************************
* @brief Overloaded operator / to divide a vector by a scalar
*
* @param lhs - The left hand side vector
*
* @param rhs - The scalar
*
* @return The quotient of the vector and the scalar
****************************************************************/
Vector2D operator / (const Vector2D& lhs, float rhs) {
	return Vector2D(lhs.x / rhs, lhs.y / rhs);
}

bool operator==(const Vector2D& lhs, const Vector2D& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Vector2D& lhs, const Vector2D& rhs)
{
	return !(lhs == rhs);
}

// Functions
/****************************************************************
* @brief Gets the normalized version of the vector.
*
* @return A normalized vector.
****************************************************************/
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

/****************************************************************
* @brief Gets the length (magnitude) of the vector.
*
* @return The length of the vector.
****************************************************************/
float Vector2D::length() const {
	float length = static_cast<float>(sqrt(x * x + y * y));
	return length;
}

/****************************************************************
* @brief Gets the squared length of the vector.
*        Useful when avoiding the overhead of sqrt() in length().
*
* @return The squared length of the vector.
****************************************************************/
float Vector2D::lengthSquared() const {
	float lengthSquared = x * x + y * y;
	return lengthSquared;
}

// Public Functions
/****************************************************************
* @brief Get the dot product of two vectors
*
* @param lhs - The left hand side vector
*
* @param rhs - The right hand side vector
*
* @return The dot product of the two vectors
****************************************************************/
float Vec2Dot(const Vector2D& lhs, const Vector2D& rhs) {
	float _x = lhs.x * rhs.x;
	float _y = lhs.y * rhs.y;
	return _x + _y;
}

/****************************************************************
* @brief Get the cross product of two vectors
*
* @param lhs - The left hand side vector
*
* @param rhs - The right hand side vector
*
* @return The cross product of the two vectors
****************************************************************/
float Vec2Cross(const Vector2D& lhs, const Vector2D& rhs) {
	float _x = lhs.x * rhs.y;
	float _y = lhs.y * rhs.x;
	return _x - _y;
}

/****************************************************************
* @brief Get the distance between two vectors
*
* @param lhs - The left hand side vector
*
* @param rhs - The right hand side vector
*
* @return The distance between the two vectors
****************************************************************/
float Vec2Dist(const Vector2D& lhs, const Vector2D& rhs) {
	Vector2D diff = lhs - rhs;
	return diff.length();
}

/****************************************************************
* @brief Lerp between two vectors
*
* @param start - The starting vector
*
* @param end - The ending vector
*
* @param percent - The percentage to lerp
*
* @return The lerped vector
****************************************************************/
Vector2D lerp(const Vector2D& start, const Vector2D& end, float percent) {
	Vector2D result = start + (end - start) * percent;
	return result;
}
