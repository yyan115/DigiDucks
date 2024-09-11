/******************************************************************************/
/*!
\file		Matrix3x3.cpp
\author 	Ernest Ho, h.yonghengernest, 2301223
\par    	h.yonghengernestt@digipen.edu
\date   	Sep 9 2024
\brief  	This file includes the definition of the Vector2D structure

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Matrix3x3.h"
#include <math.h>

// Constructors
Matrix3x3::Matrix3x3(const float* ptArr) {
	m2[0][0] = m[0] = m00 = ptArr[0];
	m2[0][1] = m[1] = m01 = ptArr[1];
	m2[0][2] = m[2] = m02 = ptArr[2];

	m2[1][0] = m[3] = m10 = ptArr[3];
	m2[1][1] = m[4] = m11 = ptArr[4];
	m2[1][2] = m[5] = m12 = ptArr[5];

	m2[2][0] = m[6] = m20 = ptArr[6];
	m2[2][1] = m[7] = m21 = ptArr[7];
	m2[2][2] = m[8] = m22 = ptArr[8];
}

Matrix3x3::Matrix3x3(	float _m00, float _m01, float _m02,
						float _m10, float _m11, float _m12,
						float _m20, float _m21, float _m22) {
	m2[0][0] = m[0] = m00 = _m00;
	m2[0][1] = m[1] = m01 = _m01;
	m2[0][2] = m[2] = m02 = _m02;

	m2[1][0] = m[3] = m10 = _m10;
	m2[1][1] = m[4] = m11 = _m11;
	m2[1][2] = m[5] = m12 = _m12;

	m2[2][0] = m[6] = m20 = _m20;
	m2[2][1] = m[7] = m21 = _m21;
	m2[2][2] = m[8] = m22 = _m22;

}

// Copy Constructor

Matrix3x3::Matrix3x3(const Matrix3x3& rhs) {
	m2[0][0] = m[0] = m00 = rhs.m00;
	m2[0][1] = m[1] = m01 = rhs.m01;
	m2[0][2] = m[2] = m02 = rhs.m02;

	m2[1][0] = m[3] = m10 = rhs.m10;
	m2[1][1] = m[4] = m11 = rhs.m11;
	m2[1][2] = m[5] = m12 = rhs.m12;

	m2[2][0] = m[6] = m20 = rhs.m20;
	m2[2][1] = m[7] = m21 = rhs.m21;
	m2[2][2] = m[8] = m22 = rhs.m22;
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs) {
	m2[0][0] = m[0] = m00 = rhs.m00;
	m2[0][1] = m[1] = m01 = rhs.m01;
	m2[0][2] = m[2] = m02 = rhs.m02;

	m2[1][0] = m[3] = m10 = rhs.m10;
	m2[1][1] = m[4] = m11 = rhs.m11;
	m2[1][2] = m[5] = m12 = rhs.m12;

	m2[2][0] = m[6] = m20 = rhs.m20;
	m2[2][1] = m[7] = m21 = rhs.m21;
	m2[2][2] = m[8] = m22 = rhs.m22;

	return *this;
}

// Assignment Operator
Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& rhs) {
	m2[0][0] = m[0] = m00 += rhs.m00;
	m2[0][1] = m[1] = m01 += rhs.m01;
	m2[0][2] = m[2] = m02 += rhs.m02;

	m2[1][0] = m[3] = m10 += rhs.m10;
	m2[1][1] = m[4] = m11 += rhs.m11;
	m2[1][2] = m[5] = m12 += rhs.m12;

	m2[2][0] = m[6] = m20 += rhs.m20;
	m2[2][1] = m[7] = m21 += rhs.m21;
	m2[2][2] = m[8] = m22 += rhs.m22;

	return *this;
}

Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& rhs) {
	m2[0][0] = m[0] = m00 -= rhs.m00;
	m2[0][1] = m[1] = m01 -= rhs.m01;
	m2[0][2] = m[2] = m02 -= rhs.m02;

	m2[1][0] = m[3] = m10 -= rhs.m10;
	m2[1][1] = m[4] = m11 -= rhs.m11;
	m2[1][2] = m[5] = m12 -= rhs.m12;

	m2[2][0] = m[6] = m20 -= rhs.m20;
	m2[2][1] = m[7] = m21 -= rhs.m21;
	m2[2][2] = m[8] = m22 -= rhs.m22;

	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs) {
	Matrix3x3 temp;
	// Row * Column
	temp.m[0] = (m2[0][0] * rhs.m2[0][0]) + (m2[0][1] * rhs.m2[1][0]) + (m2[0][2] * rhs.m2[2][0]);
	temp.m[1] = (m2[0][0] * rhs.m2[0][1]) + (m2[0][1] * rhs.m2[1][1]) + (m2[0][2] * rhs.m2[2][1]);
	temp.m[2] = (m2[0][0] * rhs.m2[0][2]) + (m2[0][1] * rhs.m2[1][2]) + (m2[0][2] * rhs.m2[2][2]);

	temp.m[3] = (m2[1][0] * rhs.m2[0][0]) + (m2[1][1] * rhs.m2[1][0]) + (m2[1][2] * rhs.m2[2][0]);
	temp.m[4] = (m2[1][0] * rhs.m2[0][1]) + (m2[1][1] * rhs.m2[1][1]) + (m2[1][2] * rhs.m2[2][1]);
	temp.m[5] = (m2[1][0] * rhs.m2[0][2]) + (m2[1][1] * rhs.m2[1][2]) + (m2[1][2] * rhs.m2[2][2]);

	temp.m[6] = (m2[2][0] * rhs.m2[0][0]) + (m2[2][1] * rhs.m2[1][0]) + (m2[2][2] * rhs.m2[2][0]);
	temp.m[7] = (m2[2][0] * rhs.m2[0][1]) + (m2[2][1] * rhs.m2[1][1]) + (m2[2][2] * rhs.m2[2][1]);
	temp.m[8] = (m2[2][0] * rhs.m2[0][2]) + (m2[2][1] * rhs.m2[1][2]) + (m2[2][2] * rhs.m2[2][2]);

	*this = temp;
	return *this;
}

Matrix3x3& Matrix3x3::operator*=(float scalar) {
	m2[0][0] = m[0] = m00 *= scalar;
	m2[0][1] = m[1] = m01 *= scalar;
	m2[0][2] = m[2] = m02 *= scalar;

	m2[1][0] = m[3] = m10 *= scalar;
	m2[1][1] = m[4] = m11 *= scalar;
	m2[1][2] = m[5] = m12 *= scalar;

	m2[2][0] = m[6] = m20 *= scalar;
	m2[2][1] = m[7] = m21 *= scalar;
	m2[2][2] = m[8] = m22 *= scalar;

	return *this;
}

Matrix3x3& Matrix3x3::operator/=(float scalar) {
	if(scalar == 0.f) {
		return *this;
	}
	m2[0][0] = m[0] = m00 /= scalar;
	m2[0][1] = m[1] = m01 /= scalar;
	m2[0][2] = m[2] = m02 /= scalar;

	m2[1][0] = m[3] = m10 /= scalar;
	m2[1][1] = m[4] = m11 /= scalar;
	m2[1][2] = m[5] = m12 /= scalar;

	m2[2][0] = m[6] = m20 /= scalar;
	m2[2][1] = m[7] = m21 /= scalar;
	m2[2][2] = m[8] = m22 /= scalar;

	return *this;
}

bool Matrix3x3::operator==(const Matrix3x3& rhs) const {
	return (m00 == rhs.m00 && m01 == rhs.m01 && m02 == rhs.m02 &&
			m10 == rhs.m10 && m11 == rhs.m11 && m12 == rhs.m12 &&
			m20 == rhs.m20 && m21 == rhs.m21 && m22 == rhs.m22);
}

// Operators Overloading
Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs) {
	Matrix3x3 temp(lhs);
	temp += rhs;
	return temp;
}

Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs) {
	Matrix3x3 temp(lhs);
	temp -= rhs;
	return temp;
}

Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs) {
	Matrix3x3 temp(lhs);
	temp *= rhs;
	return temp;
}

Matrix3x3 operator*(const Matrix3x3& lhs, float scalar) {
	Matrix3x3 temp(lhs);
	temp *= scalar;
	return temp;
}

Matrix3x3 operator*(float scalar, const Matrix3x3& rhs) {
	Matrix3x3 temp(rhs);
	temp *= scalar;
	return temp;
}

Vector2D operator*(const Matrix3x3& lhs, const Vector2D& rhs) {
	Vector2D temp;

	temp.x = (lhs.m2[0][0] * rhs.x) + (lhs.m2[0][1] * rhs.y) + lhs.m2[0][2];
	temp.y = (lhs.m2[1][0] * rhs.x) + (lhs.m2[1][1] * rhs.y) + lhs.m2[1][2];

	return temp;
}

Matrix3x3 operator/(const Matrix3x3& lhs, float scalar) {
	Matrix3x3 temp(lhs);
	temp /= scalar;
	return temp;
}


// Functions
void MtxZero(Matrix3x3& result) {
	// 0 0 0
	// 0 0 0
	// 0 0 0

	result.m00 = result.m[0] = result.m2[0][0] = 0.f;
	result.m01 = result.m[1] = result.m2[0][1] = 0.f;
	result.m02 = result.m[2] = result.m2[0][2] = 0.f;

	result.m10 = result.m[3] = result.m2[1][0] = 0.f;
	result.m11 = result.m[4] = result.m2[1][1] = 0.f;
	result.m12 = result.m[5] = result.m2[1][2] = 0.f;

	result.m20 = result.m[6] = result.m2[2][0] = 0.f;
	result.m21 = result.m[7] = result.m2[2][1] = 0.f;
	result.m22 = result.m[8] = result.m2[2][2] = 0.f;

}

void MtxIdentity(Matrix3x3& result) {
	MtxZero(result);

	result.m00 = result.m[0] = result.m2[0][0] = 1.f;	// 1 0 0
	result.m11 = result.m[4] = result.m2[1][1] = 1.f;	// 0 1 0
	result.m22 = result.m[8] = result.m2[2][2] = 1.f;	// 0 0 1
}

void MtxTranslate(Matrix3x3& result, float x, float y) {
	MtxIdentity(result);

	result.m02 = result.m[2] = result.m2[0][2] = x;		// 1 0 x
	result.m12 = result.m[5] = result.m2[1][2] = y;		// 0 1 y
}

void MtxScale(Matrix3x3& result, float x, float y) {
	MtxZero(result);

	result.m00 = result.m[0] = result.m2[0][0] = x;		// x 0 0
	result.m11 = result.m[4] = result.m2[1][1] = y;		// 0 y 0
	result.m22 = result.m[8] = result.m2[2][2] = 1.f;	// 0 0 1
}

void MtxRotateRad(Matrix3x3& result, float angle) {
	// 2D Rotate along the z-axis
	// cos -sin 0
	// sin cos  0
	// 0   0    1
	result.m00 = result.m[0] = result.m2[0][0] = static_cast<float>(cos(angle));
	result.m01 = result.m[1] = result.m2[0][1] = static_cast<float>(-sin(angle));
	result.m02 = result.m[2] = result.m2[0][2] = 0.f;

	result.m10 = result.m[3] = result.m2[1][0] = static_cast<float>(sin(angle));
	result.m11 = result.m[4] = result.m2[1][1] = static_cast<float>(cos(angle));
	result.m12 = result.m[5] = result.m2[1][2] = 0.f;

	result.m20 = result.m[6] = result.m2[2][0] = 0.f;
	result.m21 = result.m[7] = result.m2[2][1] = 0.f;
	result.m22 = result.m[8] = result.m2[2][2] = 1.f;
}

void MtxRotateDeg(Matrix3x3& result, float angle) {
	MtxRotateRad(result, static_cast<float>(angle * M_PI / 180.f));
}

void MtxTranspose(Matrix3x3& result, const Matrix3x3& mat) {

	// Transpose the matrix
	// 0 1 2	-> 0 3 6
	// 3 4 5	-> 1 4 7
	// 6 7 8	-> 2 5 8
	
	Matrix3x3 temp(mat);	// Allow for in-place transpose

	result.m00 = result.m[0] = result.m2[0][0] = temp.m[0];
	result.m01 = result.m[1] = result.m2[0][1] = temp.m[3];
	result.m02 = result.m[2] = result.m2[0][2] = temp.m[6];

	result.m10 = result.m[3] = result.m2[1][0] = temp.m[1];
	result.m11 = result.m[4] = result.m2[1][1] = temp.m[4];
	result.m12 = result.m[5] = result.m2[1][2] = temp.m[7];

	result.m20 = result.m[6] = result.m2[2][0] = temp.m[2];
	result.m21 = result.m[7] = result.m2[2][1] = temp.m[5];
	result.m22 = result.m[8] = result.m2[2][2] = temp.m[8];
}

void MtxDeterminant(float& determinant, const Matrix3x3& mat) {

	// Matrix:
	// A0	|	A1	|	A2
	// B0	|	B1	|	B2
	// C0	|	C1	|	C2
	float	A0{ mat.m[0] }, A1{ mat.m[1] }, A2{ mat.m[2] },
			B0{ mat.m[3] }, B1{ mat.m[4] }, B2{ mat.m[5] },
			C0{ mat.m[6] }, C1{ mat.m[7] }, C2{ mat.m[8] };


	// Determinant = Main Diagonal - Anti Diagonal
	float m_D{}, a_D{};

	m_D = (A0 * B1 * C2) + (A1 * B2 * C0) + (B0 * C1 * A2);

	a_D = (A2 * B1 * C0) + (A1 * B0 * C2) + (A0 * C1 * B2);

	determinant = m_D - a_D;
}


void MtxCofactor(Matrix3x3& result, const Matrix3x3& mat) {
	// Cofactors:
	// + (B1*C2 - C1*B2)  |  - (B0*C2 - C0*B2 ) |  + (B0*C1 - C0*B1)
	// - (A1*C2 - C1*A2)  |  + (A0*C2 - C0*A2)  |  - (A0*C1 - C0*A1)
	// + (A1*B2 - B1*A2)  |  - (A0*B2 - B0*A2)  |  + (A0*B1 - B0*A1)

	float	A0{ mat.m[0] }, A1{ mat.m[1] }, A2{ mat.m[2] },
			B0{ mat.m[3] }, B1{ mat.m[4] }, B2{ mat.m[5] },
			C0{ mat.m[6] }, C1{ mat.m[7] }, C2{ mat.m[8] };

	result.m00 = result.m[0] = result.m2[0][0] = (B1 * C2) - (C1 * B2);
	result.m01 = result.m[1] = result.m2[0][1] = -((B0 * C2) - (C0 * B2));
	result.m02 = result.m[2] = result.m2[0][2] = (B0 * C1) - (C0 * B1);

	result.m10 = result.m[3] = result.m2[1][0] = -((A1 * C2) - (C1 * A2));
	result.m11 = result.m[4] = result.m2[1][1] = (A0 * C2) - (C0 * A2);
	result.m12 = result.m[5] = result.m2[1][2] = -((A0 * C1) - (C0 * A1));

	result.m20 = result.m[6] = result.m2[2][0] = (A1 * B2) - (B1 * A2);
	result.m21 = result.m[7] = result.m2[2][1] = -((A0 * B2) - (B0 * A2));
	result.m22 = result.m[8] = result.m2[2][2] = (A0 * B1) - (B0 * A1);

}


void MtxInverse(Matrix3x3& result, const Matrix3x3& mat) {
	float det{};
	MtxDeterminant(det, mat);

	// If det is no 0 inverse the matrix
	if(det == 0) {
		return;
	}

	// Get the cofactor matrix
	Mat3 cofactor{};
	MtxCofactor(cofactor, mat);

	// Transpose the cofactor matrix
	Mat3 co_trans{};
	MtxTranspose(co_trans, cofactor);

	// Multiply 1/det to the transposed cofactor matrix
	// to get the inverse matrix
	result = co_trans / det;
}