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
	Matrix3x3& operator+=(const Matrix3x3& rhs);
	Matrix3x3& operator-=(const Matrix3x3& rhs);
	Matrix3x3& operator*=(const Matrix3x3& rhs);
	Matrix3x3& operator*=(float scalar);
	Matrix3x3& operator/=(float scalar);



}Mat3, Matrix3x3, Mtx3x3;