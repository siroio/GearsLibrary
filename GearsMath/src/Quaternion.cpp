#include <Quaternion.h>
#include <Vector3.h>
#include <Mathf.h>
#include <sstream>

Quaternion::Quaternion() : q{ 0.0f }
{}

Quaternion::Quaternion(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w }
{}

Quaternion Quaternion::Identity()
{
    return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
}

float Quaternion::Angle(const Quaternion& q1, const Quaternion& q2)
{
    float dot = Dot(q1, q2);
    if (dot > 1.0f - Mathf::EPSILON) return 0.0f;
    return Mathf::Acos(Mathf::Clamp(dot, -1.0f, 1.0f)) * 2.0f;
}

Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis)
{
    float rad = angle * 0.5f * Mathf::DEG2RAD;
    float sin = Mathf::Sin(rad) / axis.Magnitude();
    return Quaternion{ axis.x * sin, axis.y * sin, axis.z * sin, Mathf::Cos(rad) };
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
    return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
    float mag = Mathf::Sqrt(Dot(q, q));
    float magInv = Mathf::Inverse(mag);
    if (mag < Mathf::EPSILON)
    {
        return Identity();
    }

    return Quaternion{ q.x * magInv, q.y * magInv, q.z * magInv, q.w * magInv };
}

Quaternion Quaternion::Euler(float x, float y, float z)
{
    return Euler(Vector3{ x, y, z });
}

Quaternion Quaternion::Euler(const Vector3& euler)
{
    Vector3 v = euler * Mathf::DEG2RAD;
    float xCos = Mathf::Cos(v.x * 0.5f);
    float yCos = Mathf::Cos(v.y * 0.5f);
    float zCos = Mathf::Cos(v.z * 0.5f);
    float xSin = Mathf::Sin(v.x * 0.5f);
    float ySin = Mathf::Sin(v.y * 0.5f);
    float zSin = Mathf::Sin(v.z * 0.5f);
    float x = xSin * yCos * zCos + xCos * ySin * zSin;
    float y = xCos * ySin * zCos - xSin * yCos * zSin;
    float z = xCos * yCos * zSin - xSin * ySin * zCos;
    float w = xCos * yCos * zCos + xSin * ySin * zSin;
    return Quaternion{ x, y, z, w };
}

Quaternion Quaternion::Inverse(const Quaternion& q)
{
    return Quaternion{ -q.x, -q.y, -q.z, q.w };
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
    return SlerpUnclamped(a, b, Mathf::Clamp01(t));
}

Quaternion Quaternion::SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t)
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

    if (dot < 1.0f - Mathf::EPSILON)
    {
        float theta = Mathf::Acos(dot);
        t0 = Mathf::Sin(theta * t0) / Mathf::Sin(theta);
        t1 = Mathf::Sin(theta * t1) / Mathf::Sin(theta);
    }

    return ((a * t0) + (b * sign * t1)).Normalized();
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
{
    float angle = Vector3::Angle(fromDirection, toDirection);
    if (angle < Mathf::EPSILON) return Identity();

    Vector3 axis = Vector3::Cross(fromDirection, toDirection);
    if (axis.SqrMagnitude() < Mathf::EPSILON_SQRT)
    {
        axis = Vector3::Cross(Vector3{ 0.0f, 0.0f, 1.0f }, fromDirection);
        if (axis.SqrMagnitude() < Mathf::EPSILON_SQRT)
        {
            axis = Vector3::Cross(Vector3{ 1.0f, 0.0f, 0.0f }, fromDirection);
        }
    }

    return AngleAxis(angle, axis);
}

Quaternion Quaternion::LookRotation(const Vector3& view, const Vector3& up)
{
    Vector3 forward = Vector3::Normalize(view);
    if (forward.SqrMagnitude() < (Mathf::EPSILON_SQRT))
    {
        forward = Vector3{ 0.0f, 0.0f, 1.0f };
    }

    Vector3 right = Vector3::Cross(up, forward);
    if (right.SqrMagnitude() < (Mathf::EPSILON_SQRT))
    {
        right = Vector3{ 1.0f, 0.0f, 0.0f };
    }

    Vector3 upwards = Vector3::Cross(forward, right);
    upwards.Normalize();
    right = Vector3::Cross(forward, upwards);
    Quaternion result;
    float radicand = right.x + upwards.y + forward.z;
    if (radicand > 0.0f)
    {
        result.w = Mathf::Sqrt(1.0f + radicand) * 0.5f;
        float recip = 1.0f / (4.0f * result.w);
        result.x = (upwards.z - forward.y) * recip;
        result.y = (forward.x - right.z) * recip;
        result.z = (right.y - upwards.x) * recip;
    }
    else if (right.x >= upwards.y && right.x >= forward.z)
    {
        result.x = Mathf::Sqrt(1.0f + right.x - upwards.y - forward.z) * 0.5f;
        float recip = 1.0f / (4.0f * result.x);
        result.w = (upwards.z - forward.y) * recip;
        result.z = (forward.x + right.z) * recip;
        result.y = (right.y + upwards.x) * recip;
    }
    else if (upwards.y > forward.z)
    {
        result.y = Mathf::Sqrt(1.0f - right.x + upwards.y - forward.z) * 0.5f;
        float recip = 1.0f / (4.0f * result.y);
        result.z = (upwards.z + forward.y) * recip;
        result.w = (forward.x - right.z) * recip;
        result.x = (right.y + upwards.x) * recip;
    }
    else
    {
        result.z = Mathf::Sqrt(1.0f - right.x - upwards.y + forward.z) * 0.5f;
        float recip = 1.0f / (4.0f * result.z);
        result.y = (upwards.z + forward.y) * recip;
        result.x = (forward.x + right.z) * recip;
        result.w = (right.y - upwards.x) * recip;
    }
    return result;
}

Quaternion Quaternion::LookRotation(const Vector3& view)
{
    return LookRotation(view, Vector3::Up());
}

Quaternion Quaternion::RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
{
    float angle = Angle(from, to);
    if (angle == 0.0f) return to;
    return SlerpUnclamped(from, to, Mathf::Min(1.0f, maxDegreesDelta / angle));
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

void Quaternion::SetLookRotation(const Vector3& view, const Vector3& up)
{
    LookRotation(view, up);
}

Vector3 Quaternion::EulerAngles() const
{
    Vector3 ax = *this * Vector3::Right();
    Vector3 ay = *this * Vector3::Up();
    Vector3 az = *this * Vector3::Forward();
    Vector3 result = Vector3::Zero();

    if (az.y < 1.0f && az.y > -1.0f)
    {
        result.x = Mathf::FastAsin(-az.y);
        result.y = Mathf::Atan2(az.x, az.z);
        result.z = Mathf::Atan2(ax.y, ay.y);
    }
    else
    {
        if (az.y <= -1.0f)
        {
            result.x = Mathf::HALF_PI;
            result.y = -Mathf::Atan2(-ay.x, ax.x);
            result.z = 0.0f;
        }
        else
        {
            result.x = -Mathf::HALF_PI;
            result.y = Mathf::Atan2(-ay.x, ax.x);
            result.z = 0.0f;
        }
    }

    result *= Mathf::RAD2DEG;
    MakePositive(result);
    return result;
}

void Quaternion::EulerAngles(const Vector3& angles)
{
    *this = Euler(angles);
}

void Quaternion::Normalize()
{
    *this = Normalize(*this);
}

Quaternion Quaternion::Normalized() const
{
    return Normalize(*this);
}

std::string Quaternion::ToString() const
{
    std::ostringstream ss;
    ss << this->x << ", ";
    ss << this->y << ", ";
    ss << this->z << ", ";
    ss << this->w;
    return ss.str();
}

float Quaternion::NormalizeAxis(float angle)
{
    angle = Mathf::Mod(angle, 360.0f);
    if (angle < 0.0f) angle += 360.0f;
    if (angle > 180.0f) angle -= 360.0f;

    return angle;
}

void Quaternion::MakePositive(Vector3& angles)
{
    constexpr float NegativeFlip = -Mathf::EPSILON * Mathf::RAD2DEG;
    constexpr float PositiveFlip = 360.0f + NegativeFlip;

    if (angles.x < NegativeFlip)
        angles.x += 360.0f;
    else if (angles.x > PositiveFlip)
        angles.x -= 360.0f;

    if (angles.y < NegativeFlip)
        angles.y += 360.0f;
    else if (angles.y > PositiveFlip)
        angles.y -= 360.0f;

    if (angles.z < NegativeFlip)
        angles.z += 360.0f;
    else if (angles.z > PositiveFlip)
        angles.z -= 360.0f;
}

void Quaternion::operator=(const Quaternion& v)
{
    Set(v);
}

float Quaternion::operator[](const size_t index) const
{
    if (index > 3)
    {
        throw std::out_of_range("Index is out of range");
    }

    return q[index];
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

Quaternion operator*(const Quaternion& q1, float scalar)
{
    return Quaternion{ q1.x * scalar, q1.y * scalar, q1.z * scalar, q1.w * scalar };
}

Quaternion operator*(float scalar, const Quaternion& q1)
{
    return q1 * scalar;
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

Quaternion& operator*=(Quaternion& q1, float scalar)
{
    q1.x *= scalar;
    q1.y *= scalar;
    q1.z *= scalar;
    q1.w *= scalar;

    return q1;
}

bool operator==(const Quaternion& q1, const Quaternion& q2)
{
    return Mathf::Abs(Quaternion::Dot(q1, q2) - 1.0f) < Mathf::EPSILON;
}

bool operator!=(const Quaternion& q1, const Quaternion& q2)
{
    return !(q1 == q2);
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
{
    return stream << q.ToString();
}
