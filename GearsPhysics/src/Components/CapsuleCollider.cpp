#include <Components/CapsuleCollider.h>
#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <GLGUI.h>
#include <Mathf.h>

void Glib::CapsuleCollider::Start()
{
    if (Initialize())
    {
        CreateShape(
            Internal::Geometry::CreateCapsule(GameObject(), height_, radius_)
        );

        auto* shape = Shape();
        const auto& center = Center();
        auto pose = shape->getLocalPose();
        pose.p = physx::PxVec3{ center.x, center.y, center.z };
        pose.q = physx::PxQuat{ Mathf::HALF_PI, { 0.0f, 0.0f, 1.0f } };
        shape->setLocalPose(pose);
    }
}

float Glib::CapsuleCollider::Height() const
{
    return height_;
}

void Glib::CapsuleCollider::Height(float height)
{
    height_ = height;
}

float Glib::CapsuleCollider::Radius() const
{
    return radius_;
}

void Glib::CapsuleCollider::Radius(float radius)
{
    radius_ = radius;
}

void Glib::CapsuleCollider::SyncGeometry()
{
    Shape()->setGeometry(Internal::Geometry::CreateCapsule(GameObject(), height_, radius_));
}

void Glib::CapsuleCollider::OnGUI()
{
    Collider::OnGUI();

    GLGUI::DragFloat("HalfHeight", &height_, 0.01f, Mathf::EPSILON);
    GLGUI::DragFloat("Radius", &radius_, 0.01f, Mathf::EPSILON);
}
