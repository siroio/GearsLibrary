#pragma once
#include <Component.h>
#include <Vector3.h>
#include <Quaternion.h>
#include <list>

namespace Glib
{
    /**
     * @brief Transformコンポーネント
     */
    class Transform : public Component
    {
    public:
        enum class Space
        {
            Self,
            World,
        };

        Transform() = default;
        ~Transform() override;

        Vector3 Forward() const;
        Vector3 Right() const;
        Vector3 Up() const;
        void Forward(const Vector3& value);
        void Right(const Vector3& value);
        void Up(const Vector3& value);

        Vector3 LossyScale() const;
        Vector3 Position() const;
        Quaternion Rotation() const;
        Vector3 Scale() const;
        Vector3 EulerAngles() const;
        void Position(const Vector3& position);
        void Rotation(const Quaternion& rotation);
        void Scale(const Vector3& scale);
        void EulerAngles(const Vector3& eulerAngles);

        Vector3 LocalPosition() const;
        Quaternion LocalRotation() const;
        Vector3 LocalScale() const;
        Vector3 LocalEulerAngles() const;
        void LocalPosition(const Vector3& position);
        void LocalRotation(const Quaternion& rotation);
        void LocalScale(const Vector3& scale);
        void LocalEulerAngles(const Vector3& angles);

        void LookAt(const Glib::WeakPtr<Transform>& target, const Vector3& up = Vector3::Up());
        void LookAt(const Vector3& target, const Vector3& up = Vector3::Up());
        void Rotate(const Vector3& angles, Space relativeTo = Space::Self);
        void Rotate(const Vector3& axis, float angle, Space relativeTo = Space::Self);
        void RotateAround(const Vector3& point, const Vector3& axis, float angle);
        void Translate(const Vector3& translation, Space relativeTo = Space::Self);

        Matrix4x4 LocalToWorldMatrix() const;
        Matrix4x4 WorldToLocalMatrix() const;

        Vector3 TransformPoint(const Vector3& position) const;
        Vector3 TransformVector(const Vector3& vector) const;
        Vector3 TransformDirection(const Vector3& direction) const;

        Vector3 InverseTransformPoint(const Vector3& position) const;
        Vector3 InverseTransformVector(const Vector3& vector) const;
        Vector3 InverseTransformDirection(const Vector3& direction) const;

        void Parent(Glib::WeakPtr<Transform> parent);
        Glib::WeakPtr<Transform> Parent() const;
        const std::list<Glib::WeakPtr<Transform>>& Children() const;
        void ClearChildren();

    private:
        void AddChild(const Glib::WeakPtr<Transform>& child);
        void RemoveChild(const Glib::WeakPtr<Transform>& child);


    private:
        Vector3 local_position_{ Vector3::Zero() };
        Quaternion local_rotation_{ Quaternion::Identity() };
        Vector3 local_scale_{ Vector3::One() };
        Glib::WeakPtr<Transform> parent_{ nullptr };
        std::list<Glib::WeakPtr<Transform>> children_;
    };
}
