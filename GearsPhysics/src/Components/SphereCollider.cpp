#include <Components/SphereCollider.h>
#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GLGUI.h>
#include <Mathf.h>

void Glib::SphereCollider::Start()
{
    if (Initialize())
    {
        CreateShape(
            Internal::Geometry::CreateSphere(GameObject(), radius_)
        );
    }
}

const float Glib::SphereCollider::Radius() const
{
    return radius_;
}

void Glib::SphereCollider::Radius(float radius)
{
    radius_ = radius;
}

void Glib::SphereCollider::SyncGeometry()
{
    Shape()->setGeometry(Internal::Geometry::CreateSphere(GameObject(), radius_));
}

void Glib::SphereCollider::OnGUI()
{
    Collider::OnGUI();
    GLGUI::DragFloat("Radius", &radius_, 0.01f, Mathf::EPSILON);
}
