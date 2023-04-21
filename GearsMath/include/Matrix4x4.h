#ifndef GEAR_MATRIX4X4_H
#define GEAR_MATRIX4X4_H

#include <string>

struct Vector3;
struct Vector4;
struct Quaternion;

struct Matrix4x4
{
public:
    union
    {
        struct
        {
            float m11, m12, m13, m14,
                m21, m22, m23, m24,
                m31, m32, m33, m34,
                m41, m42, m43, m44;
        };
        struct
        {
            float matrix[4][4];
        };
    };

public:
    Matrix4x4() = default;
    Matrix4x4(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44);

public:
    static Matrix4x4 Zero();
    static Matrix4x4 Identity();
    static Matrix4x4 Scale(const Vector3& scale);
    static Matrix4x4 Scale(float x, float y, float z);
    static Matrix4x4 LookAt(const Vector3& position, const Vector3& target, const Vector3& up);
    static Matrix4x4 Translate(const Vector3& v);
    static Matrix4x4 Rotate(const Quaternion& q);
    static Matrix4x4 RotationX(float deg);
    static Matrix4x4 RotationY(float deg);
    static Matrix4x4 RotationZ(float deg);
    static Matrix4x4 TRS(const Vector3& p, const Quaternion& q, const Vector3& s);
    static Matrix4x4 Perspective(float width, float height, float nearDistance, float farDistance);
    static Matrix4x4 PerspectiveFOV(float fieldOfView, float aspectRatio, float nearDistance, float farDistance);
    static Matrix4x4 Orthographic(float width, float height, float nearZ, float farZ);

public:
    Vector3 MultiplyPoint(const Vector3& point) const;
    Vector3 MultiplyPoint4x3(const Vector3& point) const;
    Vector3 MultiplyVector(const Vector3& vector) const;

    Vector3 Scale() const;
    Vector3 Position() const;
    Quaternion Rotation() const;

    Vector3 Forward() const;
    Vector3 Up() const;
    Vector3 Right() const;

    Matrix4x4 Inverse() const;
    Matrix4x4 Transpose() const;

    void SetTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

    std::string ToString();
};
#endif // !GEAR_RANDOM_H
