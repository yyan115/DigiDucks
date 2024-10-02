/******************************************************************************/
/*!
\file       Matrix3x3.cpp
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       Sep 9 2024
\brief      This file includes the definition of the Matrix3x3 structure

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Matrix3x3.h"
#include <math.h>

// Constructors
Matrix3x3::Matrix3x3() {
    MtxIdentity(*this);
}

Matrix3x3::Matrix3x3(const float* ptArr) {
    for (int i = 0; i < 9; ++i) {
        m[i] = ptArr[i];
    }
}

Matrix3x3::Matrix3x3(float _m00, float _m01, float _m02,
    float _m10, float _m11, float _m12,
    float _m20, float _m21, float _m22) {
    m[0] = _m00; m[1] = _m01; m[2] = _m02;
    m[3] = _m10; m[4] = _m11; m[5] = _m12;
    m[6] = _m20; m[7] = _m21; m[8] = _m22;
}

// Destructor
Matrix3x3::~Matrix3x3() {}

// Copy Constructor
Matrix3x3::Matrix3x3(const Matrix3x3& rhs) {
    for (int i = 0; i < 9; ++i) {
        m[i] = rhs.m[i];
    }
}

// Assignment Operator
Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs) {
    if (this != &rhs) {
        for (int i = 0; i < 9; ++i) {
            m[i] = rhs.m[i];
        }
    }
    return *this;
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& rhs) {
    for (int i = 0; i < 9; ++i) {
        m[i] += rhs.m[i];
    }
    return *this;
}

Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& rhs) {
    for (int i = 0; i < 9; ++i) {
        m[i] -= rhs.m[i];
    }
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs) {
    Matrix3x3 temp;
    temp.m[0] = (m[0] * rhs.m[0]) + (m[1] * rhs.m[3]) + (m[2] * rhs.m[6]);
    temp.m[1] = (m[0] * rhs.m[1]) + (m[1] * rhs.m[4]) + (m[2] * rhs.m[7]);
    temp.m[2] = (m[0] * rhs.m[2]) + (m[1] * rhs.m[5]) + (m[2] * rhs.m[8]);

    temp.m[3] = (m[3] * rhs.m[0]) + (m[4] * rhs.m[3]) + (m[5] * rhs.m[6]);
    temp.m[4] = (m[3] * rhs.m[1]) + (m[4] * rhs.m[4]) + (m[5] * rhs.m[7]);
    temp.m[5] = (m[3] * rhs.m[2]) + (m[4] * rhs.m[5]) + (m[5] * rhs.m[8]);

    temp.m[6] = (m[6] * rhs.m[0]) + (m[7] * rhs.m[3]) + (m[8] * rhs.m[6]);
    temp.m[7] = (m[6] * rhs.m[1]) + (m[7] * rhs.m[4]) + (m[8] * rhs.m[7]);
    temp.m[8] = (m[6] * rhs.m[2]) + (m[7] * rhs.m[5]) + (m[8] * rhs.m[8]);

    *this = temp;
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(float scalar) {
    for (int i = 0; i < 9; ++i) {
        m[i] *= scalar;
    }
    return *this;
}

Matrix3x3& Matrix3x3::operator/=(float scalar) {
    if (scalar != 0.f) {
        for (int i = 0; i < 9; ++i) {
            m[i] /= scalar;
        }
    }
    return *this;
}

bool Matrix3x3::operator==(const Matrix3x3& rhs) const {
    for (int i = 0; i < 9; ++i) {
        if (m[i] != rhs.m[i]) return false;
    }
    return true;
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
    temp.x = (lhs.m[0] * rhs.x) + (lhs.m[1] * rhs.y) + lhs.m[2];
    temp.y = (lhs.m[3] * rhs.x) + (lhs.m[4] * rhs.y) + lhs.m[5];
    return temp;
}

Matrix3x3 operator/(const Matrix3x3& lhs, float scalar) {
    Matrix3x3 temp(lhs);
    temp /= scalar;
    return temp;
}

// Functions
void MtxZero(Matrix3x3& result) {
    for (int i = 0; i < 9; ++i) {
        result.m[i] = 0.f;
    }
}

void MtxIdentity(Matrix3x3& result) {
    MtxZero(result);
    result.m[0] = result.m[4] = result.m[8] = 1.f;
}

void MtxTranslate(Matrix3x3& result, float x, float y) {
    MtxIdentity(result);
    result.m[2] = x;
    result.m[5] = y;
}

void MtxScale(Matrix3x3& result, float x, float y) {
    MtxZero(result);
    result.m[0] = x;
    result.m[4] = y;
    result.m[8] = 1.f;
}

void MtxRotateRad(Matrix3x3& result, float angle) {
    result.m[0] = cosf(angle);
    result.m[1] = -sinf(angle);
    result.m[3] = sinf(angle);
    result.m[4] = cosf(angle);
    result.m[8] = 1.f;
}

void MtxRotateDeg(Matrix3x3& result, float angle) {
    MtxRotateRad(result, angle * static_cast<float>(M_PI) / 180.f);
}

void MtxTranspose(Matrix3x3& result, const Matrix3x3& mat) {
    result.m[0] = mat.m[0];
    result.m[1] = mat.m[3];
    result.m[2] = mat.m[6];
    result.m[3] = mat.m[1];
    result.m[4] = mat.m[4];
    result.m[5] = mat.m[7];
    result.m[6] = mat.m[2];
    result.m[7] = mat.m[5];
    result.m[8] = mat.m[8];
}

void MtxDeterminant(float& determinant, const Matrix3x3& mat) {
    determinant = mat.m[0] * (mat.m[4] * mat.m[8] - mat.m[5] * mat.m[7]) -
        mat.m[1] * (mat.m[3] * mat.m[8] - mat.m[5] * mat.m[6]) +
        mat.m[2] * (mat.m[3] * mat.m[7] - mat.m[4] * mat.m[6]);
}

void MtxCofactor(Matrix3x3& result, const Matrix3x3& mat) {
    result.m[0] = mat.m[4] * mat.m[8] - mat.m[5] * mat.m[7];
    result.m[1] = -(mat.m[3] * mat.m[8] - mat.m[5] * mat.m[6]);
    result.m[2] = mat.m[3] * mat.m[7] - mat.m[4] * mat.m[6];
    result.m[3] = -(mat.m[1] * mat.m[8] - mat.m[2] * mat.m[7]);
    result.m[4] = mat.m[0] * mat.m[8] - mat.m[2] * mat.m[6];
    result.m[5] = -(mat.m[0] * mat.m[7] - mat.m[1] * mat.m[6]);
    result.m[6] = mat.m[1] * mat.m[5] - mat.m[2] * mat.m[4];
    result.m[7] = -(mat.m[0] * mat.m[5] - mat.m[2] * mat.m[3]);
    result.m[8] = mat.m[0] * mat.m[4] - mat.m[1] * mat.m[3];
}

void MtxInverse(Matrix3x3& result, const Matrix3x3& mat) {
    float det;
    MtxDeterminant(det, mat);
    if (det == 0.f) return;
    Matrix3x3 cofactor;
    MtxCofactor(cofactor, mat);
    MtxTranspose(result, cofactor);
    result /= det;
}
