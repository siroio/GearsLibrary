#include <Vector2.h>
#include <Vector3.h>
#include <iosfwd>
#include <sstream>

Vector2::Vector2(float x, float y) : x{ x }, y{ y }
{}

Vector2::Vector2(float xy) : x{ xy }, y{ xy }
{}

Vector2::Vector2(const Vector3& v) : x{ v.x }, y{ v.y }
{}

Vector2 Vector2::Zero()
{
    return { 0.0f, 0.0f };
}

Vector2 Vector2::One()
{
    return { 1.0f, 1.0f };
}

Vector2 Vector2::Up()
{
    return { 0.0f, 1.0f };
}

Vector2 Vector2::Down()
{
    return { 0.0f, -1.0f };
}

Vector2 Vector2::Left()
{
    return { -1.0f, 0.0f };
}

Vector2 Vector2::Right()
{
    return { 1.0f, 0.0 };
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.x + v1.y * v2.x;
}

void Vector2::Set(const Vector2& v)
{
    x = v.x;
    y = v.y;
}

void Vector2::Set(float v1, float v2)
{
    x = v1;
    y = v2;
}

void Vector2::Set(float xy)
{
    x = y = xy;
}

void Vector2::Set0()
{
    Set(0.0f);
}

float Vector2::SqrMagnitude() const
{
    return x * x + y * y;
}

float Vector2::Magnitude() const
{
    return sqrtf(SqrMagnitude());
}

std::string Vector2::ToString() const
{
    std::stringstream ss;
    ss << this->x << ", ";
    ss << this->y;
    return ss.str();
}


void Vector2::operator = (const Vector2& v)
{
    Set(v);
}

std::ostream& operator<<(std::ostream& stream, const Vector2& v)
{
    return stream << v.ToString();
}
