#include "Matrix3x3.h"
#include <Matrix3x3.h>
#include <Vector2.h>
#include <Mathf.h>
#include <sstream>

Matrix3x3::Matrix3x3() : matrix{ 0 }
{}

Matrix3x3::Matrix3x3(
    float m11, float m12, float m13,
    float m21, float m22, float m23,
    float m31, float m32, float m33) :
    m11{ m11 }, m12{ m12 }, m13{ m13 },
    m21{ m21 }, m22{ m22 }, m23{ m23 },
    m31{ m31 }, m32{ m32 }, m33{ m33 }
{}

Matrix3x3 Matrix3x3::Zero()
{
    return Matrix3x3{
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
    };
}

Matrix3x3 Matrix3x3::Identity()
{
    return Matrix3x3{
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };
}

Matrix3x3 Matrix3x3::Translate(const Vector2& v)
{
    return Matrix3x3{
        1.0, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        v.x, v.y, 1.0f
    };
}

Matrix3x3 Matrix3x3::Rotate(float deg)
{
    return Matrix3x3{
        Mathf::Cos(deg), Mathf::Sin(deg), 0.0f,
        -Mathf::Sin(deg), Mathf::Cos(deg), 0.0f,
        0.0f, 0.0f, 1.0f
    };
}

Matrix3x3 Matrix3x3::Scale(const Vector2& scalar)
{
    return Matrix3x3{
        scalar.x, 0.0f, 0.0f,
        0.0f, scalar.y, 0.0f,
        0.0f, 0.0f, 1.0f
    };
}

Matrix3x3 Matrix3x3::Scale(float x, float y)
{
    return Matrix3x3{
        x, 0.0f, 0.0f,
        0.0f, y, 0.0f,
        0.0f, 0.0f, 1.0f
    };
}

void Matrix3x3::Set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
    this->m11 = m11; this->m12 = m12; this->m13 = m13;
    this->m11 = m21; this->m22 = m22; this->m23 = m23;
    this->m11 = m31; this->m32 = m32; this->m33 = m33;
}

void Matrix3x3::Set(const Matrix3x3& m)
{
    m11 = m.m11; m12 = m.m12; m13 = m.m13;
    m21 = m.m21; m22 = m.m22; m23 = m.m23;
    m31 = m.m31; m32 = m.m32; m33 = m.m33;
}

void Matrix3x3::SetIdentity()
{
    Set(Matrix3x3::Identity());
}

std::string Matrix3x3::ToString() const
{
    std::ostringstream ss;
    ss << m11 << ", " << m12 << ", " << m13 << "\n";
    ss << m21 << ", " << m22 << ", " << m23 << "\n";
    ss << m31 << ", " << m32 << ", " << m33 << "\n";
    return ss.str();
}

std::array<float, 3> Matrix3x3::operator[](const size_t index) const
{
    return matrix[index];
}

std::array<float, 3>& Matrix3x3::operator[](const size_t index)
{
    return matrix[index];
}

Matrix3x3 operator+=(Matrix3x3& m1, const Matrix3x3& m2)
{
    m1[0][0] += m2[0][0];
    m1[0][1] += m2[0][1];
    m1[0][2] += m2[0][2];

    m1[1][0] += m2[1][0];
    m1[1][1] += m2[1][1];
    m1[1][2] += m2[1][2];

    m1[2][0] += m2[2][0];
    m1[2][1] += m2[2][1];
    m1[2][2] += m2[2][2];

    return m1;
}

Matrix3x3 operator-=(Matrix3x3& m1, const Matrix3x3& m2)
{
    m1[0][0] -= m2[0][0];
    m1[0][1] -= m2[0][1];
    m1[0][2] -= m2[0][2];

    m1[1][0] -= m2[1][0];
    m1[1][1] -= m2[1][1];
    m1[1][2] -= m2[1][2];

    m1[2][0] -= m2[2][0];
    m1[2][1] -= m2[2][1];
    m1[2][2] -= m2[2][2];

    return m2;
}

Matrix3x3 operator*=(Matrix3x3& m1, const Matrix3x3& m2)
{
    Matrix3x3 result{};
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 3; ++k)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    m1 = result;
    return m1;
}

Matrix3x3 operator*=(Matrix3x3& m, float scalar)
{
    m.m11 *= scalar;
    m.m12 *= scalar;
    m.m13 *= scalar;

    m.m21 *= scalar;
    m.m22 *= scalar;
    m.m23 *= scalar;

    m.m31 *= scalar;
    m.m32 *= scalar;
    m.m33 *= scalar;

    return m;
}

Matrix3x3 operator/=(Matrix3x3& m, float scalar)
{
    float invScaler = 1.0f / scalar;
    m.m11 *= invScaler;
    m.m12 *= invScaler;
    m.m13 *= invScaler;

    m.m21 *= invScaler;
    m.m22 *= invScaler;
    m.m23 *= invScaler;

    m.m31 *= invScaler;
    m.m32 *= invScaler;
    m.m33 *= invScaler;

    return m;
}

Matrix3x3 operator+(const Matrix3x3& m1, const Matrix3x3& m2)
{
    Matrix3x3 result{};
    result.m11 = m1.m11 + m2.m11;
    result.m12 = m1.m12 + m2.m12;
    result.m13 = m1.m13 + m2.m13;

    result.m21 = m1.m21 + m2.m21;
    result.m22 = m1.m22 + m2.m22;
    result.m23 = m1.m23 + m2.m23;

    result.m31 = m1.m31 + m2.m31;
    result.m32 = m1.m32 + m2.m32;
    result.m33 = m1.m33 + m2.m33;

    return result;
}

Matrix3x3 operator-(const Matrix3x3& m1, const Matrix3x3& m2)
{
    Matrix3x3 result{};
    result.m11 = m1.m11 - m2.m11;
    result.m12 = m1.m12 - m2.m12;
    result.m13 = m1.m13 - m2.m13;

    result.m21 = m1.m21 - m2.m21;
    result.m22 = m1.m22 - m2.m22;
    result.m23 = m1.m23 - m2.m23;

    result.m31 = m1.m31 - m2.m31;
    result.m32 = m1.m32 - m2.m32;
    result.m33 = m1.m33 - m2.m33;

    return result;
}

Matrix3x3 operator*(const Matrix3x3& m, float scalar)
{
    Matrix3x3 result{};
    result.m11 = m.m11 * scalar;
    result.m12 = m.m12 * scalar;
    result.m13 = m.m13 * scalar;

    result.m21 = m.m21 * scalar;
    result.m22 = m.m22 * scalar;
    result.m23 = m.m23 * scalar;

    result.m31 = m.m31 * scalar;
    result.m32 = m.m32 * scalar;
    result.m33 = m.m33 * scalar;

    return result;

}

Matrix3x3 operator*(float scalar, const Matrix3x3& m)
{
    Matrix3x3 result{};
    result.m11 = m.m11 * scalar;
    result.m12 = m.m12 * scalar;
    result.m13 = m.m13 * scalar;

    result.m21 = m.m21 * scalar;
    result.m22 = m.m22 * scalar;
    result.m23 = m.m23 * scalar;

    result.m31 = m.m31 * scalar;
    result.m32 = m.m32 * scalar;
    result.m33 = m.m33 * scalar;

    return result;
}

Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2)
{
    Matrix3x3 result{};
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 3; ++k)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return result;
}

Matrix3x3 operator/(const Matrix3x3& m, float scalar)
{
    Matrix3x3 result{};
    float invScalar = 1.0f / scalar;
    result.m11 = m.m11 * invScalar;
    result.m12 = m.m12 * invScalar;
    result.m13 = m.m13 * invScalar;

    result.m21 = m.m21 * invScalar;
    result.m22 = m.m22 * invScalar;
    result.m23 = m.m23 * invScalar;

    result.m31 = m.m31 * invScalar;
    result.m32 = m.m32 * invScalar;
    result.m33 = m.m33 * invScalar;

    return result;
}

bool operator==(const Matrix3x3& m1, const Matrix3x3& m2)
{
    return Mathf::Abs(m1.m11 - m2.m11) < Mathf::EPSILON &&
        Mathf::Abs(m1.m12 - m2.m12) < Mathf::EPSILON &&
        Mathf::Abs(m1.m13 - m2.m13) < Mathf::EPSILON &&
        Mathf::Abs(m1.m21 - m2.m21) < Mathf::EPSILON &&
        Mathf::Abs(m1.m22 - m2.m22) < Mathf::EPSILON &&
        Mathf::Abs(m1.m23 - m2.m23) < Mathf::EPSILON &&
        Mathf::Abs(m1.m31 - m2.m31) < Mathf::EPSILON &&
        Mathf::Abs(m1.m32 - m2.m32) < Mathf::EPSILON &&
        Mathf::Abs(m1.m33 - m2.m33) < Mathf::EPSILON;
}

bool operator!=(const Matrix3x3& m1, const Matrix3x3& m2)
{
    return !(m1 == m2);
}

std::ostream& operator<<(std::ostream& stream, const Matrix3x3& m)
{
    return stream << m.ToString();
}
