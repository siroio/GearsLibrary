#pragma once
#include <iosfwd>
#include <string>
#include <array>

struct Vector2;
struct Quaternion;
struct Matrix4x4;

struct Vector3
{

public:
    union
    {
        struct
        {
            float x, y, z;
        };
        struct
        {
            std::array<float, 3> xyz;
        };
    };

public:
    Vector3(const Vector3&) = default;
    explicit Vector3();
    explicit Vector3(float x, float y, float z);
    explicit Vector3(float xyz);
    explicit Vector3(const Vector2& v);

public:
    static Vector3 Zero();
    static Vector3 One();
    static Vector3 Forward();
    static Vector3 Back();
    static Vector3 Up();
    static Vector3 Down();
    static Vector3 Left();
    static Vector3 Right();
    static Vector3 Divide(const Vector3& v1, const Vector3 v2, float threshold = 1e-5f);
    static float Dot(const Vector3& v1, const Vector3& v2);
    static float Angle(const Vector3& from, const Vector3& to);
    static float SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis);
    static float Distance(const Vector3 a, const Vector3 b);
    static float SqrMagnitude(const Vector3& vector);
    static float Magnitude(const Vector3& vector);
    static Vector3 Min(const Vector3& lhs, const Vector3& rhs);
    static Vector3 Max(const Vector3& lhs, const Vector3& rhs);
    static Vector3 Cross(const Vector3& a, const Vector3& b);
    static Vector3 Scale(const Vector3& a, const Vector3& b);
    static Vector3 Scale(const Vector3& v, float scalar);
    static Vector3 Reflect(const Vector3& inDirection, const Vector3& inNormal);
    static Vector3 Project(const Vector3& vector, const Vector3& onNormal);
    static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal);
    static Vector3 ClampMagnitude(const Vector3& vector, float maxLength);
    static Vector3 Normalize(const Vector3& v);
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
    static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t);
    static Vector3 SLerp(const Vector3& a, const Vector3& b, float t);
    static Vector3 SlerpUnclamped(const Vector3& a, const Vector3& b, float t);
    static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);

public:
    void Set(const Vector3& v);
    void Set(float x, float y, float z);
    void Set(float xyz);
    void Set0();
    float SqrMagnitude() const;
    float Magnitude() const;
    void Normalize();
    Vector3 Normalized() const;
    std::string ToString() const;

public:
    void operator = (const Vector3& v);
    float operator [] (const size_t index) const;
    float& operator [] (const size_t index);
};

Vector3 operator - (const Vector3& v);
Vector3 operator + (const Vector3& v1, const Vector3& v2);
Vector3 operator - (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Quaternion& q, const Vector3& v);
Vector3 operator * (const Vector3& v1, float scalar);
Vector3 operator * (float scalar, const Vector3& v1);
Vector3 operator * (const Vector3& v, const Matrix4x4& m);
Vector3 operator * (const Quaternion& q, const Vector3& v);
Vector3 operator / (const Vector3& v1, const Vector3& v2);
Vector3 operator / (const Vector3& v1, float scalar);

Vector3& operator += (Vector3& v1, const Vector3& v2);
Vector3& operator -= (Vector3& v1, const Vector3& v2);
Vector3& operator *= (Vector3& v1, const Vector3& v2);
Vector3& operator *= (Vector3& v, const Matrix4x4& m);
Vector3& operator *= (Vector3& v, float scalar);
Vector3& operator /= (Vector3& v1, const Vector3& v2);
Vector3& operator /= (Vector3& v, float scalar);

bool operator == (const Vector3& v1, const Vector3 v2);
bool operator != (const Vector3& v1, const Vector3 v2);

std::ostream& operator << (std::ostream& stream, const Vector3& v);
