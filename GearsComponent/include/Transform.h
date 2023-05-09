#ifndef GEARS_TRANSFORM_H
#define GEARS_TRANSFORM_H

#include <Vector3.h>
#include <Quaternion.h>
#include <list>

class Transform
{
private:
    Vector3 position{0.0f, 0.0f, 0.0f};
    Quaternion rotation{0.0f, 0.0f, 0.0f, 1.0f};
    Vector3 scale{0.0f, 0.0f, 0.0f};

    Vector3 local_position{0.0f, 0.0f, 0.0f};
    Quaternion local_rotation{0.0f, 0.0f, 0.0f, 1.0f};
    Vector3 local_scale{0.0f, 0.0f, 0.0f};

    Transform *parent{nullptr};
    std::list<Transform *> children;

public:
    enum class Space
    {
        Local,
        World,
    };

    Transform() = default;
    ~Transform() = default;

    Vector3 Forward() const;
    Vector3 Right() const;
    Vector3 Up() const;
    void Forward(const Vector3 &value);
    void Right(const Vector3 &value);
    void Up(const Vector3 &value);

    Vector3 LossyScale();
    Vector3 Position();
    Quaternion Rotation();
    Vector3 EulerAngles();
    void Position(const Vector3 &position);
    void Rotation(const Quaternion &rotation);
    void EulerAngles(const Vector3 &eulerAngles);

    Vector3 LocalPosition() const;
    Quaternion LocalRotation() const;
    Vector3 LocalScale() const;
    Vector3 LocalEulerAngles() const;
    void LocalPosition(const Vector3 &position);
    void LocalRotation(const Quaternion &rotation);
    void LocalScale(const Vector3 &scale);
    void LocalEulerAngles(const Vector3 &angles);

    void LookAt(const Transform &target, const Vector3 &up = Vector3::Up());
    void LookAt(const Vector3 &target, const Vector3 &up = Vector3::Up());
    void Rotate(const Vector3 &angles, Space relativeTo = Space::Local);
    void Rotate(const Vector3 &axis, float angle, Space relativeTo = Space::Local);
    void RotateAround(const Vector3 &point, const Vector3 &axis, float angle);
    void Translate(const Vector3 &translation, Space relativeTo = Space::Local);

    Matrix4x4 LocalToWorldMatrix() const;
    Matrix4x4 WorldToLocalMatrix() const;

    Vector3 TransformPoint(const Vector3 &position) const;
    Vector3 TransformVector(const Vector3 &vector) const;
    Vector3 TransformDirection(const Vector3 &direction) const;

    Vector3 InverseTransformPoint(const Vector3 &position) const;
    Vector3 InverseTransformVector(const Vector3 &vector) const;
    Vector3 InverseTransformDirection(const Vector3 &direction) const;

    void Parent(Transform *parent);
    void Parent(Transform *parent, bool worldPositionStays = true);
    Transform *Parent() const;

    void DetachParent();
    void DetachChildren();

private:
    void UpdateWorldTransform();
};
#endif // !GEARS_TRANSFORM_H
