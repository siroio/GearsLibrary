#include <Matrix4x4.h>
#include <Quaternion.h>
#include <Vector3.h>
#include <Mathf.h>
#include <sstream>

Matrix4x4::Matrix4x4() : matrix{ 0.0f }
{}

Matrix4x4::Matrix4x4(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44) :
    m11{ m11 }, m12{ m12 }, m13{ m13 }, m14{ m14 },
    m21{ m21 }, m22{ m22 }, m23{ m23 }, m24{ m24 },
    m31{ m31 }, m32{ m32 }, m33{ m33 }, m34{ m34 },
    m41{ m41 }, m42{ m42 }, m43{ m43 }, m44{ m44 }
{}

Matrix4x4 Matrix4x4::Zero()
{
    return Matrix4x4
    {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
}

Matrix4x4 Matrix4x4::Identity()
{
    return Matrix4x4
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::Translate(const Vector3& v)
{
    return Matrix4x4
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
         v.x,  v.y,  v.z, 1.0f
    };
}

Matrix4x4 Matrix4x4::Rotate(const Quaternion& q)
{
    float x = q.x * 2.0F;
    float y = q.y * 2.0F;
    float z = q.z * 2.0F;
    float xx = q.x * x;
    float yy = q.y * y;
    float zz = q.z * z;
    float xy = q.x * y;
    float xz = q.x * z;
    float yz = q.y * z;
    float wx = q.w * x;
    float wy = q.w * y;
    float wz = q.w * z;

    return Matrix4x4
    {
        1.0f - yy - zz, xy + wz, xz - wy, 0.0f,
        xy - wz, 1.0f - xx - zz, yz + wx, 0.0f,
        xz + wy, yz - wx, 1.0f - xx - yy, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scalar)
{
    return Matrix4x4
    {
        scalar.x, 0.0f, 0.0f, 0.0f,
        0.0f, scalar.y, 0.0f, 0.0f,
        0.0f, 0.0f, scalar.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::Scale(float x, float y, float z)
{
    return Matrix4x4
    {
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
    if (position == target) return Matrix4x4::Identity();

    auto zaxis = (target - position).Normalized();
    if (zaxis.SqrMagnitude() < Mathf::EPSILON) return Matrix4x4::Identity();

    auto xaxis = Vector3::Cross(up, zaxis).Normalized();
    auto yaxis = Vector3::Cross(zaxis, xaxis).Normalized();

    return Matrix4x4
    {
        xaxis.x, yaxis.x, zaxis.x, 0.0f,
        xaxis.y, yaxis.y, zaxis.y, 0.0f,
        xaxis.z, yaxis.z, zaxis.z, 0.0f,
        -Vector3::Dot(xaxis, position), -Vector3::Dot(yaxis, position), -Vector3::Dot(zaxis, position), 1.0f
    };
}

Matrix4x4 Matrix4x4::RotationX(float deg)
{
    float rad = deg * Mathf::DEG2RAD;
    float cos = Mathf::Cos(rad);
    float sin = Mathf::Sin(rad);

    return Matrix4x4
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  cos, sin, 0.0f,
        0.0f, -sin, cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::RotationY(float deg)
{
    float rad = deg * Mathf::DEG2RAD;
    float cos = Mathf::Cos(rad);
    float sin = Mathf::Sin(rad);

    return Matrix4x4
    {
         cos, 0.0f, -sin, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
         sin, 0.0f,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::RotationZ(float deg)
{
    float rad = deg * Mathf::DEG2RAD;
    float cos = Mathf::Cos(rad);
    float sin = Mathf::Sin(rad);

    return Matrix4x4
    {
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4x4 Matrix4x4::TRS(const Vector3& pos, const Quaternion& q, const Vector3& s)
{
    Matrix4x4 m = Rotate(q);
    m.m11 *= s.x; m.m12 *= s.x; m.m13 *= s.x;
    m.m21 *= s.y; m.m22 *= s.y; m.m23 *= s.y;
    m.m31 *= s.z; m.m32 *= s.z; m.m33 *= s.z;
    m.m41 = pos.x;  m.m42 = pos.y;  m.m43 = pos.z;

    return m;
}

Matrix4x4 Matrix4x4::Perspective(float fieldOfView, float aspectRatio, float nearDistance, float farDistance)
{
    float yScale = 1.0f / Mathf::Tan(fieldOfView * Mathf::DEG2RAD * 0.5f);
    float xScale = yScale / aspectRatio;

    return Matrix4x4
    {
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, farDistance / (farDistance - nearDistance), 1.0f,
        0.0f, 0.0f, (-nearDistance * farDistance) / (farDistance - nearDistance), 0.0f
    };
}

Matrix4x4 Matrix4x4::Orthographic(float width, float height, float nearZ, float farZ)
{
    float m11 = 2.0f / width;
    float m22 = 2.0f / height;
    float m33 = 1.0f / (farZ - nearZ);
    float m43 = nearZ / (nearZ - farZ);

    return Matrix4x4
    {
        m11, 0.0f, 0.0f, 0.0f,
        0.0f, m22, 0.0f, 0.0f,
        0.0f, 0.0f, m33, 0.0f,
        0.0f, 0.0f, m43, 1.0f
    };
}

Vector3 Matrix4x4::MultiplyPoint(const Vector3& point) const
{
    float x = m11 * point.x + m12 * point.y + m13 * point.z + m14;
    float y = m21 * point.x + m22 * point.y + m23 * point.z + m24;
    float z = m31 * point.x + m32 * point.y + m33 * point.z + m34;
    float w = m41 * point.x + m41 * point.y + m43 * point.z + m44;

    w = 1.0f / w;
    x *= w;
    y *= y;
    z *= w;

    return Vector3{ x, y ,z };
}

Vector3 Matrix4x4::MultiplyPoint4x3(const Vector3& point) const
{
    float x = m11 * point.x + m12 * point.y + m13 * point.z + m14;
    float y = m21 * point.x + m22 * point.y + m23 * point.z + m24;
    float z = m31 * point.x + m32 * point.y + m33 * point.z + m34;

    return Vector3{ x, y ,z };
}

Vector3 Matrix4x4::MultiplyVector(const Vector3& vector) const
{
    float x = m11 * vector.x + m12 * vector.y + m13 * vector.z;
    float y = m21 * vector.x + m22 * vector.y + m23 * vector.z;
    float z = m31 * vector.x + m32 * vector.y + m33 * vector.z;

    return Vector3{ x, y ,z };
}

Vector3 Matrix4x4::Scale() const
{
    float sx = Vector3{ m11, m12, m13, }.Magnitude();
    float sy = Vector3{ m21, m22, m23, }.Magnitude();
    float sz = Vector3{ m31, m32, m33, }.Magnitude();

    return Vector3{ sx, sy, sz };
}

Vector3 Matrix4x4::Position() const
{
    return Vector3{ m41, m42, m43 };
}

Quaternion Matrix4x4::Rotation() const
{
    Quaternion result = Quaternion::Identity();
    float tr = m11 + m22 + m33 + 1.0f;
    if (tr >= 1.0f)
    {
        float fourD = 2.0f + Mathf::Sqrt(tr);
        result.x = (m23 - m32) / fourD;
        result.y = (m31 - m13) / fourD;
        result.z = (m12 - m21) / fourD;
        result.w = fourD / 4.0f;
        return result;
    }

    int i = 0;
    if (m11 <= m22)
    {
        i = 1;
    }
    if (matrix[2][2] > matrix[i][i])
    {
        i = 2;
    }
    int j = (i + 1) % 3;
    int k = (j + 1) % 3;
    tr = matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f;
    float fourD = 2.0f * Mathf::Sqrt(tr);
    result[i] = fourD / 4.0f;
    result[j] = (matrix[j][i] + matrix[i][j]) / fourD;
    result[k] = (matrix[k][i] + matrix[i][k]) / fourD;
    result[3] = (matrix[j][k] - matrix[k][j]) / fourD;
    return result;
}

Vector3 Matrix4x4::Forward() const
{
    return Vector3{ m31, m32, m33 }.Normalized();
}

Vector3 Matrix4x4::Up() const
{
    return Vector3{ m21, m22, m23 }.Normalized();
}

Vector3 Matrix4x4::Right() const
{
    return Vector3{ m11, m12, m13 }.Normalized();
}

Matrix4x4 Matrix4x4::Inverse() const
{
    float a0 = m11 * m22 - m12 * m21;
    float a1 = m11 * m23 - m13 * m21;
    float a2 = m11 * m24 - m14 * m21;
    float a3 = m12 * m23 - m13 * m22;
    float a4 = m12 * m24 - m14 * m22;
    float a5 = m13 * m24 - m14 * m23;
    float b0 = m31 * m42 - m32 * m41;
    float b1 = m31 * m43 - m33 * m41;
    float b2 = m31 * m44 - m34 * m41;
    float b3 = m32 * m43 - m33 * m42;
    float b4 = m32 * m44 - m34 * m42;
    float b5 = m33 * m44 - m34 * m43;
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
    if (det == 0.0f)
    {
        return *this;
    }
    const float invDet = Mathf::Inverse(det);
    return Matrix4x4{
        (m22 * b5 - m23 * b4 + m24 * b3) * invDet,
        (-m12 * b5 + m13 * b4 - m14 * b3) * invDet,
        (m42 * a5 - m43 * a4 + m44 * a3) * invDet,
        (-m32 * a5 + m33 * a4 - m34 * a3) * invDet,
        (-m21 * b5 + m23 * b2 - m24 * b1) * invDet,
        (m11 * b5 - m13 * b2 + m14 * b1) * invDet,
        (-m41 * a5 + m43 * a2 - m44 * a1) * invDet,
        (m31 * a5 - m33 * a2 + m34 * a1) * invDet,
        (m21 * b4 - m22 * b2 + m24 * b0) * invDet,
        (-m11 * b4 + m12 * b2 - m14 * b0) * invDet,
        (m41 * a4 - m42 * a2 + m44 * a0) * invDet,
        (-m31 * a4 + m32 * a2 - m34 * a0) * invDet,
        (-m21 * b3 + m22 * b1 - m23 * b0) * invDet,
        (m11 * b3 - m12 * b1 + m13 * b0) * invDet,
        (-m41 * a3 + m42 * a1 - m43 * a0) * invDet,
        (m31 * a3 - m32 * a1 + m33 * a0) * invDet
    };
}

Matrix4x4 Matrix4x4::Transpose() const
{
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.matrix[i][j] = matrix[j][i];
        }
    }
    return result;
}

void Matrix4x4::Set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    this->m11 = m11; this->m12 = m12; this->m13 = m13; this->m14 = m14;
    this->m21 = m11; this->m22 = m12; this->m23 = m13; this->m24 = m14;
    this->m31 = m11; this->m32 = m12; this->m33 = m13; this->m34 = m14;
    this->m41 = m11; this->m42 = m12; this->m43 = m13; this->m44 = m14;
}

void Matrix4x4::Set(const Matrix4x4& m)
{
    std::copy(m.matrix.begin(), m.matrix.end(), matrix.begin());
}

void Matrix4x4::SetIdentity()
{
    *this = Identity();
}

void Matrix4x4::SetTRS(const Vector3& translate, const Quaternion& rotation, const Vector3& scalar)
{
    *this = TRS(translate, rotation, scalar);
}

std::string Matrix4x4::ToString() const
{
    std::ostringstream ss;
    ss << m11 << ", " << m12 << ", " << m13 << ", " << m14 << "\n";
    ss << m21 << ", " << m22 << ", " << m23 << ", " << m24 << "\n";
    ss << m31 << ", " << m32 << ", " << m33 << ", " << m34 << "\n";
    ss << m41 << ", " << m42 << ", " << m43 << ", " << m44;
    return ss.str();
}

std::array<float, 4> Matrix4x4::operator[](const size_t index) const
{
    return matrix[index];
}

std::array<float, 4>& Matrix4x4::operator[](const size_t index)
{
    return matrix[index];
}

Matrix4x4 operator+=(Matrix4x4& m1, const Matrix4x4& m2)
{
    m1.m11 += m2.m11;
    m1.m12 += m2.m12;
    m1.m13 += m2.m13;
    m1.m14 += m2.m14;

    m1.m21 += m2.m21;
    m1.m22 += m2.m22;
    m1.m23 += m2.m23;
    m1.m24 += m2.m24;

    m1.m31 += m2.m31;
    m1.m32 += m2.m32;
    m1.m33 += m2.m33;
    m1.m34 += m2.m34;

    m1.m41 += m2.m41;
    m1.m42 += m2.m42;
    m1.m43 += m2.m43;
    m1.m44 += m2.m44;

    return m1;
}

Matrix4x4 operator-=(Matrix4x4& m1, const Matrix4x4& m2)
{
    m1.m11 -= m2.m11;
    m1.m12 -= m2.m12;
    m1.m13 -= m2.m13;
    m1.m14 -= m2.m14;

    m1.m21 -= m2.m21;
    m1.m22 -= m2.m22;
    m1.m23 -= m2.m23;
    m1.m24 -= m2.m24;

    m1.m31 -= m2.m31;
    m1.m32 -= m2.m32;
    m1.m33 -= m2.m33;
    m1.m34 -= m2.m34;

    m1.m41 -= m2.m41;
    m1.m42 -= m2.m42;
    m1.m43 -= m2.m43;
    m1.m44 -= m2.m44;

    return m1;
}

Matrix4x4 operator*=(Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result{};

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    m1 = result;
    return m1;
}

Matrix4x4 operator*=(Matrix4x4& m, float scalar)
{
    m.m11 *= scalar;
    m.m12 *= scalar;
    m.m13 *= scalar;
    m.m14 *= scalar;

    m.m21 *= scalar;
    m.m22 *= scalar;
    m.m23 *= scalar;
    m.m24 *= scalar;

    m.m31 *= scalar;
    m.m32 *= scalar;
    m.m33 *= scalar;
    m.m34 *= scalar;

    m.m41 *= scalar;
    m.m42 *= scalar;
    m.m43 *= scalar;
    m.m44 *= scalar;

    return m;
}

Matrix4x4 operator/=(Matrix4x4& m, float scalar)
{
    const float scalerInv = Mathf::Inverse(scalar);

    m.m11 *= scalerInv;
    m.m12 *= scalerInv;
    m.m13 *= scalerInv;
    m.m14 *= scalerInv;

    m.m21 *= scalerInv;
    m.m22 *= scalerInv;
    m.m23 *= scalerInv;
    m.m24 *= scalerInv;

    m.m31 *= scalerInv;
    m.m32 *= scalerInv;
    m.m33 *= scalerInv;
    m.m34 *= scalerInv;

    m.m41 *= scalerInv;
    m.m42 *= scalerInv;
    m.m43 *= scalerInv;
    m.m44 *= scalerInv;

    return m;
}

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result{};

    result.m11 = m1.m11 + m2.m11;
    result.m12 = m1.m12 + m2.m12;
    result.m13 = m1.m13 + m2.m13;
    result.m14 = m1.m14 + m2.m14;

    result.m21 = m1.m21 + m2.m21;
    result.m22 = m1.m22 + m2.m22;
    result.m23 = m1.m23 + m2.m23;
    result.m24 = m1.m24 + m2.m24;

    result.m31 = m1.m31 + m2.m31;
    result.m32 = m1.m32 + m2.m32;
    result.m33 = m1.m33 + m2.m33;
    result.m34 = m1.m34 + m2.m34;

    result.m41 = m1.m41 + m2.m41;
    result.m42 = m1.m42 + m2.m42;
    result.m43 = m1.m43 + m2.m43;
    result.m44 = m1.m44 + m2.m44;

    return result;
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result{};

    result.m11 = m1.m11 - m2.m11;
    result.m12 = m1.m12 - m2.m12;
    result.m13 = m1.m13 - m2.m13;
    result.m14 = m1.m14 - m2.m14;

    result.m21 = m1.m21 - m2.m21;
    result.m22 = m1.m22 - m2.m22;
    result.m23 = m1.m23 - m2.m23;
    result.m24 = m1.m24 - m2.m24;

    result.m31 = m1.m31 - m2.m31;
    result.m32 = m1.m32 - m2.m32;
    result.m33 = m1.m33 - m2.m33;
    result.m34 = m1.m34 - m2.m34;

    result.m41 = m1.m41 - m2.m41;
    result.m42 = m1.m42 - m2.m42;
    result.m43 = m1.m43 - m2.m43;
    result.m44 = m1.m44 - m2.m44;

    return result;
}

Matrix4x4 operator*(const Matrix4x4& m, float scalar)
{
    Matrix4x4 result{};

    result.m11 = m.m11 + scalar;
    result.m12 = m.m12 + scalar;
    result.m13 = m.m13 + scalar;
    result.m14 = m.m14 + scalar;

    result.m21 = m.m21 + scalar;
    result.m22 = m.m22 + scalar;
    result.m23 = m.m23 + scalar;
    result.m24 = m.m24 + scalar;

    result.m31 = m.m31 + scalar;
    result.m32 = m.m32 + scalar;
    result.m33 = m.m33 + scalar;
    result.m34 = m.m34 + scalar;

    result.m41 = m.m41 + scalar;
    result.m42 = m.m42 + scalar;
    result.m43 = m.m43 + scalar;
    result.m44 = m.m44 + scalar;

    return result;
}

Matrix4x4 operator*(float scalar, const Matrix4x4& m)
{
    Matrix4x4 result{};

    result.m11 = m.m11 * scalar;
    result.m12 = m.m12 * scalar;
    result.m13 = m.m13 * scalar;
    result.m14 = m.m14 * scalar;

    result.m21 = m.m21 * scalar;
    result.m22 = m.m22 * scalar;
    result.m23 = m.m23 * scalar;
    result.m24 = m.m24 * scalar;

    result.m31 = m.m31 * scalar;
    result.m32 = m.m32 * scalar;
    result.m33 = m.m33 * scalar;
    result.m34 = m.m34 * scalar;

    result.m41 = m.m41 * scalar;
    result.m42 = m.m42 * scalar;
    result.m43 = m.m43 * scalar;
    result.m44 = m.m44 * scalar;

    return result;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                result.matrix[i][j] += m1.matrix[i][k] * m2.matrix[k][j];
            }
        }
    }

    return result;
}

Matrix4x4 operator/(const Matrix4x4& m, float scalar)
{
    Matrix4x4 result{};

    const float scalarInv = Mathf::Inverse(scalar);

    result.m11 = m.m11 * scalarInv;
    result.m12 = m.m12 * scalarInv;
    result.m13 = m.m13 * scalarInv;
    result.m14 = m.m14 * scalarInv;

    result.m21 = m.m21 * scalarInv;
    result.m22 = m.m22 * scalarInv;
    result.m23 = m.m23 * scalarInv;
    result.m24 = m.m24 * scalarInv;

    result.m31 = m.m31 * scalarInv;
    result.m32 = m.m32 * scalarInv;
    result.m33 = m.m33 * scalarInv;
    result.m34 = m.m34 * scalarInv;

    result.m41 = m.m41 * scalarInv;
    result.m42 = m.m42 * scalarInv;
    result.m43 = m.m43 * scalarInv;
    result.m44 = m.m44 * scalarInv;

    return result;
}

std::ostream& operator<<(std::ostream& stream, const Matrix4x4& m)
{
    return stream << m.ToString();
}
