#include <Vector2.h>
#include <Vector3.h>
#include <Debugging.h>
#include <Mathf.h>
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
    return { 1.0f, 0.0f };
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Angle(const Vector2& from, const Vector2& to)
{
    float deno = Mathf::Sqrt(from.SqrMagnitude() * to.SqrMagnitude());

    if (deno < Mathf::EpsilonNormalSqrt)
    {
        return 0.0f;
    }

    float dot = Mathf::Clamp(Dot(from, to) / deno, -1.0f, 1.0f);

    return Mathf::Acos(dot) * Mathf::Rad2Deg;
}

float Vector2::SignedAngle(const Vector2& from, const Vector2& to)
{
    float unsigned_angle = Angle(from, to);
    float sign = Mathf::Sign(from.x * to.y - from.y * to.x);

    return unsigned_angle * sign;
}

float Vector2::Distance(const Vector2& a, const Vector2& b)
{
    float diffX = a.x - b.x;
    float diffY = a.y - b.y;

    return Mathf::Sqrt(diffX * diffX + diffY * diffY);
}

float Vector2::SqrMagnitude(const Vector2& vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

float Vector2::Magnitude(const Vector2& vector)
{
    return Mathf::Sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vector2 Vector2::Min(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2{ Mathf::Min(lhs.x, rhs.x), Mathf::Min(lhs.y, rhs.y) };
}

Vector2 Vector2::Max(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2{ Mathf::Max(lhs.x, rhs.x), Mathf::Max(lhs.y, rhs.y) };
}

Vector2 Vector2::Scale(const Vector2& a, const Vector2& b)
{
    return Vector2{ a.x * b.x, a.y * b.y };
}

Vector2 Vector2::Reflect(const Vector2& inDirection, const Vector2& inNormal)
{
    float factor = -2.0f * Dot(inNormal, inDirection);

    return Vector2{ factor * inNormal.x + inDirection.x, factor * inNormal.y + inDirection.y };
}

Vector2 Vector2::Perpendicular(const Vector2& inDirection)
{
    return Vector2{ -inDirection.y, inDirection.x };
}

Vector2 Vector2::Direction(const Vector2& from, const Vector2& to)
{
    Vector2 direction = Vector2{ to.x - from.x, to.y - from.y };
    direction.Normalize();

    return direction;
}

Vector2 Vector2::Normalize(const Vector2& v)
{
    float mag = Magnitude(v);
    if (mag > Mathf::Epsilon)
    {
        return v / mag;
    }
    else
    {
        return Zero();
    }
}

Vector2 Vector2::ClampMagnitude(const Vector2& vector, float maxLength)
{
    float sqrMagnitude = vector.SqrMagnitude();

    if (sqrMagnitude > maxLength * maxLength)
    {
        float mag = Mathf::Sqrt(sqrMagnitude);
        float normalizedX = vector.x / mag;
        float normalizedY = vector.y / mag;
        return Vector2{ normalizedX * maxLength, normalizedY * maxLength };
    }

    return vector;
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
{
    t = Mathf::Clamp01(t);

    return Vector2{ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}

Vector2 Vector2::LerpUnclamped(const Vector2& a, const Vector2& b, float t)
{
    return Vector2{ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}

Vector2 Vector2::MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta)
{
    float toVectorX = target.x - current.x;
    float toVectorY = target.y - current.y;

    float sqrDist = toVectorX * toVectorX + toVectorY * toVectorY;

    if (sqrDist == 0 || (maxDistanceDelta >= 0 && sqrDist <= maxDistanceDelta * maxDistanceDelta))
    {
        return target;
    }

    float dist = Mathf::Sqrt(sqrDist);

    return Vector2{
        current.x + toVectorX / dist * maxDistanceDelta,
        current.y + toVectorY / dist * maxDistanceDelta
    };
}

void Vector2::Set(const Vector2& v)
{
    x = v.x;
    y = v.y;
}

void Vector2::Set(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vector2::Set(float xy)
{
    x = y = xy;
}

void Vector2::Set0()
{
    Set(0.0f);
}

void Vector2::Scale(const Vector2& scale)
{
    x *= scale.x;
    y *= scale.y;
}

void Vector2::Normalize()
{
    float len = Magnitude();
    x /= len;
    y /= len;
}

float Vector2::SqrMagnitude() const
{
    return x * x + y * y;
}

float Vector2::Magnitude() const
{
    return Mathf::Sqrt(SqrMagnitude());
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

Vector2 operator-(const Vector2& v)
{
    return Vector2{ -v.x, -v.y };
}

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
    return Vector2{ v1.x + v2.x, v1.y + v2.y };
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
    return Vector2{ v1.x - v2.x, v1.y - v2.y };
}

Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Vector2{ v1.x * v2.x, v1.y * v2.y };
}

Vector2 operator*(const Vector2& v1, float scale)
{
    return Vector2{ v1.x * scale, v1.y * scale };
}

Vector2 operator*(float scale, const Vector2& v1)
{
    return Vector2{ v1.x * scale, v1.y * scale };
}

Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    Debug_Assert(v2.x != 0.0f);
    Debug_Assert(v2.y != 0.0f);

    return Vector2{ v1.x / v2.x, v1.x / v2.x };
}

Vector2 operator/(const Vector2& v1, float scale)
{
    Debug_Assert(scale != 0.0f);
    float m = 1.0f / scale;
    return Vector2{ v1.x * m, v1.x * m };
}

Vector2& operator+=(Vector2& v1, const Vector2& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

Vector2& operator-=(Vector2& v1, const Vector2& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

Vector2& operator*=(Vector2& v1, const Vector2& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;

    return v1;
}

Vector2& operator*=(Vector2& v1, float scale)
{
    v1.x -= scale;
    v1.y -= scale;

    return v1;
}

Vector2& operator/=(Vector2& v1, const Vector2& v2)
{
    Debug_Assert(v2.x != 0.0f);
    Debug_Assert(v2.y != 0.0f);

    v1.x /= v2.x;
    v1.y /= v2.y;

    return v1;
}

Vector2& operator/=(Vector2& v1, float scale)
{
    Debug_Assert(scale != 0.0f);

    float m = 1.0f / scale;
    v1.x *= m;
    v1.y += m;

    return v1;
}

bool operator==(const Vector2& v1, const Vector2 v2)
{
    float diff_x = v1.x - v2.x;
    float diff_y = v1.y - v2.y;
    float sqrMag = diff_x * diff_x + diff_y * diff_y;

    return sqrMag < (Mathf::Epsilon * Mathf::Epsilon);
}

bool operator!=(const Vector2& v1, const Vector2 v2)
{
    return !(v1 == v2);
}

std::ostream& operator<<(std::ostream& stream, const Vector2& v)
{
    return stream << v.ToString();
}
