#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Mathf.h"
#include "Debugging.h"

Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44)
    : m11{ m11 }, m12{ m12 }, m13{ m13 }, m14{ m14 },
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

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
    return Matrix4x4
    {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
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
    auto zaxis = (target - position).Normalized();
    auto xaxis = Vector3::Cross(up, zaxis).Normalized();
    auto yaxis = Vector3::Cross(zaxis, xaxis);

    return Matrix4x4
    {
        xaxis.x, yaxis.x, zaxis.x, 0.0f,
        xaxis.y, yaxis.y, zaxis.y, 0.0f,
        xaxis.z, yaxis.z, zaxis.z, 0.0f,
        -Vector3::Dot(xaxis, position), -Vector3::Dot(yaxis, position), -Vector3::Dot(zaxis, position), 1.0f
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
    float x = q.x * 2.0f;
    float y = q.y * 2.0f;
    float z = q.z * 2.0f;

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

Matrix4x4 Matrix4x4::RotationX(float deg)
{
    float rad = deg * Mathf::Deg2Rad;
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
    float rad = deg * Mathf::Deg2Rad;
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
    float rad = deg * Mathf::Deg2Rad;
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

Matrix4x4 Matrix4x4::TRS(const Vector3& p, const Quaternion& q, const Vector3& s)
{
    Matrix4x4 m = Rotate(q);
    m.m11 *= s.x; m.m12 *= s.x; m.m13 *= s.x;
    m.m21 *= s.y; m.m22 *= s.y; m.m23 *= s.y;
    m.m31 *= s.z; m.m32 *= s.z; m.m33 *= s.z;
    m.m41 = p.x;  m.m42 = p.y;  m.m43 = p.z;

    return m;
}

Matrix4x4 Matrix4x4::Perspective(float width, float height, float nearDistance, float farDistance)
{
    Debug_Assert(nearDistance <= 0.0f);
    Debug_Assert(farDistance <= 0.0f);
    Debug_Assert(nearDistance >= farDistance);

    float Depth = nearDistance - farDistance;

    float m11 = 2.0f * nearDistance / width;
    float m22 = 2.0f * nearDistance / height;
    float m33 = farDistance / Depth;
    float m43 = nearDistance * farDistance / Depth;

    return Matrix4x4
    {
        m11, 0.0f, 0.0f, 0.0f,
        0.0f, m22, 0.0f, 0.0f,
        0.0f, 0.0f, m33, -1.0f,
        0.0f, 0.0f, m43, 0.0f
    };
}

Matrix4x4 Matrix4x4::PerspectiveFOV(float fieldOfView, float aspectRatio, float nearDistance, float farDistance)
{
    Debug_Assert(fieldOfView <= 0.0f || fieldOfView >= Mathf::PI);
    Debug_Assert(nearDistance <= 0.0f);
    Debug_Assert(farDistance <= 0.0f);
    Debug_Assert(nearDistance >= farDistance);

    float yScale = 1.0f / Mathf::Tan(fieldOfView * 0.5f * Mathf::Deg2Rad);
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
    float invDet = 1.0f / det;
    return Matrix4x4
    {
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

void Matrix4x4::SetTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{}

std::string Matrix4x4::ToString()
{
    return std::string();
}
