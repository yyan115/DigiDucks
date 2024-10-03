/******************************************************************************/
/*!
\file		Vector2D.h
\author 	Ernest Ho, h.yonghengernest, 2301223
(100%)
\email    	h.yonghengernestt@digipen.edu
\date   	Sep 9 2024
\brief  	This file includes the definition of the Vector2D class, which 
            represents a 2D vector and provides various vector operations 
            such as addition, subtraction, scalar multiplication, normalization,
            and length calculations. It also includes several constructors and 
            overloaded operators to simplify vector manipulation.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#pragma once
#define M_PI 3.14159265358979323846

class DUCKENGINE_API Vector2D {
public:
	float x, y;

	// Constructors
	/****************************************************************
	* @brief Default constructor for Vector2D, initializes x and y to 0.
	****************************************************************/
	Vector2D() : x(0.f), y(0.f) {}

	/****************************************************************
	* @brief Constructor that initializes Vector2D with given x and y values.
	*
	* @param _x The x-coordinate value.
	* @param _y The y-coordinate value.
	****************************************************************/
	Vector2D(float _x, float _y) : x(_x), y(_y) {}

	// Copy Constructor
	/****************************************************************
	* @brief Copy constructor for Vector2D.
	*
	* @param rhs The vector to copy from.
	****************************************************************/
	Vector2D(const Vector2D& rhs);

	// Move Constructor
	/****************************************************************
	* @brief Move constructor for Vector2D.
	*
	* @param rhs The vector to move.
	****************************************************************/
	Vector2D(Vector2D&& rhs) noexcept;

	// Assignment Operator
	/****************************************************************
	* @brief Assignment operator for copying a Vector2D.
	*
	* @param rhs The vector to assign from.
	* @return The updated vector after assignment.
	****************************************************************/
	Vector2D& operator=(const Vector2D& rhs);

	// Destructor
	~Vector2D() = default;

	// Assignment Operator
	/****************************************************************
	* @brief Adds a vector to the current vector.
	*
	* @param rhs The vector to add.
	* @return The updated vector after addition.
	****************************************************************/
	Vector2D& operator+=(const Vector2D& rhs);

	/****************************************************************
	* @brief Subtracts a vector from the current vector.
	*
	* @param rhs The vector to subtract.
	* @return The updated vector after subtraction.
	****************************************************************/
	Vector2D& operator-=(const Vector2D& rhs);

	/****************************************************************
	* @brief Multiplies the current vector by a scalar.
	*
	* @param scalar The scalar value to multiply by.
	* @return The updated vector after multiplication.
	****************************************************************/
	Vector2D& operator*=(float scalar);

	/****************************************************************
	* @brief Divides the current vector by a scalar.
	*
	* @param scalar The scalar value to divide by.
	* @return The updated vector after division.
	****************************************************************/
	Vector2D& operator/=(float scalar);

	/****************************************************************
	* @brief Assigns both x and y coordinates to the same value.
	*
	* @param val The value to assign.
	* @return The updated vector after assignment.
	****************************************************************/
	Vector2D& operator=(float val);

	// Negation Operator
	/****************************************************************
	* @brief Negates the vector (reverses direction).
	*
	* @return A new negated vector.
	****************************************************************/
	Vector2D operator -() const;

	// Functions
	/****************************************************************
	* @brief Gets the normalized version of the vector.
	*
	* @return A normalized vector.
	****************************************************************/
	Vector2D normalized() const;
	/****************************************************************
	* @brief Gets the length (magnitude) of the vector.
	*
	* @return The length of the vector.
	****************************************************************/
	float length() const;
	/****************************************************************
	* @brief Gets the squared length of the vector.
	*        Useful when avoiding the overhead of sqrt() in length().
	*
	* @return The squared length of the vector.
	****************************************************************/
	float lengthSquared() const;

};

typedef Vector2D Vec2;

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
DUCKENGINE_API Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator - to subtract two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The difference of the two vectors
****************************************************************/
DUCKENGINE_API Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator * to multiply a vector by a scalar
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The scalar
* 
* @return The product of the vector and the scalar
****************************************************************/
DUCKENGINE_API Vector2D operator * (const Vector2D& lhs, float rhs);

/****************************************************************
* @brief Overloaded operator * to multiply a scalar by a vector
* 
* @param lhs - The scalar
* 
* @param rhs - The right hand side vector
* 
* @return The product of the scalar and the vector
****************************************************************/
DUCKENGINE_API Vector2D operator * (float lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator / to divide a vector by a scalar
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The scalar
* 
* @return The quotient of the vector and the scalar
****************************************************************/
DUCKENGINE_API Vector2D operator / (const Vector2D& lhs, float rhs);

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
DUCKENGINE_API float Vec2Dot(const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Get the cross product of two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The cross product of the two vectors
****************************************************************/
DUCKENGINE_API float Vec2Cross(const Vector2D& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Get the distance between two vectors
* 
* @param lhs - The left hand side vector
* 
* @param rhs - The right hand side vector
* 
* @return The distance between the two vectors
****************************************************************/
DUCKENGINE_API float Vec2Dist(const Vector2D& lhs, const Vector2D& rhs);

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
DUCKENGINE_API Vector2D lerp(const Vector2D& start, const Vector2D& end, float percent);
