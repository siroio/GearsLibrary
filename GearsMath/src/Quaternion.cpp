#include "Quaternion.h"
#include "Vector3.h"
#include "Mathf.h"

Quaternion::Quaternion(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w }
{}

Quaternion Quaternion::Identity()
{
    return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
}

float Quaternion::Angle(const Quaternion& q1, const Quaternion& q2)
{
    float dot = Dot(q1, q2);
    if (dot > 1.0f - Mathf::Epsilon) return 0.0f;

    return Mathf::Acos(Mathf::Clamp(dot, -1.0f, 1.0f)) * 2.0f;
}

Quaternion Quaternion::AngleAxis(float deg, const Vector3& axis)
{
    float rad = deg * Mathf::Deg2Rad;
    float len = Mathf::Inverse(axis.Magnitude());
    auto norm = axis.Normalized();

    const float r = rad / 2.0f;
    float s = Mathf::Sin(r);
    float c = Mathf::Cos(r);

    return Quaternion{ deg * (axis.x * len), deg * (axis.y * len), deg * (axis.z * len), c };
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
    return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
    float mag = Mathf::Sqrt(Dot(q, q));
    float magInv = Mathf::Inverse(mag);
    if (mag < Mathf::Epsilon)
    {
        return Identity();
    }

    return Quaternion{ q.x * magInv, q.y * magInv, q.z * magInv, q.w * magInv };
}

Quaternion Quaternion::Euler(float x, float y, float z)
{
    auto v = Vector3{ x / 2.0f, y / 2.0f, z / 2.0f };
    float xSin = Mathf::Sin(v.x);
    float xCos = Mathf::Cos(v.x);
    float ySin = Mathf::Sin(v.y);
    float yCos = Mathf::Cos(v.y);
    float zSin = Mathf::Sin(v.z);
    float zCos = Mathf::Cos(v.z);

    float resX = xSin * yCos * zCos + xCos * ySin * zSin;
    float resY = -xSin * yCos * zSin + xCos * ySin * zCos;
    float resZ = -xSin * ySin * zCos + xCos * yCos * zSin;
    float resW = xSin * ySin * zSin + xCos * yCos * zCos;

    return { resX, resY, resZ, resW };
}

Quaternion Quaternion::Euler(const Vector3& euler)
{
    return Euler(euler.x, euler.y, euler.z);
}

Quaternion Quaternion::Inverse(const Quaternion& q)
{
    return Quaternion{ -q.x, -q.y, -q.z, q.w };
}

Quaternion Quaternion::Serp(const Quaternion& a, const Quaternion& b, float t)
{

}

Quaternion Quaternion::SerpUnclamped(const Quaternion& a, const Quaternion& b, float t)
{
    float dot = Dot(a, b);
    float sign = 1.0f;
    if (dot < 0.0f)
    {
        dot = -dot;
        sign = -1.0f;
    }

    float t0 = 1.0f - t;
    float t1 = t;

    if (dot < 1.0f - Mathf::Epsilon)
    {
        float theta = Mathf::Acos(dot);
        t0 = Mathf::Sin(theta * t0) / Mathf::Sin(theta);
        t1 = Mathf::Sin(theta * t1) / Mathf::Sin(theta);
    }

    return ((a * t0) + (b * sign * t1)).Normalized();
}

Quaternion Quaternion::Normalized() const
{
    return Normalize(*this);
}

Quaternion operator-(const Quaternion& q)
{
    return Quaternion::Inverse(q);
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion{ q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w };
}

Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion{ q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w };
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{

    // TODO : expression wip
    float x = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    float y = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    float z = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    float w = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
}

Quaternion operator*(const Quaternion& q1, float scale)
{
    return Quaternion();
}

Quaternion operator*(float scale, const Quaternion& q1)
{
    return Quaternion();
}

Quaternion& operator*=(Quaternion& q1, const Quaternion& q2)
{
    // TODO: return ステートメントをここに挿入します
}

Quaternion& operator*=(Quaternion& q1, float scale)
{
    // TODO: return ステートメントをここに挿入します
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
{
    // TODO: return ステートメントをここに挿入します
}
