#include <Transform.h>

Vector3 Transform::Forward() const
{
    return rotation * Vector3::Forward();
}

Vector3 Transform::Right() const
{
    return rotation * Vector3::Right();
}

Vector3 Transform::Up() const
{
    return rotation * Vector3::Up();
}

void Transform::Forward(const Vector3& value)
{
    Rotation(Quaternion::LookRotation(value));
}

void Transform::Right(const Vector3& value)
{
    Rotation(Quaternion::FromToRotation(Vector3::Right(), value));
}

void Transform::Up(const Vector3& value)
{
    Rotation(Quaternion::FromToRotation(Vector3::Up(), value));
}

Vector3 Transform::LossyScale()
{
    return scale;
}

Vector3 Transform::Position()
{
    return position;
}

Quaternion Transform::Rotation()
{
    return rotation;
}

Vector3 Transform::EulerAngles()
{
    return rotation.EulerAngles();
}

void Transform::Position(const Vector3& position)
{
    parent != nullptr ?
        LocalPosition(parent->InverseTransformPoint(position)) :
        LocalPosition(position);
}

void Transform::Rotation(const Quaternion& rotation)
{
    parent != nullptr ?
        LocalRotation(Quaternion::Inverse(parent->rotation) * rotation) :
        LocalRotation(rotation);
}

void Transform::EulerAngles(const Vector3& eulerAngles)
{}
