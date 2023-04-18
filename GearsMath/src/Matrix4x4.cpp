#include "Matrix4x4.h"
#include "Vector3.h"
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
    auto zaxis = Vector3::Normalize(target - position);
    auto xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
    auto yaxis = Vector3::Cross(zaxis, xaxis);

    return Matrix4x4
    {
        xaxis.x, yaxis.x, zaxis.x, 0.0f,
        xaxis.y, yaxis.y, zaxis.y, 0.0f,
        xaxis.z, yaxis.z, zaxis.z, 0.0f,
        -Vector3::Dot(xaxis, position), -Vector3::Dot(yaxis, position), -Vector3::Dot(zaxis, position), 1.0f
    };
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

    float yScale = 1.0f / Mathf::Tan(fieldOfView * 0.5f);
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

Matrix4x4 Matrix4x4::Rotate(const Quaternion& q)
{
    return Matrix4x4::Identity();
}

Matrix4x4 Matrix4x4::RotationX(float deg)
{
    return Matrix4x4::Identity();
}

Matrix4x4 Matrix4x4::RotationY(float deg)
{
    return Matrix4x4::Identity();
}

Matrix4x4 Matrix4x4::RotationZ(float deg)
{
    return Matrix4x4::Identity();
}

std::string Matrix4x4::ToString()
{
    return std::string();
}
