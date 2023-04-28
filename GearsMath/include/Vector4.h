#ifndef GEAR_VECTOR4_H
#define GEAR_VECTOR4_H

#include <iosfwd>
#include <string>
#include <array>

struct Vector3;

struct Vector4
{

public:
    union
    {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            std::array<float, 4> xyzw;
        };
    };

public:
    Vector4(const Vector4& v) = default;
    explicit Vector4();
    explicit Vector4(float x, float y, float z, float w);
    explicit Vector4(float xyzw);

public:
    static Vector4 Zero();
    static Vector4 One();

public:
    static float Dot(const Vector4& v1, const Vector4& v2);
    static float Distance(const Vector4 a, const Vector4 b);
    static float SqrMagnitude(const Vector4& vector);
    static float Magnitude(const Vector4& vector);
    static Vector4 Min(const Vector4& lhs, const Vector4& rhs);
    static Vector4 Max(const Vector4& lhs, const Vector4& rhs);
    static Vector4 Scale(const Vector4& a, const Vector4& b);
    static Vector4 Scale(const Vector4& v, float scalar);

public:
    void Set(const Vector4& v);
    void Set(float x, float y, float z, float w);
    void Set(float xyzw);
    void Set0();
    float SqrMagnitude() const;
    float Magnitude() const;
    std::string ToString() const;

public:
    void operator = (const Vector4& v);
    float operator [] (const size_t index) const;
    float& operator [] (const size_t index);
};

Vector4 operator - (const Vector4& v);
Vector4 operator + (const Vector4& v1, const Vector4& v2);
Vector4 operator - (const Vector4& v1, const Vector4& v2);
Vector4 operator * (const Vector4& v1, const Vector4& v2);
Vector4 operator * (const Vector4& v1, float scalar);
Vector4 operator * (float scalar, const Vector4& v1);
Vector4 operator / (const Vector4& v1, const Vector4& v2);
Vector4 operator / (const Vector4& v1, float scalar);

Vector4& operator += (Vector4& v1, const Vector4& v2);
Vector4& operator -= (Vector4& v1, const Vector4& v2);
Vector4& operator *= (Vector4& v1, const Vector4& v2);
Vector4& operator *= (Vector4& v1, float scalar);
Vector4& operator /= (Vector4& v1, const Vector4& v2);
Vector4& operator /= (Vector4& v1, float scalar);

bool operator == (const Vector4& v1, const Vector4 v2);
bool operator != (const Vector4& v1, const Vector4 v2);

std::ostream& operator<<(std::ostream& stream, const Vector4& v);

#endif // !GEAR_VECTOR4_H
