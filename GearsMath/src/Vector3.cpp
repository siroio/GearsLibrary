#include <Vector2.h>
#include <Vector3.h>
#include <Matrix4x4.h>
#include <Mathf.h>
#include <sstream>
#include <Quaternion.h>

Vector3::Vector3() : xyz{ 0.0f }
{}

Vector3::Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z }
{}

Vector3::Vector3(float xyz) : xyz{ xyz }
{}

Vector3::Vector3(const Vector2& v) : x{ v.x }, y{ v.y }, z{ 0.0f }
{}

Vector3 Vector3::Zero()
{
    return Vector3{ 0.0f, 0.0f, 0.0f };
}

Vector3 Vector3::One()
{
    return Vector3{ 1.0f, 1.0f, 1.0f };
}

Vector3 Vector3::Forward()
{
    return Vector3{ 0.0f, 0.0f, 1.0f };
}

Vector3 Vector3::Back()
{
    return Vector3{ 0.0f, 0.0f, -1.0f };
}

Vector3 Vector3::Up()
{
    return Vector3{ 0.0f, 1.0f, 0.0f };
}

Vector3 Vector3::Down()
{
    return Vector3{ 0.0f, -1.0f, 0.0f };
}

Vector3 Vector3::Left()
{
    return Vector3{ -1.0f, 0.0f, 0.0f };
}

Vector3 Vector3::Right()
{
    return Vector3{ 1.0f, 0.0, 0.0f };
}

Vector3 Vector3::Divide(const Vector3& v1, const Vector3 v2, float threshold)
{
    return Vector3{
        Mathf::Abs(v2.x) <= threshold ? 0.0f : v1.x / v2.x,
        Mathf::Abs(v2.y) <= threshold ? 0.0f : v1.y / v2.y,
        Mathf::Abs(v2.z) <= threshold ? 0.0f : v1.z / v2.z
    };
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector3::Angle(const Vector3& from, const Vector3& to)
{
    float deno = Mathf::Sqrt(from.SqrMagnitude() * to.SqrMagnitude());

    if (deno < Mathf::EPSILON_NORMAL_SQRT)
    {
        return 0.0f;
    }

    float dot = Mathf::Clamp(Dot(from, to) / deno, -1.0f, 1.0f);

    return Mathf::Acos(dot) * Mathf::RAD2DEG;
}

float Vector3::SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis)
{
    float unsignedAngle = Angle(from, to);

    float cross_x = from.y * to.z - from.z * to.y;
    float cross_y = from.z * to.x - from.x * to.z;
    float cross_z = from.x * to.y - from.y * to.x;
    float sign = Mathf::Sign(axis.x * cross_x + axis.y * cross_y + axis.z * cross_z);

    return unsignedAngle * sign;
}

float Vector3::Distance(const Vector3 a, const Vector3 b)
{
    float diffX = a.x - b.x;
    float diffY = a.y - b.y;
    float diffZ = a.z - b.z;

    return Mathf::Sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
}

float Vector3::SqrMagnitude(const Vector3& vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

float Vector3::Magnitude(const Vector3& vector)
{
    return Mathf::Sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3 Vector3::Min(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3{ Mathf::Min(lhs.x, rhs.x), Mathf::Min(lhs.y, rhs.y), Mathf::Min(lhs.z, rhs.z) };
}

Vector3 Vector3::Max(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3{ Mathf::Max(lhs.x, rhs.x), Mathf::Max(lhs.y, rhs.y), Mathf::Max(lhs.z, rhs.z) };
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
    return Vector3{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

Vector3 Vector3::Scale(const Vector3& a, const Vector3& b)
{
    return Vector3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

Vector3 Vector3::Scale(const Vector3& v, float scalar)
{
    return Vector3{ v.x * scalar, v.y * scalar, v.z * scalar };
}

Vector3 Vector3::Reflect(const Vector3& inDirection, const Vector3& inNormal)
{
    float factor = -2.0f * Dot(inNormal, inDirection);

    return Vector3{
        factor * inNormal.x * inDirection.x,
        factor * inNormal.y * inDirection.y,
        factor * inNormal.z * inDirection.z,
    };
}

Vector3 Vector3::Project(const Vector3& vector, const Vector3& onNormal)
{
    float sqrMag = Dot(onNormal, onNormal);

    if (sqrMag < Mathf::EPSILON)
    {
        return Zero();
    }
    else
    {
        float dot = Dot(vector, onNormal);

        return Vector3{
            onNormal.x * dot / sqrMag,
            onNormal.y * dot / sqrMag,
            onNormal.z * dot / sqrMag
        };
    }
}

Vector3 Vector3::ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
{
    float sqrMag = Dot(planeNormal, planeNormal);

    if (sqrMag < Mathf::EPSILON)
    {
        return vector;
    }
    else
    {
        float dot = Dot(vector, planeNormal);

        return Vector3{
            vector.x - planeNormal.x * dot / sqrMag,
            vector.y - planeNormal.y * dot / sqrMag,
            vector.z - planeNormal.z * dot / sqrMag
        };
    }
}

Vector3 Vector3::ClampMagnitude(const Vector3& vector, float maxLength)
{
    float sqrMag = vector.SqrMagnitude();

    if (sqrMag > maxLength * maxLength)
    {
        float mag = Mathf::Sqrt(sqrMag);
        float normalizedX = vector.x / mag;
        float normalizedY = vector.y / mag;
        float normalizedZ = vector.z / mag;

        return Vector3{ normalizedX * maxLength, normalizedY * maxLength, normalizedZ * maxLength };
    }

    return vector;
}

Vector3 Vector3::Normalize(const Vector3& v)
{
    float mag = Magnitude(v);

    if (mag <= Mathf::EPSILON) return Zero();

    float inv = Mathf::Inverse(mag);
    return Vector3{ v.x * inv, v.y * inv, v.z * inv };
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
{
    t = Mathf::Clamp01(t);
    float lerp_x = a.x + (b.x - a.x) * t;
    float lerp_y = a.y + (b.y - a.y) * t;
    float lerp_z = a.z + (b.z - a.z) * t;

    return Vector3{ lerp_x, lerp_y, lerp_z };
}

Vector3 Vector3::LerpUnclamped(const Vector3& a, const Vector3& b, float t)
{
    float lerp_x = a.x + (b.x - a.x) * t;
    float lerp_y = a.y + (b.y - a.y) * t;
    float lerp_z = a.z + (b.z - a.z) * t;

    return Vector3{ lerp_x, lerp_y, lerp_z };
}

Vector3 Vector3::SLerp(const Vector3& a, const Vector3& b, float t)
{
    float dot = Dot(a, b);
    dot = Mathf::Clamp(dot, -1.0f, 1.0f);
    float theta = Mathf::Acos(dot) * t;
    Vector3 relative = b - a * dot;
    relative.Normalize();
    return ((a * Mathf::Cos(theta)) + (relative * Mathf::Sin(theta)));
}

Vector3 Vector3::SlerpUnclamped(const Vector3& a, const Vector3& b, float t)
{
    float dot = Dot(a, b);
    float theta = Mathf::Acos(dot) * t;
    Vector3 relative = b - a * dot;
    relative.Normalize();
    return ((a * Mathf::Cos(theta)) + (relative * Mathf::Sin(theta)));
}

Vector3 Vector3::MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta)
{
    float toVectorX = target.x - current.x;
    float toVectorY = target.y - current.y;
    float toVectorZ = target.z - current.z;

    float sqrDist = toVectorX * toVectorX + toVectorY * toVectorY + toVectorZ * toVectorZ;

    if (sqrDist == 0 || (maxDistanceDelta >= 0 && sqrDist <= maxDistanceDelta * maxDistanceDelta))
    {
        return target;
    }

    float dist = Mathf::Sqrt(sqrDist);

    return Vector3{
        current.x + toVectorX / dist * maxDistanceDelta,
        current.y + toVectorY / dist * maxDistanceDelta,
        current.z + toVectorZ / dist * maxDistanceDelta
    };
}

void Vector3::Set(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void Vector3::Set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
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
    return Mathf::Sqrt(SqrMagnitude());
}

void Vector3::Normalize()
{
    *this = Normalize(*this);
}

Vector3 Vector3::Normalized() const
{
    return Normalize(*this);
}

std::string Vector3::ToString() const
{
    std::ostringstream ss;
    ss << this->x << ", ";
    ss << this->y << ", ";
    ss << this->z;
    return ss.str();
}

void Vector3::operator = (const Vector3& v)
{
    Set(v);
}

float Vector3::operator[](const size_t index) const
{
    if (index > 2)
    {
        throw std::out_of_range("Index is out of range");
    }

    return xyz[index];
}

float& Vector3::operator[](const size_t index)
{
    if (index > 2)
    {
        throw std::out_of_range("Index is out of range");
    }

    return xyz[index];
}

Vector3 operator-(const Vector3& v)
{
    return Vector3{ -v.x, -v.y, -v.z };
}

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Vector3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Vector3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
    return Vector3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

Vector3 operator*(const Quaternion& q, const Vector3& v)
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

    Vector3 res;
    res.x = (1.0f - (yy + zz)) * v.x + (xy - wz) * v.y + (xz + wy) * v.z;
    res.y = (xy + wz) * v.x + (1.0f - (xx + zz)) * v.y + (yz - wx) * v.z;
    res.z = (xz - wy) * v.x + (yz + wx) * v.y + (1.0f - (xx + yy)) * v.z;
    return res;
}

Vector3 operator*(const Vector3& v1, float scalar)
{
    return Vector3{ v1.x * scalar, v1.y * scalar, v1.z * scalar };
}

Vector3 operator*(float scalar, const Vector3& v1)
{
    return Vector3{ v1.x * scalar, v1.y * scalar, v1.z * scalar };
}

Vector3 operator*(const Vector3& v, const Matrix4x4& m)
{
    return m.MultiplyPoint(v);
}

Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
    return Vector3{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

Vector3 operator/(const Vector3& v1, float scalar)
{
    float m = 1.0f / scalar;
    return Vector3{ v1.x * m, v1.y * m, v1.z * m };
}

Vector3& operator+=(Vector3& v1, const Vector3& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

Vector3& operator*=(Vector3& v1, const Vector3& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;

    return v1;
}

Vector3& operator*=(Vector3& v, const Matrix4x4& m)
{
    v = m.MultiplyPoint(v);
    return v;
}

Vector3& operator*=(Vector3& v, float scalar)
{
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;

    return v;
}

Vector3& operator/=(Vector3& v1, const Vector3& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;

    return v1;
}

Vector3& operator/=(Vector3& v, float scalar)
{
    float m = 1.0f / scalar;
    v.x *= m;
    v.y += m;
    v.z += m;

    return v;
}

bool operator==(const Vector3& v1, const Vector3 v2)
{
    float diff_x = v1.x - v2.x;
    float diff_y = v1.y - v2.y;
    float diff_z = v1.z - v2.z;
    float sqrMag = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;

    return sqrMag < (Mathf::EPSILON_SQRT);
}

bool operator!=(const Vector3& v1, const Vector3 v2)
{
    return !(v1 == v2);
}

std::ostream& operator<<(std::ostream& stream, const Vector3& v)
{
    return stream << v.ToString();
}
