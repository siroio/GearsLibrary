#ifndef GEAR_Vector2_H
#define GEAR_Vector2_H

#include <string>

struct Vector3;

struct Vector2
{

public:
    union
    {
        struct
        {
            float xy[2];
        };
        struct
        {
            float x, y;
        };
    };

public:
    Vector2(const Vector2& v) = default;
    Vector2(float x, float y);
    explicit Vector2(float xy);
    explicit Vector2(const Vector3& v);

public:
    static Vector2 Zero();
    static Vector2 One();
    static Vector2 Up();
    static Vector2 Down();
    static Vector2 Left();
    static Vector2 Right();

public:
    static float Dot(const Vector2& v1, const Vector2& v2);
    static float Angle(const Vector2& from, const Vector2& to);
    static float SignedAngle(const Vector2& from, const Vector2& to);
    static float Distance(const Vector2& a, const Vector2& b);
    static float SqrMagnitude(const Vector2& vector);
    static float Magnitude(const Vector2& vector);
    static Vector2 Min(const Vector2& lhs, const Vector2& rhs);
    static Vector2 Max(const Vector2& lhs, const Vector2& rhs);
    static Vector2 Scale(const Vector2& a, const Vector2& b);
    static Vector2 Reflect(const Vector2& inDirection, const Vector2& inNormal);
    static Vector2 Perpendicular(const Vector2& inDirection);
    static Vector2 Direction(const Vector2& from, const Vector2& to);
    static Vector2 Normalize(const Vector2& v);
    static Vector2 ClampMagnitude(const Vector2& vector, float maxLength);
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
    static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t);
    static Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta);

public:
    void Set(const Vector2& v);
    void Set(float x, float y);
    void Set(float xy);
    void Set0();
    void Scale(const Vector2& scale);
    void Normalize();
    float SqrMagnitude() const;
    float Magnitude() const;


public:
    std::string ToString() const;

public:
    void operator = (const Vector2& v);
};

Vector2 operator - (const Vector2& v);
Vector2 operator + (const Vector2& v1, const Vector2& v2);
Vector2 operator - (const Vector2& v1, const Vector2& v2);
Vector2 operator * (const Vector2& v1, const Vector2& v2);
Vector2 operator * (const Vector2& v1, float scale);
Vector2 operator * (float scale, const Vector2& v1);
Vector2 operator / (const Vector2& v1, const Vector2& v2);
Vector2 operator / (const Vector2& v1, float scale);

Vector2& operator += (Vector2& v1, const Vector2& v2);
Vector2& operator -= (Vector2& v1, const Vector2& v2);
Vector2& operator *= (Vector2& v1, const Vector2& v2);
Vector2& operator *= (Vector2& v1, float scale);
Vector2& operator /= (Vector2& v1, const Vector2& v2);
Vector2& operator /= (Vector2& v1, float scale);

bool operator == (const Vector2& v1, const Vector2 v2);
bool operator != (const Vector2& v1, const Vector2 v2);

std::ostream& operator << (std::ostream& stream, const Vector2& v);

#endif // !GEAR_Vector2_H
