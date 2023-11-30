#pragma once
#include <limits>
#include <numbers>
#include <cmath>

template<class T = float> requires std::floating_point<T>
constexpr static T Epsilon()
{
    return std::numeric_limits<T>::epsilon();
}

template<class T = float>
static constexpr T PI()
{
    return T{ 3.14159265358979323846 };
}

template<class T = float>
struct Vector2
{
public:
    union
    {
        struct
        {
            T x;
            T y;
        };
        T elem[2];
    };

    Vector2() = default;
    Vector2(T x_, T y_) :
        x{ x_ }, y{ y_ }
    {}

    Vector2 operator+(const Vector2& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2& other) const
    {
        return { x - other.x, y - other.y };
    }

    Vector2 operator*(T scalar) const
    {
        return { x * scalar, y * scalar };
    }

    Vector2 operator/(T scalar) const
    {
        return { x / scalar, y / scalar };
    }

    bool operator==(const Vector2& other) const
    {
        return std::abs(x - other.x) < Epsilon<T>() && std::abs(y - other.y) < Epsilon<T>();
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
};

template<class T = float>
struct Vector3
{
public:
    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        T elem[3];
    };

    Vector3() = default;
    Vector3(T x_, T y_, T z_) :
        x{ x_ }, y{ y_ }, z{ z_ }
    {}

    static T Magnitude(const Vector3& v)
    {
        return std::sqrt(static_cast<double>(v.x * v.x + v.y * v.y + v.z * v.z));
    }

    static Vector3 Normalize(const Vector3& v)
    {
        T mag = Magnitude(v);
        Vector3 result{};
        if (mag > Epsilon())
        {
            result.x = v.x / mag;
            result.y = v.y / mag;
            result.z = v.z / mag;
        }
        else
        {
            result = { (T)0, (T)0, (T)0 };
        }
        return result;
    }

    static T Dot(const Vector3& v1, const Vector3& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static Vector3 Cross(const Vector3& v1, const Vector3& v2)
    {
        return {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        };
    }

    Vector3 operator+(const Vector3& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3 operator-(const Vector3& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3 operator*(T scalar) const
    {
        return { x * scalar, y * scalar, z * scalar };
    }

    Vector3 operator/(T scalar) const
    {
        return { x / scalar, y / scalar, z / scalar };
    }

    bool operator==(const Vector3& other) const
    {
        return std::abs(x - other.x) < Epsilon<T>() && std::abs(y - other.y) < Epsilon<T>() && std::abs(z - other.z) < Epsilon<T>();
    }

    bool operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }
};

template<class T = float>
struct Vector4
{
public:
    union
    {
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };
        T elem[4];
    };

    Vector4() = default;
    Vector4(T x_, T y_, T z_, T w_) :
        x{ x_ }, y{ y_ }, z{ z_ }, w{ w_ }
    {}

    static T Magnitude(const Vector4& v)
    {
        return std::sqrt(static_cast<double>(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
    }

    static Vector4 Normalize(const Vector4& v)
    {
        T mag = Magnitude(v);
        Vector4 result{};
        if (mag > Epsilon())
        {
            result.x = v.x / mag;
            result.y = v.y / mag;
            result.z = v.z / mag;
            result.w = v.w / mag;
        }
        else
        {
            result = { (T)0, (T)0, (T)0, (T)0 };
        }
        return result;
    }

    static T Dot(const Vector4& v1, const Vector4& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    Vector4 operator+(const Vector4& other) const
    {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    Vector4 operator-(const Vector4& other) const
    {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    Vector4 operator*(T scalar) const
    {
        return { x * scalar, y * scalar, z * scalar, w * scalar };
    }

    Vector4 operator/(T scalar) const
    {
        return { x / scalar, y / scalar, z / scalar, w / scalar };
    }

    bool operator==(const Vector4& other) const
    {
        return std::abs(x - other.x) < Epsilon<T>() && std::abs(y - other.y) < Epsilon<T>() && std::abs(z - other.z) < Epsilon<T>() && std::abs(w - other.w) < Epsilon<T>();
    }

    bool operator!=(const Vector4& other) const
    {
        return !(*this == other);
    }
};

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;
