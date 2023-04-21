#include "Quaternion.h"
#include "Vector3.h"
#include "Mathf.h"
#include <sstream>

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
    return Mathf::Acos(Mathf::Clamp(dot, -1.0f, 1.0f)) * 2.0f * Mathf::Rad2Deg;
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
    return SerpUnclamped(a, b, Mathf::Clamp01(t));
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

void Quaternion::Set(const Quaternion& q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

void Quaternion::Set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::Set(float xyzw)
{
    this->x = xyzw;
    this->y = xyzw;
    this->z = xyzw;
    this->w = xyzw;
}

void Quaternion::SetIdentity()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->w = 1.0f;
}

Quaternion Quaternion::Normalized() const
{
    return Normalize(*this);
}

std::string Quaternion::ToString() const
{
    std::stringstream ss;
    ss << this->x << ", ";
    ss << this->y << ", ";
    ss << this->z << ", ";
    ss << this->w;
    return ss.str();
}

void Quaternion::operator=(const Quaternion& v)
{
    Set(v);
}

float& Quaternion::operator[](const size_t index)
{
    if (index > 3)
    {
        throw std::out_of_range("Index is out of range");
    }

    return q[index];
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
    float x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    float y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
    float z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;
    float w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

    return Quaternion{ x, y, z, w };
}

Quaternion operator*(const Quaternion& q1, float scale)
{
    return Quaternion{ q1.x * scale, q1.y * scale, q1.z * scale, q1.w * scale };
}

Quaternion operator*(float scale, const Quaternion& q1)
{
    return q1 * scale;
}

Quaternion& operator*=(Quaternion& q1, const Quaternion& q2)
{
    float x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    float y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
    float z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;
    float w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

    q1.x = x;
    q1.y = y;
    q1.z = z;
    q1.w = w;

    return q1;
}

Quaternion& operator*=(Quaternion& q1, float scale)
{
    q1.x *= scale;
    q1.y *= scale;
    q1.z *= scale;
    q1.w *= scale;

    return q1;
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
{
    return stream << q.ToString();
}
