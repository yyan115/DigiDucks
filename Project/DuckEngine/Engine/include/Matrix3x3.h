/******************************************************************************/
/*!
\file		Matrix3x3.h
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
#include "Vector2.h"

typedef union Matrix3x3 {
	struct {
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};
	float m[9];
	float m2[3][3];

	// Constructors
	// Default Constructor
	Matrix3x3() : m00(1.f), m01(0.f), m02(0.f), m10(0.f), m11(1.f), m12(0.f), m20(0.f), m21(0.f), m22(1.f) {}
	// Constructor with an array of 9 floats
	Matrix3x3(const float* ptArr) {}
	// Constructor with 9 floats
	Matrix3x3(	float _m00, float _m01, float _m02, 
				float _m10, float _m11, float _m12, 
				float _m20, float _m21, float _m22) :
				m00(_m00), m01(_m01), m02(_m02), 
				m10(_m10), m11(_m11), m12(_m12), 
				m20(_m20), m21(_m21), m22(_m22) {}

	// Copy Constructor
	Matrix3x3(const Matrix3x3& rhs);
	Matrix3x3& operator=(const Matrix3x3& rhs);

	// Destructor
	~Matrix3x3() = default;

	// Assignment Operator
	/****************************************************************
	* @brief Assignment operator for addition
	* 
	* @param rhs The right hand side matrix
	* 
	* @return The matrix after addition	
	****************************************************************/
	Matrix3x3& operator+=(const Matrix3x3& rhs);
	/****************************************************************
	* @brief Assignment operator for subtraction
	* 
	* @param rhs The right hand side matrix
	* 
	* @return The matrix after subtraction
	****************************************************************/
	Matrix3x3& operator-=(const Matrix3x3& rhs);
	/****************************************************************
	* @brief Assignment operator for multiplication
	* 
	* @param rhs The right hand side matrix
	* 
	* @return The matrix after multiplication
	****************************************************************/
	Matrix3x3& operator*=(const Matrix3x3& rhs);
	/****************************************************************
	* @brief Assignment operator for multiplication with a scalar
	* 
	* @param scalar The scalar value
	* 
	* @return The matrix after multiplication with a scalar
	***************************************************************/
	Matrix3x3& operator*=(float scalar);
	/****************************************************************
	* @brief Assignment operator for division with a scalar
	* 
	* @param scalar The scalar value
	* 
	* @return The matrix after division with a scalar
	****************************************************************/
	Matrix3x3& operator/=(float scalar);
	/****************************************************************
	* @brief Check if the matrix is equal to another matrix
	*	
	* @param rhs The right hand side matrix
	*	
	* @return True if the matrices are equal, false otherwise
	***************************************************************/
	bool operator==(const Matrix3x3& rhs) const;

}Mat3, Matrix3x3, Mtx3x3;


// Operators Overloading
/****************************************************************
* @brief Overloaded operator + to add two matrices
* 
* @param lhs - The left hand side matrix
* 
* @param rhs - The right hand side matrix
* 
* @return The sum of the two matrices
***************************************************************/
Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs);

/****************************************************************
* @brief Overloaded operator - to subtract two matrices
* 
* @param lhs - The left hand side matrix
* 
* @param rhs - The right hand side matrix
* 
* @return The difference of the two matrices
***************************************************************/
Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs);

/****************************************************************
* @brief Overloaded operator * to multiply two matrices
* 
* @param lhs - The left hand side matrix
* 
* @param rhs - The right hand side matrix
* 
* @return The product of the two matrices
***************************************************************/
Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs);

/****************************************************************
* @brief Overloaded operator * to multiply a matrix by a scalar
* 
* @param lhs - The left hand side matrix
* 
* @param scalar - The scalar
* 
* @return The product of the matrix and the scalar
***************************************************************/
Matrix3x3 operator*(const Matrix3x3& lhs, float scalar);

/****************************************************************
* @brief Overloaded operator * to multiply a scalar by a matrix
* 
* @param scalar - The scalar
* 
* @param rhs - The right hand side matrix
* 
* @return The product of the scalar and the matrix
***************************************************************/
Matrix3x3 operator*(float scalar, const Matrix3x3& rhs);

/****************************************************************
* @brief Overloaded operator / to divide a matrix by a scalar
* 
* @param lhs - The left hand side matrix
* 
* @param scalar - The scalar
* 
* @return The quotient of the matrix and the scalar
***************************************************************/
Vector2D operator*(const Matrix3x3& lhs, const Vector2D& rhs);

/****************************************************************
* @brief Overloaded operator / to divide a matrix by a scalar
* 
* @param lhs - The left hand side matrix
* 
* @param scalar - The scalar
* 
* @return The quotient of the matrix and the scalar
***************************************************************/
Matrix3x3 operator/(const Matrix3x3& lhs, float scalar);


// Functions
/****************************************************************
* @brief Set the matrix to zero
* 
* @param result The matrix to set to zero
****************************************************************/
void MtxZero(Matrix3x3& result);

/****************************************************************
* @brief Set the matrix to the identity matrix
* 
* @param result The matrix to set to the identity matrix
****************************************************************/
void MtxIdentity(Matrix3x3& result);

/****************************************************************
* @brief Translate the matrix by the given x and y values
* 
* @param result The matrix to translate
* 
* @param x The x value to translate by
* 
* @param y The y value to translate by
****************************************************************/
void MtxTranslate(Matrix3x3& result, float x, float y);

/****************************************************************
* @brief Scale the matrix by the given x and y values
* 
* @param result The matrix to scale
* 
* @param x The x value to scale by
*	
* @param y The y value to scale by
****************************************************************/
void MtxScale(Matrix3x3& result, float x, float y);

/****************************************************************
* @brief Rotate the matrix by the given angle in radians
* 
* @param result The matrix to rotate
* 
* @param angle The angle to rotate by in radians
**************************************************************/
void MtxRotateRad(Matrix3x3& result, float angle);

/****************************************************************
* @brief Rotate the matrix by the given angle in degrees
* 
* @param result The matrix to rotate
* 
* @param angle The angle to rotate by in degrees
**************************************************************/
void MtxRotateDeg(Matrix3x3& result, float angle);

/****************************************************************
* @bried Transpose the matrix
* 
* @param result The matrix to transpose
* 
* @param mat The matrix to transpose
**************************************************************/
void MtxTranspose(Matrix3x3& result, const Matrix3x3& mat);

/****************************************************************
* @brief Get the Inverse of the matrix
*	
* @param result The matrix to get the inverse
* 
* @param mat The matrix to get the inverse
**************************************************************/
void MtxInverse(Matrix3x3& result, const Matrix3x3& mat);

/****************************************************************
* @brief Get the determinant of the matrix
* 
* @param result The determinant of the matrix
* 
* @param mat The matrix to get the determinant
**************************************************************/
void MtxDeterminant(float& result, const Matrix3x3& mat);

/****************************************************************
* @brief Get the cofactor of the matrix
* 
* @param result The cofactor of the matrix
* 
* @param mat The matrix to get the cofactor
***************************************************************/
void MtxCofactor(Matrix3x3& result, const Matrix3x3& mat);