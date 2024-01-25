#include <Vector2.h>
#include <Vector3.h>
#include <Mathf.h>
#include <sstream>

Vector2::Vector2() : xy{ 0.0f }
{}

Vector2::Vector2(float x, float y) : x{ x }, y{ y }
{}

Vector2::Vector2(float xy) : x{ xy }, y{ xy }
{}

Vector2::Vector2(const Vector3& v) : x{ v.x }, y{ v.y }
{}

Vector2 Vector2::Zero()
{
    return Vector2{ 0.0f, 0.0f };
}

Vector2 Vector2::One()
{
    return Vector2{ 1.0f, 1.0f };
}

Vector2 Vector2::Up()
{
    return Vector2{ 0.0f, 1.0f };
}

Vector2 Vector2::Down()
{
    return Vector2{ 0.0f, -1.0f };
}

Vector2 Vector2::Left()
{
    return Vector2{ -1.0f, 0.0f };
}

Vector2 Vector2::Right()
{
    return Vector2{ 1.0f, 0.0f };
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Angle(const Vector2& from, const Vector2& to)
{
    float deno = Mathf::Sqrt(from.SqrMagnitude() * to.SqrMagnitude());

    if (deno < Mathf::EPSILON_NORMAL_SQRT)
    {
        return 0.0f;
    }

    float dot = Mathf::Clamp(Dot(from, to) / deno, -1.0f, 1.0f);

    return Mathf::Acos(dot) * Mathf::RAD2DEG;
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

Vector2 Vector2::Clamp(const Vector2& vector, float min, float max)
{
    return Vector2{
        Mathf::Clamp(vector.x, min, max),
        Mathf::Clamp(vector.y, min, max)
    };
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
    if (mag > Mathf::EPSILON)
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

void Vector2::Scale(const Vector2& scalar)
{
    x *= scalar.x;
    y *= scalar.y;
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
    std::ostringstream ss;
    ss << this->x << ", ";
    ss << this->y;
    return ss.str();
}


void Vector2::operator = (const Vector2& v)
{
    Set(v);
}

void Vector2::operator=(const Vector3& v)
{
    this->x = v.x;
    this->y = v.y;
}

float Vector2::operator[](const size_t index) const
{
    if (index > 1)
    {
        throw std::out_of_range("Index is out of range");
    }

    return xy[index];
}

float& Vector2::operator[](const size_t index)
{
    if (index > 1)
    {
        throw std::out_of_range("Index is out of range");
    }

    return xy[index];
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

Vector2 operator*(const Vector2& v1, float scalar)
{
    return Vector2{ v1.x * scalar, v1.y * scalar };
}

Vector2 operator*(float scalar, const Vector2& v1)
{
    return Vector2{ v1.x * scalar, v1.y * scalar };
}

Vector2 operator/(const Vector2& v1, const Vector2& v2)
{
    return Vector2{ v1.x / v2.x, v1.y / v2.y };
}

Vector2 operator/(const Vector2& v1, float scalar)
{
    float m = 1.0f / scalar;
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

Vector2& operator*=(Vector2& v, float scalar)
{
    v.x *= scalar;
    v.y *= scalar;

    return v;
}

Vector2& operator/=(Vector2& v1, const Vector2& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;

    return v1;
}

Vector2& operator/=(Vector2& v, float scalar)
{
    float m = 1.0f / scalar;
    v.x *= m;
    v.y += m;

    return v;
}

bool operator==(const Vector2& v1, const Vector2 v2)
{
    float diff_x = v1.x - v2.x;
    float diff_y = v1.y - v2.y;
    float sqrMag = diff_x * diff_x + diff_y * diff_y;

    return sqrMag < (Mathf::EPSILON_SQRT);
}

bool operator!=(const Vector2& v1, const Vector2 v2)
{
    return !(v1 == v2);
}

std::ostream& operator<<(std::ostream& stream, const Vector2& v)
{
    return stream << v.ToString();
}
