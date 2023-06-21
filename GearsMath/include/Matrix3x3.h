#ifndef GEARS_MATRIX3X3_H
#define GEARS_MATRIX3X3_H

#include <array>
#include <string>

struct Vector2;

struct Matrix3x3
{
public:
    union
    {
        struct
        {
            float m11, m12, m13,
                m21, m22, m23,
                m31, m32, m33;
        };
        struct
        {
            std::array<std::array<float, 3>, 3> matrix;
        };
    };

public:
    Matrix3x3(const Matrix3x3&) = default;
    constexpr explicit Matrix3x3() : matrix{ 0 }
    {}

    constexpr explicit Matrix3x3(
        float m11, float m12, float m13,
        float m21, float m22, float m23,
        float m31, float m32, float m33) :
        m11{ m11 }, m12{ m12 }, m13{ m13 },
        m21{ m21 }, m22{ m22 }, m23{ m23 },
        m31{ m31 }, m32{ m32 }, m33{ m33 }
    {}

public:
    static constexpr Matrix3x3 Zero()
    {
        return Matrix3x3{
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        };
    }
    static constexpr Matrix3x3 Identity()
    {
        return Matrix3x3{
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };
    }

public:
    static Matrix3x3 Translate(const Vector2& v);
    static Matrix3x3 Rotate(float deg);
    static Matrix3x3 Scale(const Vector2& scalar);
    static Matrix3x3 Scale(float x, float y);

public:
    void Set(float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44);
    void Set(const Matrix3x3& m);
    void SetIdentity();
    std::string ToString() const;

public:
    std::array<float, 3> operator [] (const size_t index) const;
    std::array<float, 3>& operator [] (const size_t index);
};

Matrix3x3 operator += (Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator -= (Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator *= (Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator *= (Matrix3x3& m, float scalar);
Matrix3x3 operator /= (Matrix3x3& m, float scalar);

Matrix3x3 operator + (const Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator - (const Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator * (const Matrix3x3& m, float scalar);
Matrix3x3 operator * (float scalar, const Matrix3x3& m);
Matrix3x3 operator * (const Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator / (const Matrix3x3& m, float scalar);

std::ostream& operator << (std::ostream& stream, const Matrix3x3& m);

#endif // !GEARS_MATRIX3X3_H
