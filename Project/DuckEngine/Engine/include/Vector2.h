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

#pragma once

typedef union Vector2D {

	struct {
		float x, y;
	};

	float m[2];

	// Constructors
	Vector2D() : x(0.f), y(0.f) { m[0] = x; m[1] = y; }
	Vector2D(float x, float y) : x(x), y(y) { m[0] = x; m[1] = y; }

	// Copy Constructor
	Vector2D(const Vector2D& rhs);
	Vector2D& operator=(const Vector2D& rhs);

	// Destructor
	~Vector2D() = default;

	// Assignment Operator
	Vector2D& operator+=(const Vector2D& rhs);
	Vector2D& operator-=(const Vector2D& rhs);
	Vector2D& operator*=(float scalar);
	Vector2D& operator/=(float scalar);
	Vector2D& operator=(float val);

	// Negate
	Vector2D operator -() const;

	// Functions
	/****************************************************************
	* @brief Get the normalized of the vector
	* 
	* @return A normalized vector
	****************************************************************/
	Vector2D normalized() const;
	/****************************************************************
	* @brief Get the length of the vector
	* 
	* @return The length of the vector
	****************************************************************/
	float length() const;

} Vector2D, Vec2;

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
Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator - to subtract two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The difference of the two vectors
****************************************************************/
Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator * to multiply a vector by a scalar
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The scalar
* 
* @return The product of the vector and the scalar
****************************************************************/
Vector2D operator * (const Vector2D& lhs, float rhs);

/****************************************************************
* @brief Overloaded operator * to multiply a scalar by a vector
* 
* @param lhs - The scalar
* 
* @param rhs - The right hand side vector
* 
* @return The product of the scalar and the vector
****************************************************************/
Vector2D operator * (float lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator / to divide a vector by a scalar
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The scalar
* 
* @return The quotient of the vector and the scalar
****************************************************************/
Vector2D operator / (const Vector2D& lhs, float rhs);

// Functions
/****************************************************************
* @brief Get the dot product of two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The dot product of the two vectors
****************************************************************/
float Vec2Dot(const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Get the cross product of two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The cross product of the two vectors
****************************************************************/
float Vec2Cross(const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Get the distance between two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The distance between the two vectors
****************************************************************/
float Vec2Dist(const Vector2D& lhs, const Vector2D& rhs);

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
Vector2D lerp(const Vector2D& start, const Vector2D& end, float percent);
