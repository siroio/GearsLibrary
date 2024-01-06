#include <Components/Transform.h>
#include <Matrix4x4.h>
#include <GLGUI.h>

Glib::Transform::~Transform()
{
    if (!parent_.expired())
    {
        parent_->RemoveChild(weak_from_this());
    }
}

Vector3 Glib::Transform::Forward() const
{
    return Rotation() * Vector3::Forward();
}

Vector3 Glib::Transform::Right() const
{
    return Rotation() * Vector3::Right();
}

Vector3 Glib::Transform::Up() const
{
    return Rotation() * Vector3::Up();
}

void Glib::Transform::Forward(const Vector3& value)
{
    Rotation(Quaternion::LookRotation(value));
}

void Glib::Transform::Right(const Vector3& value)
{
    Rotation(Quaternion::FromToRotation(Vector3::Right(), value));
}

void Glib::Transform::Up(const Vector3& value)
{
    Rotation(Quaternion::FromToRotation(Vector3::Up(), value));
}

Vector3 Glib::Transform::LossyScale() const
{
    return local_scale_;
}

Vector3 Glib::Transform::Position() const
{
    return parent_.expired() ?
        local_position_ :
        parent_->TransformPoint(local_position_);
}

Quaternion Glib::Transform::Rotation() const
{
    return parent_.expired() ?
        LocalRotation() :
        parent_->Rotation() * local_rotation_;
}

Vector3 Glib::Transform::Scale() const
{
    return parent_.expired() ?
        local_scale_ :
        Vector3::Scale(parent_->Scale(), local_scale_);
}

Vector3 Glib::Transform::EulerAngles() const
{
    return Rotation().EulerAngles();
}

void Glib::Transform::Position(const Vector3& position)
{
    parent_.expired() ?
        LocalPosition(position) :
        LocalPosition(parent_->InverseTransformPoint(position));
}

void Glib::Transform::Rotation(const Quaternion& rotation)
{
    if (parent_.expired())
    {
        local_rotation_ = rotation;
    }
    else
    {
        local_rotation_ = Quaternion::Inverse(parent_->Rotation()) * rotation;
    }
}

void Glib::Transform::Scale(const Vector3& scale)
{
    if (parent_.expired())
    {
        local_scale_ = scale;
    }
    else
    {
        local_scale_ = Vector3::Divide(local_scale_, parent_->Scale());
    }
}

void Glib::Transform::EulerAngles(const Vector3& eulerAngles)
{
    Rotation(Quaternion::Euler(eulerAngles));
}

Vector3 Glib::Transform::LocalPosition() const
{
    return local_position_;
}

Quaternion Glib::Transform::LocalRotation() const
{
    return local_rotation_;
}

Vector3 Glib::Transform::LocalScale() const
{
    return local_scale_;
}

Vector3 Glib::Transform::LocalEulerAngles() const
{
    return local_rotation_.EulerAngles();
}

void Glib::Transform::LocalPosition(const Vector3& position)
{
    local_position_ = position;
}

void Glib::Transform::LocalRotation(const Quaternion& rotation)
{
    local_rotation_ = rotation;
}

void Glib::Transform::LocalScale(const Vector3& scale)
{
    local_scale_ = scale;
}

void Glib::Transform::LocalEulerAngles(const Vector3& angles)
{
    LocalRotation(Quaternion::Euler(angles));
}

void Glib::Transform::LookAt(const Glib::WeakPtr<Transform>& target, const Vector3& up)
{
    LookAt(target->Position(), up);
}

void Glib::Transform::LookAt(const Vector3& target, const Vector3& up)
{
    Rotation(Quaternion::LookRotation(target - Position(), up));
}

void Glib::Transform::Rotate(const Vector3& angles, Space relativeTo)
{
    relativeTo == Space::Self ?
        LocalRotation(LocalRotation() * Quaternion::Euler(angles)) :
        Rotation(Quaternion::Euler(angles) * Rotation());
}

void Glib::Transform::Rotate(const Vector3& axis, float angle, Space relativeTo)
{
    relativeTo == Space::Self ?
        LocalRotation(LocalRotation() * Quaternion::AngleAxis(angle, axis)) :
        Rotation(Quaternion::AngleAxis(angle, axis) * Rotation());
}

void Glib::Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
{
    Quaternion rot = Quaternion::AngleAxis(angle, axis);
    Position(point + rot * (Position() - point));
    Rotation(rot * Rotation());
}

void Glib::Transform::Translate(const Vector3& translation, Space relativeTo)
{
    relativeTo == Space::Self ?
        Position(Position() + TransformDirection(translation)) :
        Position(Position() + translation);
}

Matrix4x4 Glib::Transform::LocalToWorldMatrix() const
{
    return Matrix4x4::TRS(Position(), Rotation(), Scale());
}

Matrix4x4 Glib::Transform::WorldToLocalMatrix() const
{
    return LocalToWorldMatrix().Inverse();
}

Vector3 Glib::Transform::TransformPoint(const Vector3& position) const
{
    return Rotation() * Vector3::Scale(position, Scale()) + Position();
}

Vector3 Glib::Transform::TransformVector(const Vector3& vector) const
{
    return Rotation() * Vector3::Scale(vector, Scale());
}

Vector3 Glib::Transform::TransformDirection(const Vector3& direction) const
{
    return Rotation() * direction;
}

Vector3 Glib::Transform::InverseTransformPoint(const Vector3& position) const
{
    Vector3 result = position - Position();
    result = Quaternion::Inverse(Rotation()) * result;
    result = Vector3::Divide(result, Scale());

    return result;
}

Vector3 Glib::Transform::InverseTransformVector(const Vector3& vector) const
{
    Vector3 result = vector;
    result = Quaternion::Inverse(Rotation()) * result;
    result = Vector3::Divide(result, Scale());

    return result;
}

Vector3 Glib::Transform::InverseTransformDirection(const Vector3& direction) const
{
    return Quaternion::Inverse(Rotation()) * direction;
}

void Glib::Transform::Parent(Glib::WeakPtr<Transform> parent)
{
    if (parent == weak_from_this()) return;
    if (!parent.expired())
    {
        parent->RemoveChild(weak_from_this());
        local_position_ = parent->TransformPoint(local_position_);
        local_rotation_ = parent->Rotation() * local_rotation_;
        local_scale_ = Vector3::Scale(local_scale_, parent->Scale());
    }

    parent_ = parent;

    if (!parent.expired())
    {
        parent_->AddChild(weak_from_this());
        local_position_ = parent_->InverseTransformPoint(local_position_);
        local_rotation_ = Quaternion::Inverse(parent->Rotation()) * local_rotation_;
        local_scale_ = Vector3::Divide(local_scale_, parent_->Scale());
    }
}

Glib::WeakPtr<Glib::Transform> Glib::Transform::Parent() const
{
    return parent_;
}

const std::list<Glib::WeakPtr<Glib::Transform>>& Glib::Transform::Children() const
{
    return children_;
}

void Glib::Transform::ClearChildren()
{
    children_.clear();
}


void Glib::Transform::AddChild(const Glib::WeakPtr<Transform>& child)
{
    children_.push_back(child);
}

void Glib::Transform::RemoveChild(const Glib::WeakPtr<Transform>& child)
{
    children_.remove(child);
}

void Glib::Transform::OnGUI()
{
    GLGUI::DragVector3("Position", &local_position_);

    auto eulr = LocalEulerAngles();
    if (GLGUI::DragVector3("Rotation", &eulr))
    {
        LocalEulerAngles(eulr);
    }

    GLGUI::DragVector3("Scale", &local_scale_);
}
