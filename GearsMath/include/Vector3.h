#ifndef GEAR_Vector3_H
#define GEAR_Vector3_H

#include <string>

struct Vector2;

struct Vector3
{

public:
    union
    {
        struct
        {
            float xyz[3];
        };
        struct
        {
            float x, y, z;
        };
    };

public:
    Vector3(const Vector3& v) = default;
    Vector3(float x, float y, float z);
    explicit Vector3(float xyz);
    explicit Vector3(const Vector2& v);

public:
    static Vector3 Zero();
    static Vector3 One();
    static Vector3 Up();
    static Vector3 Down();
    static Vector3 Left();
    static Vector3 Right();

public:
    static float Dot(const Vector3& v1, const Vector3& v2);

public:
    void Set(const Vector3& v);
    void Set(float v1, float v2, float v3);
    void Set(float xyz);
    void Set0();

    float SqrMagnitude() const;
    float Magnitude() const;

public:
    std::string ToString() const;

public:
    void operator = (const Vector3& v);
};

std::ostream& operator << (std::ostream& stream, const Vector3& v);

#endif // !GEAR_Vector3_H
