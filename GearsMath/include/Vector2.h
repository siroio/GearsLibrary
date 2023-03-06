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

public:
    void Set(const Vector2& v);
    void Set(float v1, float v2);
    void Set(float xy);
    void Set0();

    float SqrMagnitude() const;
    float Magnitude() const;

public:
    std::string ToString() const;

public:
    void operator = (const Vector2& v);
};

std::ostream& operator << (std::ostream& stream, const Vector2& v);

#endif // !GEAR_Vector2_H
