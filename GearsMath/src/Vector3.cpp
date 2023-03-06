#include <Vector2.h>
#include <Vector3.h>
#include <iosfwd>
#include <sstream>

Vector3::Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z }
{}

Vector3::Vector3(float xyz) : x{ xyz }, y{ xyz }, z{ xyz }
{}

Vector3::Vector3(const Vector2& v) : x{ v.x }, y{ v.y }, z{ 0.0f }
{}

Vector3 Vector3::Zero()
{
    return { 0.0f, 0.0f, 0.0f };
}

Vector3 Vector3::One()
{
    return { 1.0f, 1.0f, 1.0f };
}

Vector3 Vector3::Up()
{
    return { 0.0f, 1.0f, 0.0f };
}

Vector3 Vector3::Down()
{
    return { 0.0f, -1.0f, 0.0f };
}

Vector3 Vector3::Left()
{
    return { -1.0f, 0.0f, 0.0f };
}

Vector3 Vector3::Right()
{
    return { 1.0f, 0.0, 0.0f };
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
    return v1.x * v2.x + v1.y * v2.x + v1.z * v2.z;
}

void Vector3::Set(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void Vector3::Set(float v1, float v2, float v3)
{
    x = v1;
    y = v2;
    z = v3;
}

void Vector3::Set(float xyz)
{
    x = y = z = xyz;
}

void Vector3::Set0()
{
    Set(0.0f);
}

float Vector3::SqrMagnitude() const
{
    return x * x + y * y + z * z;
}

float Vector3::Magnitude() const
{
    return sqrtf(SqrMagnitude());
}

std::string Vector3::ToString() const
{
    std::stringstream ss;
    ss << this->x << ", ";
    ss << this->y << ", ";
    ss << this->z;
    return ss.str();
}

void Vector3::operator = (const Vector3& v)
{
    Set(v);
}

std::ostream& operator<<(std::ostream& stream, const Vector3& v)
{
    return stream << v.ToString();
}
