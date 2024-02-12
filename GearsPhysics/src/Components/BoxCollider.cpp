#include <Components/BoxCollider.h>
#include <Internal/Geometry.h>
#include <PxPhysicsAPI.h>
#include <GameObject.h>
#include <GLGUI.h>
#include <Mathf.h>

void Glib::BoxCollider::Start()
{
    if (Initialize())
    {
        CreateShape(
            Internal::Geometory::CreateBox(GameObject(), halfSize_)
        );
    }
}

const Vector3& Glib::BoxCollider::Size() const
{
    return halfSize_;
}

void Glib::BoxCollider::Size(const Vector3& size)
{
    halfSize_ = size;
}

void Glib::BoxCollider::SyncGeometry()
{
    Shape()->setGeometry(Internal::Geometory::CreateBox(GameObject(), halfSize_));
}

void Glib::BoxCollider::OnGUI()
{
    Collider::OnGUI();
    GLGUI::DragVector3("Size", &halfSize_, 0.01f, Mathf::EPSILON);
}
